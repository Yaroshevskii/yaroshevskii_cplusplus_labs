#include <iostream>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
#include <regex>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>


using namespace std;

#define AliveSymbol ('*')
#define DeadSymbol ('_')

//cell
//generation
//universe

class mCell
{
    int value;

public:
    explicit mCell()
    {
        value = 0;
    }


    mCell& operator++ (int)
    {
        value++;
        return *this;
    }

    int getvalue() const
    {
        return value;
    }

};





class mGeneration
{
    unordered_map<int, mCell> space;
    int n=0, m=0;

    int convert_double_index_to_single(int a, int b);

    void parseString(string item, int string_number);
public:
    explicit mGeneration(string FileName);
};


mGeneration::mGeneration(string FileName)
{
    ifstream in(FileName);
    string str = "";
    int string_num = -1;
    if (in.is_open())
    {
        while (getline(in, str))
        {
            if (string_num == -1)
            {
                //string[] words = str.Split(new char[] { ' ' });
                istringstream sstr(str);
                istream_iterator<string> it(sstr);
                n = stoi(*it);
                m = stoi(*(++it));
                cout << " n = " << n << " m = " << m << endl;
            }else
            {
                parseString(str, string_num);
            }
            string_num++;
            cout << str << endl;
        }
        in.close();
    }
}

int mGeneration::convert_double_index_to_single(int a, int b)
{

    return a*m + b;
}

void mGeneration::parseString(string item, int line_number)
{
    for (unsigned int i=0; i < item.size(); i++)
    {
        if (item[i] == AliveSymbol)
        {
            cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " alive" << endl;
            //   int index = convert_double_index_to_single(string_number, i);
        }
        if (item[i] == DeadSymbol)
        {
            cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " dead" << endl;
        }
    }
}







int main() {

    mGeneration mg("world.txt");

    return 0;
}