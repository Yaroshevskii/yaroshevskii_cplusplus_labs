#include <iostream>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
#include <locale>
#include <regex>
#include <map>

using namespace std;
typedef std::map<std::string,int>  mapT;




void build_map(vector<string> items, mapT &result_map) {

    vector<string>::iterator it;
    mapT::iterator itr;
    for (it = items.begin(); it != items.end(); ++it) {
        cout << *it << " ";

        if (result_map.find(*it) == result_map.end()) {
            result_map.insert(pair<string, int>(*it, 1));
        } else{
            result_map[*it]++;
        }
    }
    cout << endl;


    for (itr = result_map.begin(); itr != result_map.end(); itr++)
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



/*
    vector<string>::iterator it;
    for(it = ret.begin(); it != ret.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
*/
    /*
    const string reg = "\\W|_+";
    std::regex rgx(reg, std::regex_constants::collate | std::regex_constants::icase);
    std::sregex_token_iterator iter(item_string.begin(),
                                    item_string.end(),
                                    rgx,
                                    -1);
    std::sregex_token_iterator end;
    for ( ; iter != end; ++iter)
        if (*iter != "") {
            std::cout << *iter << '\n';
        }
    */

return ret;

}

void read_file_by_line(std::string filename, mapT &result_map )
{
    /*
    std::vector<byte> newVector{};
    std::ifstream ifs(filename,std::ios::in | std::ifstream::binary);
    std::istreambuf_iterator<byte> iter(ifs);
    std::istreambuf_iterator<byte> end{};
    std::copy(iter,end,std::back_inserter(newVector));
    ifs.close();

    string ret(newVector.begin(), newVector.end());
    cout << ret << endl;
    split_str(ret);
*/

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

int main() {
    mapT indexs;
    //std::locale::global(std::locale(""));


    //string str = "ели булки три два раз";
//    string str = "";

/*
    std::ifstream in("test1.txt");

    if (in.is_open())
    {
        while (getline(in, str))
        {
          //  std::cout << str << std::endl;
            split_str(str);
        }
    }
    in.close();     // закрываем файл
    */


    read_file_by_line("test1.txt", indexs);

    
    return 0;
}
