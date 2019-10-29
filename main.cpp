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

//cell
//generation
//universe

class mCell
{
private:
    int value;
public:
    static const char AliveSymbol = '*';
    static const char DeadSymbol = '_';

    explicit mCell() : value(0) {}
    mCell(int val) : value(val) {}

    friend mCell& operator++ (mCell& i, int);
    int getvalue() const;
    char GetSymbol();

};

typedef std::unordered_map<int, mCell> mapCell;

class mGeneration
{
private:
    mapCell space;
    int n=0, m=0, msize;

    int convert_double_index_to_single(int a, int b);
    void parseString(string , int );
public:
    explicit mGeneration(string FileName);
    void printSpace();

};

class mUniverse
{
private:

public:

};







mCell& operator++ (mCell& item, int)
{
    item.value++;
    return item;
}

int mCell::getvalue() const
{
    return value;
}

char mCell::GetSymbol()
{
    if ((value == 2) || (value == 3))
    {
        return AliveSymbol;
    }
    else
    {
        return DeadSymbol;
    }
}



void mGeneration::printSpace()
{

    for (std::pair<int, mCell> element : space)
    {
        cout << " index = " << element.first << " value = " << element.second.getvalue() << endl;
    }
}


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
                istringstream sstr(str);
                istream_iterator<string> it(sstr);
                n = stoi(*it);
                m = stoi(*(++it));
                msize = m*n;
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

    for (int i=0; i < static_cast<int>(item.size()); i++)
    {
        if (item[i] == mCell::AliveSymbol)
        {
            cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " alive" << endl;
            int index = convert_double_index_to_single(line_number, i);
            space.emplace (pair<int, mCell>(index, mCell() ) );
        }
        if (item[i] == mCell::DeadSymbol)
        {
            cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " dead" << endl;
        }
    }
}







int main() {

    mGeneration mg("world.txt");
    mg.printSpace();

    return 0;
}