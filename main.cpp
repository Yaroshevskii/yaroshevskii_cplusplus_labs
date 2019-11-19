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
#include "mUniverse.h"


using namespace std;



int main(int argc, char* argv[])
{

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "SOURCE DESTINATION" << std::endl;
        return 1;
    }

    string inputfile = argv[1];
    int iterations = atoi(argv[2]);
    //cout << " filename = " << inputfile << " iteration = " << iterations << endl;




    mUniverse life(inputfile, iterations);
    life.Go();


    return 0;
}