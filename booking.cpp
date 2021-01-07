#include <iostream>
#include <deque>
#include <map>

class Booking
{
public:
     using Time = signed long long int;

private:
     struct ClientBooking
     {
          Time time;
          int roomCount;
     };

     using ClientsQueue = std::deque< ClientBooking >;

     struct Hotel
     {
          int bookedRooms;
          std::map< int, ClientsQueue > clients;
     };

     static const Time day = 86400;

public:
     Booking()
     : currentTime_( 0 )
     {}

     void Book( Time time, std::string const& hotelName, int clientId, int roomCount )
     {
          currentTime_ = time;
          Hotel& hotel = hotels_[ hotelName ];
          hotel.clients[ clientId ].push_back( { time, roomCount } );
          hotel.bookedRooms += roomCount;
          RemoveOldBooking();
     }

     int Client( std::string const& hotelName ) const
     {
          auto it = hotels_.find( hotelName );
          if( it == hotels_.end() )
          {
               return 0;
          }
          return it->second.clients.size();
     }

     int Rooms( std::string const& hotelName ) const
     {
          auto it = hotels_.find( hotelName );
          if( it == hotels_.end() )
          {
               return 0;
          }
          return it->second.bookedRooms;
     }

private:
     void RemoveOldBooking()
     {
          for( auto& hotelPair: hotels_ )
          {
               for( auto& clientPair: hotelPair.second.clients )
               {
                    while( !clientPair.second.empty() )
                    {
                         if( clientPair.second.front().time <= ( currentTime_ - day ) )
                         {
                              hotelPair.second.bookedRooms -= clientPair.second.front().roomCount;
                              clientPair.second.pop_front();
                              continue;
                         }
                         break;
                    }
               }
          }
     }

private:
     Time currentTime_;
     std::map< std::string, Hotel > hotels_;
};

int main()
{
     std::ios::sync_with_stdio(false);
     std::cin.tie(nullptr);

     Booking booking;

     int query_count;
     std::cin >> query_count;

     for (int query_id = 0; query_id < query_count; ++query_id) {
          std::string query_type;
          std::cin >> query_type;
          if (query_type == "BOOK")
          {
               Booking::Time time;
               std::string hotelName;
               int clientId;
               int roomCount;
               std::cin >> time >> hotelName >> clientId >> roomCount;
               booking.Book( time, hotelName, clientId, roomCount );
          }
          else if (query_type == "CLIENTS")
          {
               std::string hotelName;
               std::cin >> hotelName;
               std::cout << booking.Client( hotelName ) << '\n';
          }
          else if (query_type == "ROOMS")
          {
               std::string hotelName;
               std::cin >> hotelName;
               std::cout << booking.Rooms( hotelName ) << '\n';
          }
     }

     return 0;
}