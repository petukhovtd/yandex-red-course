#include "test_runner.h"

#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// Token bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
     vector<Sentence<Token>> result;
     for( Token& token: tokens )
     {
          if( result.empty() || ( !result.back().empty()
                                  && result.back().back().IsEndSentencePunctuation()
                                  && !token.IsEndSentencePunctuation() ) )
          {
               result.push_back(Sentence<Token>());
          }
          result.back().push_back( std::move( token ));
     }
     return result;
}


struct TestToken {
     string data;
     bool is_end_sentence_punctuation = false;

     bool IsEndSentencePunctuation() const {
          return is_end_sentence_punctuation;
     }
     bool operator==(const TestToken& other) const {
          return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
     }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
     return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
     ASSERT_EQUAL(
               SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
               vector<Sentence<TestToken>>({
                                                     {{"Split"}, {"into"}, {"sentences"}, {"!"}}
                                           })
     );

     ASSERT_EQUAL(
               SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
               vector<Sentence<TestToken>>({
                                                     {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
                                           })
     );

     ASSERT_EQUAL(
               SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
               vector<Sentence<TestToken>>({
                                                     {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
                                                     {{"Without"}, {"copies"}, {".", true}},
                                           })
     );
}

int main() {
     TestRunner tr;
     RUN_TEST(tr, TestSplitting);
     return 0;
}