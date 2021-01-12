#include <string>
#include <list>
#include "test_runner.h"

using namespace std;

class Editor
{
     using Container = std::list< char >;
     using Cursor = Container::iterator;
public:
     Editor()
               : text_()
               , buffer_()
               , cursor_( text_.end() )
     {}

     void Left()
     {
          if( cursor_ != text_.begin() )
          {
               cursor_--;
          }
     }

     void Right()
     {
          if( cursor_ != text_.end() )
          {
               cursor_++;
          }
     }

     void Insert( char token )
     {
          text_.insert( cursor_, token );
     }

     void Cut( size_t tokens = 1 )
     {
          if( tokens == 0 )
          {
               buffer_.clear();
               return;
          }
          Copy( tokens );
          auto begin = cursor_;
          if( tokens > static_cast< size_t >( std::distance( cursor_, text_.end() ) ))
          {
               cursor_ = text_.end();
          }
          else
          {
               std::advance( cursor_, tokens );
          }
          text_.erase( begin, cursor_ );
     }

     void Copy( size_t tokens = 1 )
     {
          if( tokens == 0 )
          {
               buffer_.clear();
               return;
          }
          Cursor end;
          if( tokens > static_cast< size_t >( std::distance( cursor_, text_.end() ) ))
          {
               end = text_.end();
          }
          else
          {
               end = cursor_;
               std::advance( end, tokens );
          }
          buffer_ = Container( cursor_, end );
     }

     void Paste()
     {
          if( buffer_.empty() )
          {
               return;
          }
          auto bufferCopy = buffer_;
          text_.splice( cursor_, bufferCopy );
     }

     std::string GetText() const
     {
          return std::string( text_.begin(), text_.end() );
     }

private:
     static std::size_t Counter()
     {
          static size_t test_count = 0;
          return ++test_count;
     }

private:
     Container text_;
     Container buffer_;
     Cursor cursor_;
};

void TypeText( Editor& editor, const string& text )
{
     for( char c : text )
     {
          editor.Insert( c );
     }
}

void TestEditing()
{
     {
          Editor editor;

          const size_t text_len = 12;
          const size_t first_part_len = 7;
          TypeText( editor, "hello, world" );
          for( size_t i = 0; i < text_len; ++i )
          {
               editor.Left();
          }
          editor.Cut( first_part_len );
          for( size_t i = 0; i < text_len - first_part_len; ++i )
          {
               editor.Right();
          }
          TypeText( editor, ", " );
          editor.Paste();
          editor.Left();
          editor.Left();
          editor.Cut( 3 );

          ASSERT_EQUAL( editor.GetText(), "world, hello" );
     }
     {
          Editor editor;

          TypeText( editor, "misprnit" );
          editor.Left();
          editor.Left();
          editor.Left();
          editor.Cut( 1 );
          editor.Right();
          editor.Paste();

          ASSERT_EQUAL( editor.GetText(), "misprint" );
     }
}

void TestReverse()
{
     Editor editor;

     const string text = "esreveR";
     for( char c : text )
     {
          editor.Insert( c );
          editor.Left();
     }

     ASSERT_EQUAL( editor.GetText(), "Reverse" );
}

void TestNoText()
{
     Editor editor;
     ASSERT_EQUAL( editor.GetText(), "" );

     editor.Left();
     editor.Left();
     editor.Right();
     editor.Right();
     editor.Copy( 0 );
     editor.Cut( 0 );
     editor.Paste();

     ASSERT_EQUAL( editor.GetText(), "" );
}

void TestEmptyBuffer()
{
     Editor editor;

     editor.Paste();
     TypeText( editor, "example" );
     editor.Left();
     editor.Left();
     editor.Paste();
     editor.Right();
     editor.Paste();
     editor.Copy( 0 );
     editor.Paste();
     editor.Left();
     editor.Cut( 0 );
     editor.Paste();

     ASSERT_EQUAL( editor.GetText(), "example" );
}

void TestCopyBoard()
{
     {
          Editor editor;
          TypeText(editor, "happy birthday");
          editor.Left();
          editor.Left();
          editor.Copy(5);
          editor.Paste();
          ASSERT_EQUAL(editor.GetText(), "happy birthdayay");
     }
     {
          Editor editor;
          TypeText(editor, "happy birthday");
          editor.Left();
          editor.Left();
          editor.Copy(5);
          editor.Right();
          editor.Right();
          editor.Paste();
          ASSERT_EQUAL(editor.GetText(), "happy birthdayay");
     }
}

void TestCutBoard()
{
     {
          Editor editor;
          TypeText(editor, "happy birthday");
          editor.Left();
          editor.Left();
          editor.Cut(5);
          ASSERT_EQUAL(editor.GetText(), "happy birthd");
          editor.Paste();
          ASSERT_EQUAL(editor.GetText(), "happy birthday");
     }
     {
          Editor editor;
          TypeText(editor, "happy birthday");
          editor.Cut(5);
          editor.Right();
          editor.Right();
          editor.Paste();
          ASSERT_EQUAL(editor.GetText(), "happy birthday");
     }
}

void TestCPP()
{
     {
          Editor editor;
          TypeText(editor, "happy birthday");
          editor.Left();
          editor.Left();
          editor.Cut(5);
          ASSERT_EQUAL(editor.GetText(), "happy birthd");
          editor.Paste();
          ASSERT_EQUAL(editor.GetText(), "happy birthday");
          editor.Paste();
          ASSERT_EQUAL(editor.GetText(), "happy birthdayay");
     }
}

int main()
{
     TestRunner tr;
     RUN_TEST( tr, TestEditing );
     RUN_TEST( tr, TestReverse );
     RUN_TEST( tr, TestNoText );
     RUN_TEST( tr, TestEmptyBuffer );
     RUN_TEST( tr, TestCopyBoard );
     RUN_TEST( tr, TestCutBoard );
     RUN_TEST( tr, TestCPP );

     return 0;
}