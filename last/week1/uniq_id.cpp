#include <string>
#include <vector>
using namespace std;

#define UNIQ_ID_IMPL(first,last) first##last
#define UNIQ_ID_IMPL0(x,y) UNIQ_ID_IMPL(x,y)
#define UNIQ_ID UNIQ_ID_IMPL0(name,__LINE__)

int main() {
     int UNIQ_ID = 0;
     string UNIQ_ID = "hello";
     vector<string> UNIQ_ID = {"hello", "world"};
     vector<int> UNIQ_ID = {1, 2, 3, 4};
}