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
    bool isNextGen;
public:
    static const char AliveSymbol = '*';
    static const char DeadSymbol = '_';

    explicit mCell() : value(0), isNextGen(false) {}
    mCell(int val, bool NextGen) : value(val), isNextGen(NextGen) {}

    friend mCell& operator++ (mCell& i, int);
    int getvalue() const;
    bool GetIsNexGen();
    void SetValue(int val, bool isGen);


};

typedef std::unordered_map<int, mCell> mapCell;

class mGeneration
{
private:
    int n = 0, m = 0, msize = 0;
    mapCell space;

    int convert_double_index_to_single(int a, int b);
    pair<int, int> convert_single_index_to_double(int);
    void parseString(string , int );
public:
    explicit mGeneration(string FileName);
    mGeneration(const mGeneration &item): n(item.n), m(item.m), msize(item.msize), space(item.space) {}
    void printSpace(int);
    void NextGen();
    int CountOfElements();
    bool isAliveExist();
    vector<int> neighbours(int);
};

class mUniverse
{
private:
    mGeneration gen, prev_gen;
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

void mCell::SetValue(int val, bool isGen)
{
    value = val;
    isNextGen = isGen;
}

bool mCell::GetIsNexGen()
{
    return isNextGen;
}

int mGeneration::CountOfElements()
{
    return space.size();
}

bool mGeneration::isAliveExist()
{
    return !space.empty();
}


vector<int> mGeneration::neighbours(int item)
{
    vector<int> ret;
    pair<int, int> indexs = convert_single_index_to_double(item);
    //cout << "neighbours el = " << item << " i = " << indexs.first << " j = " << indexs.second << endl;

    vector<int> steps_inline = {1, -1};
    vector<int> steps_downline = {m-1, m, m+1};
    vector<int> steps_upline = {-m-1, -m, -m+1};

    int element;

    for (int i=0; i<static_cast<int>(steps_inline.size()); i++)
    {
        element = item + steps_inline[i];
        if ( (element >= 0) & (element < msize) )
        {
            ret.push_back(element);
        }
    }

    for (int i=0; i<static_cast<int>(steps_upline.size()); i++)
    {
        element = item + steps_upline[i];
        if ( (element >= 0) & (element < msize) & ( element/m == (indexs.first - 1) ))
        {
            ret.push_back(element);
        }
    }

    for (int i=0; i<static_cast<int>(steps_downline.size()); i++)
    {
        element = item + steps_downline[i];
        if ( (element >= 0) & (element < msize) & (element/m == (indexs.first + 1) ))
        {
            ret.push_back(element);
        }
    }


    return ret;
}

void mGeneration::NextGen() {

    mapCell potentialCell;
    mapCell::iterator itr;
    vector<int> item;
    const int countCellForAlive = 3;



        for (auto it = space.begin(); it != space.end(); ++it) {
            //cout << " potentialCell element = " << it->first << " neighbours = ";
            item = neighbours(it->first);
            for (auto i: item) {
                itr = potentialCell.find(i);
                if (itr == potentialCell.end()) {
                    potentialCell.emplace(pair<int, mCell>(i, mCell(1, true)));
                } else {
                    potentialCell[i]++;
                }
                //cout << i << ' ';
            }
            //cout << endl;
        }

        for (auto it = potentialCell.begin(); it != potentialCell.end();) {
            if (it->second.getvalue() != countCellForAlive) {
                it = potentialCell.erase(it);
            } else {
                //cout << " potentialCell index = " << it->first << " value = " << it->second.getvalue() << endl;
                space.emplace(pair<int, mCell>(it->first, mCell(3, true)));
                ++it;
            }
        }


    for (auto it = space.begin(); it != space.end(); ++it) {
        //cout << " space element = " << it->first << " neighbours = ";
        item = neighbours(it->first);
        if (!it->second.GetIsNexGen())
        {
            for (auto i: item)
            {
                itr = space.find(i);
                if (itr != space.end())
                {
                    if (!space[i].GetIsNexGen())
                    {
                        it->second++;
                    }

                }
                //cout << i << ' ';
            }
            //cout << " value = " << it->second.getvalue() << " isNextGen = " << it->second.GetIsNexGen() << endl;
        }
    }

    for (auto it = space.begin(); it != space.end();)
    {
        if ((it->second.getvalue() < 2) || (it->second.getvalue() > 3) )
        {
            it = space.erase(it);
        } else
        {
            space[it->first].SetValue(0, false);
            ++it;
        }
    }

}


void mGeneration::printSpace(int gen)
{


   /* for (std::pair<int, mCell> element : space)
    {
        cout << " index = " << element.first << " value = " << element.second.getvalue() << endl;
    }*/


    mapCell::iterator itr;
    cout << "=========="  << gen << "==========" << endl;
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            int index = convert_double_index_to_single(i,j);
            itr = space.find(index);

            if (itr == space.end())
            {
                cout << mCell::DeadSymbol;
            } else
            {
                cout << mCell::AliveSymbol;
            }
        }
        cout << endl;
    }
    cout << "=====================" << endl;
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
                //cout << " n = " << n << " m = " << m << endl;
            }else
            {
                parseString(str, string_num);
            }
            string_num++;
            //cout << str << endl;
        }
        in.close();
    }
}

int mGeneration::convert_double_index_to_single(int a, int b)
{
    return a*m + b;
}

pair<int, int>  mGeneration::convert_single_index_to_double(int index)
{
    pair<int, int>  ret;
    ret.first =  index/m;
    ret.second = index - ret.first*m;

    return ret;
}


void mGeneration::parseString(string item, int line_number)
{

    for (int i=0; i < static_cast<int>(item.size()); i++)
    {
        if (item[i] == mCell::AliveSymbol)
        {
            //cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " alive" << endl;
            int index = convert_double_index_to_single(line_number, i);
            space.emplace (pair<int, mCell>(index, mCell() ) );
        }
        if (item[i] == mCell::DeadSymbol)
        {
            //cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " dead" << endl;
        }
    }
}





int main() {

    mGeneration mg("world.txt");
    for (int i=0; i<10; i++)
    {
        mg.printSpace(i);
        mg.NextGen();
        if (mg.isAliveExist() == false)
        {
            break;
        }
    }


    return 0;
}