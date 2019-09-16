#include <iostream>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
#include <locale>
#include <regex>
#include <map>

using namespace std;
typedef std::map<std::string, int>  mapT;

typedef struct{
    mapT indexes;
    int count;
}mymap;

void build_mymap(vector<string> items, mymap &result_map) {

    vector<string>::iterator it;
    mapT::iterator itr;
    for (it = items.begin(); it != items.end(); ++it) {
      //  cout << *it << " ";
        itr = result_map.indexes.find(*it);
        if (itr == result_map.indexes.end()) {
            result_map.indexes.insert(pair<string, int>(*it, 1));
        } else{
            itr->second++;
        }
        result_map.count++;
    }
   // cout << endl;

/*
    for (itr = result_map.indexes.begin(); itr != result_map.indexes.end(); itr++)
    {
        std::cout << itr->first << '=' << itr->second << endl;
    }
    cout << endl;
*/

}

vector<string> split_str(string item_string)
{

    vector<string> ret;
    regex reg("[a-z0-9]+",std::regex_constants::icase);

    smatch words;
    while (regex_search(item_string, words, reg))
    {
       // cout << words.str() << "\n";
        ret.push_back(words.str());
        item_string = words.suffix();
    }

return ret;

}

void read_file_by_line_into_mymap(std::string filename, mymap &result_map )
{
    string str = "";
    std::ifstream in(filename);

    if (in.is_open())
    {
        while (getline(in, str))
        {
            //  std::cout << str << std::endl;
            build_mymap(split_str(str),result_map);
        }
    }
    in.close();     // закрываем файл

}


void write_mymap_to_csv_file(string filename, mymap &result_map)
{
    multimap<int, string> reverseMyMap;
    for (pair<string, int> pair : result_map.indexes) {
            reverseMyMap.insert(std::pair<int, string>(pair.second, pair.first) );
         }

    std::ofstream out(filename);
   // cout << "Reverse:\n";
    multimap<int, string>::reverse_iterator it = reverseMyMap.rbegin();
    while (it != reverseMyMap.rend()) {
        if (out.is_open())
        {
            out << it->second << ", " << it->first << ", " << (static_cast<float>(it->first)/result_map.count)*100.0f << "%" << endl;
        }
       // cout << it->first << ": " << it->second << '\n';
        it++;
    }
}



int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "SOURCE DESTINATION" << std::endl;
        return 1;
    }

    mymap indexs;
    indexs.count = 0;

    //string inputfile = "test1.txt", outputfile = "output1.csv";
    string inputfile = argv[1], outputfile = argv[2];


    read_file_by_line_into_mymap(inputfile, indexs);
    write_mymap_to_csv_file(outputfile, indexs);

    return 0;
}
