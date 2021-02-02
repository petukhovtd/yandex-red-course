#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <future>
#include "synchronized.h"

using namespace std;

class InvertedIndex
{
public:
     using DocIdCount = std::pair< size_t, std::size_t >;
     using DocCounter = std::vector< DocIdCount >;

     void Add( string document );

     DocCounter const& Lookup( const string& word ) const;

     const string& GetDocument( size_t id ) const
     {
          return docs[ id ];
     }

     size_t Size() const
     {
          return docs.size();
     }

private:
     map< string, DocCounter > index;
     vector< string > docs;
};

class SearchServer
{
public:
     SearchServer() = default;

     explicit SearchServer( istream& document_input );

     void UpdateDocumentBase( istream& document_input );

     void AddQueriesStream( istream& query_input, ostream& search_results_output );

private:
     Synchronized< InvertedIndex > index;
     std::vector< std::future< void >> futures_;
};
