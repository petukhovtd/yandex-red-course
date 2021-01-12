#pragma once

#include "http_request.h"

#include <string_view>
#include <map>

class Stats
{
public:
     using StatsMap = std::map< string_view, int >;

     Stats();

     void AddMethod( string_view method );

     void AddUri( string_view uri );

     const StatsMap& GetMethodStats() const;

     const StatsMap& GetUriStats() const;

private:
     StatsMap metodStats_;
     StatsMap uriStats_;
     StatsMap::iterator unknownMetod_;
     StatsMap::iterator unknownUri_;
};

HttpRequest ParseRequest( string_view line );