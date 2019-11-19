//
// Created by Andy on 19.11.2019.
//

#ifndef TASK0_MUNIVERSE_H
#define TASK0_MUNIVERSE_H

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
#include "mLife.h"


class mUniverse
{
private:
    mGeneration gen;
    int CountOfIteration;
public:
    mUniverse(string filename, int iterations) : gen(filename), CountOfIteration(iterations) {}
    void Go();
    void Start();
    void Start(int);
};



#endif //TASK0_MUNIVERSE_H
