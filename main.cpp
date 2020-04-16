#include <iostream>
#include <gtest/gtest.h>
#include <math.h>
#include "csvParser.h"
#include <tuple>
#include <iterator>
#include <algorithm>
#include <utility>
#include <type_traits>
#include <typeinfo>

using namespace std;


#define TupleStruct int, string

TEST(CSVParser, TestForThrow) {
    csvParser<TupleStruct> *parser;
    std::ifstream file("../test.csv");
    parser = new csvParser<TupleStruct> (file, 0);
    ASSERT_ANY_THROW({
                         for (std::tuple<TupleStruct> rs : *parser) {
                         }
                     });
}

TEST(CSVParser, TestNoThrow) {
    csvParser<TupleStruct> *parser;
    std::ifstream file("../test.csv");
    parser = new csvParser<TupleStruct> (file, 0);
    ASSERT_NO_THROW({
                        for (std::tuple<TupleStruct> rs : *parser) {
                        }
                    });
}

int main(int argc, char** argv) {


    ifstream file("../test.csv", ios::in);
    if (!file) return 1;

    csvParser<TupleStruct> parser{file, 0};

    for(tuple<TupleStruct> rs: parser)
    {
        cout << rs << endl;
    }


/*
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
*/

    return 0;
}
