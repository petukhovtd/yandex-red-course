#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <fstream>

using namespace std;

template< typename Iterator >
class Page
{
public:
     Page( Iterator begin, Iterator end )
               : begin_( begin )
               , end_( end )
     {}

     Iterator begin() const
     {
          return begin_;
     }

     Iterator end() const
     {
          return end_;
     }

     std::size_t size() const
     {
          return std::distance( begin_, end_ );
     }

private:
     Iterator begin_;
     Iterator end_;
};

template< typename Iterator >
class Paginator
{
     using PagesIterator = typename std::vector< Page< Iterator > >::iterator;
     using PagesConstIterator = typename std::vector< Page< Iterator > >::const_iterator;
public:
     Paginator( Iterator begin, Iterator end, size_t pageSize )
     {
          while( true )
          {
               std::size_t size = std::distance( begin, end );
               if( size == 0 )
               {
                    break;
               }
               Page page( begin, next( begin, min( pageSize, size ) ) );
               begin = page.end();
               pages_.push_back( page );
          }
     }

     PagesIterator begin()
     {
          return pages_.begin();
     }

     PagesConstIterator begin() const
     {
          return pages_.begin();
     }

     PagesIterator end()
     {
          return pages_.end();
     }

     PagesConstIterator end() const
     {
          return pages_.end();
     }

     size_t size() const
     {
          return pages_.size();
     }

private:
     std::vector< Page< Iterator > > pages_;
};

template< typename C >
auto Paginate( C& c, size_t page_size )
{
     return Paginator( begin( c ), end( c ), page_size );
}

struct Stats
{
     Stats() = default;

     map< string, int > word_frequences;

     void Add( std::string const& word )
     {
          word_frequences[ word ]++;
     }

     void operator+=( const Stats& other )
     {
          for( auto&[key, value]: other.word_frequences )
          {
               word_frequences[ key ] += value;
          }
     }
};

Stats ExploreLine( const set< string >& key_words, const string& line )
{
     Stats stats;
     std::stringstream ss( line );
     while( ss && ss.good() )
     {
          std::string s;
          ss >> s;
          if( key_words.count( s ) )
          {
               stats.Add( s );
          }
     }
     return stats;
}

Stats ExploreKeyWordsSingleThread(
          const set< string >& key_words, istream& input
)
{
     Stats result;
     for( string line; getline( input, line ); )
     {
          result += ExploreLine( key_words, line );
     }
     return result;
}

Stats ExploreKeyWords( const set< string >& key_words, istream& input )
{
     Stats stats;
     std::vector< std::future< Stats > > futures;
     std::vector< std::string > text;
     for( string line; getline( input, line ); )
     {
          text.push_back( std::move( line ) );
     }

     for( auto& page: Paginate( text, text.size() > 4? text.size() / 4: 4 ) )
     {
          futures.push_back( std::async( [ page, &text, &key_words ]()
                                         {
                                              Stats stats;
                                              for( auto const& line: page )
                                              {
                                                   stats += ExploreLine( key_words, line );
                                              }
                                              return stats;
                                         } ) );
     }

     for( auto& f: futures )
     {
          stats += f.get();
     }
     return stats;
}

void TestLine()
{
     const set< string > key_words = { "yangle", "rocks", "sucks", "all" };

     const auto stats = ExploreLine( key_words, "this new yangle service really rocks" );
     const map< string, int > expected = {
               { "yangle", 1 },
               { "rocks",  1 },
     };
     ASSERT_EQUAL( stats.word_frequences, expected );
}

void TestBasicSingle()
{
     const set< string > key_words = { "yangle", "rocks", "sucks", "all" };

     stringstream ss;
     ss << "this new yangle service really rocks\n";
     ss << "It sucks when yangle isn't available\n";
     ss << "10 reasons why yangle is the best IT company\n";
     ss << "yangle rocks others suck\n";
     ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

     const auto stats = ExploreKeyWordsSingleThread( key_words, ss );
     const map< string, int > expected = {
               { "yangle", 6 },
               { "rocks",  2 },
               { "sucks",  1 },
     };
     ASSERT_EQUAL( stats.word_frequences, expected );
}

void TestBasic()
{
     const set< string > key_words = { "yangle", "rocks", "sucks", "all" };

     stringstream ss;
     ss << "this new yangle service really rocks\n";
     ss << "It sucks when yangle isn't available\n";
     ss << "10 reasons why yangle is the best IT company\n";
     ss << "yangle rocks others suck\n";
     ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

     const auto stats = ExploreKeyWords( key_words, ss );
     const map< string, int > expected = {
               { "yangle", 6 },
               { "rocks",  2 },
               { "sucks",  1 },
     };
     ASSERT_EQUAL( stats.word_frequences, expected );
}

int main()
{
     TestRunner tr;
     RUN_TEST( tr, TestLine );
     RUN_TEST( tr, TestBasicSingle );
     RUN_TEST( tr, TestBasic );
}