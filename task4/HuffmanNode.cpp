#include <cstdio>
#include <iostream>
#include <fstream>
#include "HuffmanNode.h"

huffman_node::huffman_node() {
    letter = ' ';
    key = 0;
    left = std::nullptr_t();
    right = std::nullptr_t();

}

huffman_node::huffman_node(char let, int32_t k) {
    letter = let;
    key = k;
    left = std::nullptr_t();
    right = std::nullptr_t();
}

huffman_node::huffman_node(const huffman_node& node) {
    if (node.left != std::nullptr_t()) {
        left = new huffman_node();
        *left = *(node.left);
    }
    if (node.right != std::nullptr_t()) {
        right = new huffman_node();
        *right = *(node.right);
    }
    code = node.code;
    letter = node.letter;
    key = node.key;
}

huffman_node::huffman_node(huffman_node* l, huffman_node* r) {
    left = l;
    right = r;
    key = left->get_key() + right->get_key();
}

huffman_node::~huffman_node(){
    if (left != std::nullptr_t()) delete left;
    if (right != std::nullptr_t()) delete right;
}