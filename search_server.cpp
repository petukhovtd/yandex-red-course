#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>
#include <future>

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

vector< string > SplitIntoWords( string const& line )
{
     istringstream words_input( line );
     return { make_move_iterator( istream_iterator< string >( words_input ) ),
              make_move_iterator( istream_iterator< string >() ) };
}

SearchServer::SearchServer( istream& document_input )
{
     UpdateDocumentBase( document_input );
}

void SingleThreadUpdateDocumentBase( istream& document_input, Synchronized< InvertedIndex >& index )
{
     InvertedIndex new_index;

     for( string current_document; getline( document_input, current_document ); )
     {
          new_index.Add( move( current_document ) );
     }

     index.GetAccess().ref_to_value = move( new_index );
}

void SearchServer::UpdateDocumentBase( istream& document_input )
{
     if( index.GetAccess().ref_to_value.Size() != 0 )
     {
          futures_.push_back( std::async( SingleThreadUpdateDocumentBase, std::ref( document_input ), std::ref( index )));
     }
     else
     {
          SingleThreadUpdateDocumentBase( document_input, index );
     }
}

void SingleThreadQueries( istream& query_input, ostream& search_results_output, Synchronized< InvertedIndex >& index )
{
     std::vector< std::size_t > docid_count;
     std::vector< int64_t > docid;
     for( string current_query; getline( query_input, current_query ); )
     {
          const auto words = SplitIntoWords( current_query );
          {
               const auto access = index.GetAccess();
               const auto& indexSync = access.ref_to_value;
               const std::size_t docSize = indexSync.Size();
               docid_count.assign( docSize, 0 );
               docid.resize( docSize );

               for( const auto& word : words )
               {
                    for( InvertedIndex::DocIdCount const& docIdCount: indexSync.Lookup( word ) )
                    {
                         docid_count[ docIdCount.first ] += docIdCount.second;
                    }
               }
          }

          std::iota( docid.begin(), docid.end(), 0 );

          std::partial_sort( docid.begin(), Head( docid, 5 ).end(), docid.end(),
                             [ &docid_count ]( int64_t lhs, int64_t rhs )
                             {
                                  return make_pair( docid_count[ lhs ], -lhs ) > make_pair( docid_count[ rhs ], -rhs );
                             } );

          search_results_output << current_query << ':';
          for( size_t id : Head( docid, 5 ) )
          {
               const std::size_t hitcount = docid_count[ id ];
               if( hitcount == 0 )
               {
                    break;
               }
               search_results_output << " {"
                                     << "docid: " << id << ", "
                                     << "hitcount: " << hitcount << '}';
          }
          search_results_output << '\n';
     }

}

void SearchServer::AddQueriesStream( istream& query_input, ostream& search_results_output )
{
     futures_.push_back( std::async( SingleThreadQueries, std::ref( query_input ), std::ref( search_results_output ),
                                     std::ref( index ) ) );
}

void InvertedIndex::Add( string document )
{
     const size_t docid = docs.size();
     for( const auto& word : SplitIntoWords( document ) )
     {
          auto& docCount = index[ word ];

          if( !docCount.empty() && docCount.back().first == docid )
          {
               ++docCount.back().second;
          }
          else
          {
               docCount.emplace_back( docid, 1 );
          }
     }

     docs.push_back( std::move( document ));
}

InvertedIndex::DocCounter const& InvertedIndex::Lookup( const string& word ) const
{
     if( auto it = index.find( word ); it != index.end() )
     {
          return it->second;
     }

     static const DocCounter empty;
     return empty;
}
