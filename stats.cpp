#include "stats.h"


Stats::Stats()
          : metodStats_( { { "GET",     0 },
                           { "PUT",     0 },
                           { "POST",    0 },
                           { "DELETE",  0 },
                           { "UNKNOWN", 0 } } )
          , uriStats_( { { "/",        0 },
                         { "/order",   0 },
                         { "/product", 0 },
                         { "/basket",  0 },
                         { "/help",    0 },
                         { "unknown",  0 } } )
{
     unknownMetod_ = metodStats_.find( "UNKNOWN" );
     unknownUri_ = uriStats_.find( "unknown" );
}

void Stats::AddMethod( string_view method )
{
     auto it = metodStats_.find( method );
     if( it == metodStats_.end() )
     {
          unknownMetod_->second++;
          return;
     }
     it->second++;
}

void Stats::AddUri( string_view uri )
{
     auto it = uriStats_.find( uri );
     if( it == uriStats_.end() )
     {
          unknownUri_->second++;
          return;
     }
     it->second++;
}

const Stats::StatsMap& Stats::GetMethodStats() const
{
     return metodStats_;
}

const Stats::StatsMap& Stats::GetUriStats() const
{
     return uriStats_;
}

HttpRequest ParseRequest( string_view line )
{
     HttpRequest httpRequest;

     auto pos = line.find_first_not_of( ' ' );
     line.remove_prefix( pos );

     pos = line.find(' ' );
     httpRequest.method = line.substr( 0, pos );
     line.remove_prefix( pos + 1 );

     pos = line.find(' ' );
     httpRequest.uri = line.substr( 0, pos );
     line.remove_prefix( pos + 1 );

     httpRequest.protocol = line;

     return httpRequest;
}
