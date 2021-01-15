#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <map>

using namespace std;

class ReadingManager
{
public:
    ReadingManager()
            : users_()
            , pages_()
    {}

    void Read(int user_id, int page_count)
    {
        if( users_.count( user_id ) == 1 )
        {
            int lastPage = users_[ user_id ];
            pages_[ lastPage ].userCount--;
            if( pages_[ lastPage ].userCount == 0 )
            {
                 pages_.erase( lastPage );
            }
        }

        users_[ user_id ] = page_count;
        pages_[ page_count ].userCount++;
        PagesRateUpdate();
    }

    double Cheer(int user_id) const
    {
        auto it = users_.find( user_id );
        if( it == users_.end() )
        {
            return 0;
        }
        return pages_.at( users_.at( user_id ) ).rate;
    }

private:
    struct PageRate
    {
        size_t userCount;
        double rate;
    };

    std::map< int, int > users_;
    std::map< int, PageRate > pages_;

    void PagesRateUpdate()
    {
        size_t usersSize = users_.size();

        if( usersSize == 1 )
        {
            pages_.begin()->second.rate = 1;
            return;
        }

        size_t count = 0;
        for( auto& page: pages_ )
        {
            page.second.rate = count * 1.0 / ( usersSize - 1 );
            count += page.second.userCount;
        }
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}