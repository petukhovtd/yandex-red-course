#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>

vector< string > SplitIntoWords( string const& line )
{
     istringstream words_input( line );
     return { istream_iterator< string >( words_input ), istream_iterator< string >() };
}

SearchServer::SearchServer( istream& document_input )
{
     UpdateDocumentBase( document_input );
}

void SearchServer::UpdateDocumentBase( istream& document_input )
{
     InvertedIndex new_index;

     for( string current_document; getline( document_input, current_document ); )
     {
          new_index.Add( move( current_document ) );
     }

     index = move( new_index );
}

void SearchServer::AddQueriesStream( istream& query_input, ostream& search_results_output )
{
     std::vector< std::size_t > docid_count;
     std::vector< int64_t > docid;
     for( string current_query; getline( query_input, current_query ); )
     {
          const std::size_t docSize = index.Size();
          docid_count.resize( docSize );
          docid_count.assign( docSize, 0 );
          docid.resize( docSize );
          std::iota( docid.begin(), docid.end(), 0 );

          const auto words = SplitIntoWords( current_query );

          for( const auto& word : words )
          {
               for( InvertedIndex::DocIdCount const& docIdCount: index.Lookup( word ) )
               {
                    docid_count[ docIdCount.first ] += docIdCount.second;
               }
          }

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
          search_results_output << endl;
     }
}

void InvertedIndex::Add( string document )
{
     const size_t docid = docs.size();
     for( const auto& word : SplitIntoWords( document ) )
     {
          auto& docCount = index[ word ];

          auto it = std::find_if( docCount.begin(), docCount.end(), [ &docid ]( DocIdCount const& p )
          {
               return docid == p.first;
          });
          if( it == docCount.end() )
          {
               docCount.emplace_back( docid, 1 );
               continue;
          }
          it->second++;
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
