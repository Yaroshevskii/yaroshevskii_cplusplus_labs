//
// Created by Andy on 19.11.2019.
//

#ifndef TASK0_MLIFE_H
#define TASK0_MLIFE_H

#include <iostream>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
#include <regex>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include <cstdlib>


//cell
//generation
//universe
using namespace std;

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
    void SetValue(int val);


};

typedef std::unordered_map<int, mCell> mapCell;


class IGeneration
{
public:
    virtual void NextGen() = 0;
    virtual bool isAliveExist() = 0;
    virtual void printSpace() = 0;
    virtual ~IGeneration(){}
};



class mGeneration : public IGeneration
{
private:
    int n = 0, m = 0, msize = 0;
    mapCell space;

    int convert_double_index_to_single(int a, int b);
    pair<int, int> convert_single_index_to_double(int);
    void parseString(string , int );
    vector<int> neighbours(int);
public:
    explicit mGeneration(string FileName);
    mGeneration(const mGeneration &item): n(item.n), m(item.m), msize(item.msize), space(item.space) {}
    void printSpace();
    void NextGen();
    bool isAliveExist();
    ~mGeneration(){};
};





#endif //TASK0_MLIFE_H
