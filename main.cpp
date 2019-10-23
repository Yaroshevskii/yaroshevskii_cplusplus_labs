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
    mCell()
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

    int convert_double_index_to_single(int a, int b)
    {
        return a*m + b;
    }

    void parseString(string item, int string_number)
    {
        for (unsigned int i=0; i < item.size(); i++)
        {
            if (item[i] == AliveSymbol)
            {
                cout << " item = " << item[i] << " line = " << string_number << " row = " << i << " alive" << endl;
             //   int index = convert_double_index_to_single(string_number, i);
            }
            if (item[i] == DeadSymbol)
            {
                cout << " item = " << item[i] << " line = " << string_number << " row = " << i << " dead" << endl;
            }
        }
    }
public:
    mGeneration(string FileName)
    {
        ifstream in(FileName);
        string str = "";
        int string_num = 0;
        if (in.is_open())
        {
            while (getline(in, str))
            {
                if (string_num == -1)
                {
                    //string[] words = str.Split(new char[] { ' ' });
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
};

int main() {

    mCell mc;

    mc++;
    mc++;
    cout << " i = " << mc.getvalue() << endl;

    mGeneration mg("world.txt");

    return 0;
}