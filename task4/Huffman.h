#ifndef TASK2_HUFFMAN_H
#define TASK2_HUFFMAN_H

#include <string>
#include <map>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include "HuffmanNode.h"

class huffman_code {
private:
    int32_t size_of_tree;
    int32_t numb_of_characters;
    const char *data;
    const char *out;
    std::vector<huffman_node*> tree;
    std::map<char, int32_t> frequency_table;
    std::map<char, std::vector<bool>> code_table;
    std::vector<char> code_tree;
    void build_frequency_table();
    huffman_node* rebuild_tree();
    void build_code_table(huffman_node* root, std::string code);
    void build_tree();
    std::vector<bool> char_to_byte(char c);
    int32_t getnumb_of_char();
    void encode_tree(huffman_node* root);

public:
    huffman_code();
    huffman_code(const char *s1, const char *s2, const char param);
    ~huffman_code();
    void compress();
    void decompress();
    void show_frequency_table();
    void show_code_table();
};

#endif //TASK2_HUFFMAN_H
