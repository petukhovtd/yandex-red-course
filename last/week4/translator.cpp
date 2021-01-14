#include "test_runner.h"
#include <string>
#include <vector>

using namespace std;

class Translator
{
public:
     void Add( string_view source, string_view target )
     {
          auto sit = dic_.insert( std::string( source ));
          auto tit = dic_.insert( std::string( target ));
          forward_[ std::string_view( *sit.first ) ] = std::string_view( *tit.first );
          backward_[ std::string_view( *tit.first ) ] = std::string_view( *sit.first );
     }

     string_view TranslateForward( string_view source ) const
     {
          auto it = forward_.find( source );
          if( it == forward_.end() )
          {
               return std::string_view();
          }
          return it->second;
     }

     string_view TranslateBackward( string_view target ) const
     {
          auto it = backward_.find( target );
          if( it == backward_.end() )
          {
               return std::string_view();
          }
          return it->second;
     }

private:
     std::set< std::string > dic_;
     std::map< std::string_view, std::string_view > forward_;
     std::map< std::string_view, std::string_view > backward_;
};

void TestSimple()
{
     Translator translator;
     translator.Add( string( "okno" ), string( "window" ) );
     translator.Add( string( "stol" ), string( "table" ) );

     ASSERT_EQUAL( translator.TranslateForward( "okno" ), "window" );
     ASSERT_EQUAL( translator.TranslateBackward( "table" ), "stol" );
     ASSERT_EQUAL( translator.TranslateBackward( "stol" ), "" );
}

int main()
{
     TestRunner tr;
     RUN_TEST( tr, TestSimple );
     return 0;
}
