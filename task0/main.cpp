#include <iostream>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
#include <locale>
#include <regex>
#include <map>

using namespace std;
typedef std::map<std::string,int>  mapT;

typedef struct{
    mapT indexes;
    int count;
}mymap;

void build_map(vector<string> items, mymap &result_map) {

    vector<string>::iterator it;
    mapT::iterator itr;
    for (it = items.begin(); it != items.end(); ++it) {
      //  cout << *it << " ";

        if (result_map.indexes.find(*it) == result_map.indexes.end()) {
            result_map.indexes.insert(pair<string, int>(*it, 1));
        } else{
            result_map.indexes[*it]++;
        }
        result_map.count++;
    }
   // cout << endl;


    for (itr = result_map.indexes.begin(); itr != result_map.indexes.end(); itr++)
    {
        std::cout << itr->first << '=' << itr->second << endl;
    }
    cout << endl;

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

void read_file_by_line_into_map(std::string filename, mymap &result_map )
{
    string str = "";
    std::ifstream in(filename);

    if (in.is_open())
    {
        while (getline(in, str))
        {
            //  std::cout << str << std::endl;
            build_map(split_str(str),result_map);
        }
    }
    in.close();     // закрываем файл

}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "SOURCE DESTINATION" << std::endl;
        return 1;
    }

    mymap indexs;
    indexs.count = 0;
    //string inputfile = "test1.txt", outputfile = "output1.txt";
    string inputfile = argv[1], outputfile = argv[1];

    read_file_by_line_into_map(inputfile, indexs);

    cout << endl << "count = " <<   indexs.count << endl;

    return 0;
}
