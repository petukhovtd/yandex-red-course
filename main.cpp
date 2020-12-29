#include <iostream>

vector<string> all_strings = ReadStrings();
vector<string> strings_to_find = ReadStrings();
auto result = search(begin(all_strings), end(all_strings),
                     begin(strings_to_find), end(strings_to_find));

                     // NML


vector<string> strings = ReadStrings();
set<string> chosen_strings;
remove_copy_if(begin(strings), end(strings), inserter(chosen_strings),
[](const string& s) { return s[0] == 'C'; });

// ! N
// ! N LogL
// N logN


vector<string> strings = ReadStrings();
string joined_string =
          accumulate(begin(strings), end(strings), string());

// ! NL
// ! N L^2
// N^2 L


int main( int, char** )
{
     std::cout << __cplusplus << std::endl;
     return 0;
}
