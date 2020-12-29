#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

bool operator==( Date const& lhs, Date const& rhs )
{
     return tie( lhs.year, lhs.month, lhs.day ) == tie( rhs.year, rhs.month, rhs.day );
}

std::ostream& operator<<( std::ostream& os, Date const& date )
{
     os << date.year << '-' << date.month << '-' << date.day;
     return os;
}

bool operator==( Time const& lhs, Time const& rhs )
{
     return tie( lhs.hours, lhs.minutes ) == tie( rhs.hours, rhs.minutes );
}

std::ostream& operator<<( std::ostream& os, Time const& time )
{
     os << time.hours << ':' << time.minutes;
     return os;
}

std::istream& operator>>( std::istream& is, Date& date )
{
     int year = 0;
     int month = 0;
     int day = 0;
     std::string dateStr;

     is >> dateStr;
     std::stringstream ss( dateStr );

     ss >> year;
     if( !ss || ss.peek() != '-' )
     {
          throw std::invalid_argument( "Wrong date format: " + dateStr );
     }
     ss.ignore();
     ss >> month;
     if( !ss || ss.peek() != '-' )
     {
          throw std::invalid_argument( "Wrong date format: " + dateStr );
     }
     ss.ignore();
     ss >> day;
     if( !ss )
     {
          throw std::invalid_argument( "Wrong date format: " + dateStr );
     }

     if( month > 12 || month < 1 )
     {
          throw std::invalid_argument( "Month value is invalid: " + std::to_string( month ) );
     }

     if( day > 31 || day < 1 )
     {
          throw std::invalid_argument( "Day value is invalid: " + std::to_string( day ) );
     }

     date.year = year;
     date.month = month;
     date.day = day;
     return is;
}

std::istream& operator>>( std::istream& is, Time& time )
{
     int hours = 0;
     int minutes = 0;
     std::string dateStr;

     is >> dateStr;
     std::stringstream ss( dateStr );

     ss >> hours;
     if( !ss || ss.peek() != ':' )
     {
          throw std::invalid_argument( "Wrong time format: " + dateStr );
     }
     ss.ignore();
     ss >> minutes;
     if( !ss )
     {
          throw std::invalid_argument( "Wrong time format: " + dateStr );
     }

     if( hours > 23 || hours < 0 )
     {
          throw std::invalid_argument( "Hours value is invalid: " + std::to_string( hours ) );
     }

     if( minutes > 59 || minutes < 0 )
     {
          throw std::invalid_argument( "Minutes value is invalid: " + std::to_string( minutes ) );
     }

     time.hours = hours;
     time.minutes = minutes;
     return is;
}

#define UPDATE_FIELD(ticket, field, values) { \
auto it = values.find( #field ); \
if (it != values.end()) \
{ \
    istringstream is(it->second); \
    is >> ticket.field; \
} \
}

void TestUpdate() {
     AirlineTicket t;
     t.price = 0;

     const map<string, string> updates1 = {
               {"departure_date", "2018-2-28"},
               {"departure_time", "17:40"},
     };
     UPDATE_FIELD(t, departure_date, updates1);
     UPDATE_FIELD(t, departure_time, updates1);
     UPDATE_FIELD(t, price, updates1);

     ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
     ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
     ASSERT_EQUAL(t.price, 0);

     const map<string, string> updates2 = {
               {"price", "12550"},
               {"arrival_time", "20:33"},
     };
     UPDATE_FIELD(t, departure_date, updates2);
     UPDATE_FIELD(t, departure_time, updates2);
     UPDATE_FIELD(t, arrival_time, updates2);
     UPDATE_FIELD(t, price, updates2);

     ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
     ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
     ASSERT_EQUAL(t.price, 12550);
     ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
     TestRunner tr;
     RUN_TEST(tr, TestUpdate);
}