#include <cstdio>
#include <iostream>
#include "Huffman.h"



int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "Input error (must be 4 args)"
                  << std::endl;
        return 1;
    }
    const char *param = argv[1];
    const char *s1 = argv[2];
    const char *s2 = argv[3];

    new huffman_code(s1, s2, param[1]);


    //archive->show_frequency_table();
    //archive->show_code_table();

    return 0;

    return 0;
}