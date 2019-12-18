#include "Huffman.h"


huffman_code::huffman_code(const char *s1, const char *s2, const char param) {
    data = s1;
    out = s2;
    if (param == 'c') {
        build_frequency_table();
        build_tree();
        std::string code = "";
        int32_t root = tree.size() - 1;
        build_code_table(tree[root], code);
        std::reverse(code_tree.begin(), code_tree.end());
        compress();
    } else {
        decompress();
    }
}

huffman_code::~huffman_code(){
    tree.clear();
    frequency_table.clear();
    code_table.clear();
}

void huffman_code::build_frequency_table() {
    std::cout << "Building frequency table..." << std::endl;
    std::ifstream input;
    char let = 'a';
    input.open(data);
    input.get(let);
    while (!input.eof()) {
        if (frequency_table.find(let) == frequency_table.end()) {
            frequency_table[let] = 1;
        } else {
            frequency_table[let]++;
        }
        input.get(let);
    }
    input.close();
    std::cout << "Frequency table successfuly build" << std::endl;

}

void huffman_code::build_code_table(huffman_node* root, std::string code) {
    std::cout << "Building code table..." << std::endl;

    if (root->get_left() == std::nullptr_t() && root->get_right() == std::nullptr_t()) {
        std::vector<bool> bool_code;
        for (auto a : code) bool_code.push_back (a == '1');
        code_table[root->get_letter()] = bool_code;
        return;
    }
    if (root->get_left() != std::nullptr_t()) {
        build_code_table(root->get_left(), code + "0");
    }
    if (root->get_right() != std::nullptr_t()) {
        build_code_table((root->get_right()), code + "1");
    }
    std::cout << "Code table build successful" << std::endl;

}

void huffman_code::encode_tree(huffman_node* root) {
    if (root->get_left() == std::nullptr_t() && root->get_right() == std::nullptr_t()) {
        code_tree.push_back('1');
        code_tree.push_back(root->get_letter());
    } else {
        code_tree.push_back('0');
        if (root->get_left() != std::nullptr_t()) {
            encode_tree(root->get_left());
        }
        if (root->get_right() != std::nullptr_t()) {
            encode_tree(root->get_right());
        }
    }
}

void huffman_code::build_tree() {
    std::cout << "Building tree..." << std::endl;
    std::vector<std::pair<char, int32_t>> freq_vector(frequency_table.begin(), frequency_table.end());
    std::vector<huffman_node*> temp;
    for (auto it = frequency_table.begin(); it != frequency_table.end(); ++it) {
        temp.push_back(new huffman_node(it->first, it->second));
    }
    std::sort(temp.begin(), temp.end(),
              [](auto &left, auto &right) {
                  return left->get_key() > right->get_key();
              }
    );

    while (temp.size() != 1) {
        int32_t len = temp.size() - 1;
        auto left_node = temp[len];
        auto right_node = temp[len - 1];
        tree.push_back(left_node);
        tree.push_back(right_node);
        auto parent = new huffman_node(left_node, right_node);
        tree.push_back(parent);
        temp.resize(len - 1);
        temp.push_back(parent);
        std::sort(temp.begin(), temp.end(),
                  [](auto &left, auto &right) {
                      return left->get_key() > right->get_key();
                  }
        );
    }
    temp.clear();
    std::cout << "Tree build successful" << std::endl;
}

void huffman_code::show_frequency_table() {
    for (auto it = frequency_table.begin(); it != frequency_table.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }
}

void huffman_code::show_code_table() {
    for (auto it = code_table.begin(); it != code_table.end(); ++it) {
        std::cout << it->first << " ";
        for (auto code_bool = (it->second).begin();
             code_bool != (it->second).end();
             ++code_bool) {
            std::cout << *code_bool;
        }
        std::cout << std::endl;
    }
}

int32_t huffman_code::getnumb_of_char() {
    int32_t res = 0;
    for (auto it = code_table.begin(); it != code_table.end(); ++it) {
        res += (it->second).size() * frequency_table[it->first];
    }
    return res;
}

void huffman_code::compress() {
    encode_tree(tree[tree.size() - 1]);
    size_of_tree = code_tree.size();
    std::ifstream input;
    char let = 'a';
    input.open(data);
    std::ofstream output;
    output.open(out, std::ios::binary);
    char code = 0;
    int32_t bitcount = 0;
    input.get(let);
    numb_of_characters = getnumb_of_char();
    output.write((char*) &numb_of_characters, sizeof(int32_t));
    output.write((char*) &size_of_tree, sizeof(int32_t));
    for (auto it = code_tree.begin(); it != code_tree.end(); ++it) {
        output.write((char*) &(*it), sizeof(char));
    }
    std::cout << "Starting encryption..." << std::endl;
    while (!input.eof()) {
        for (auto it = code_table[let].begin(); it != code_table[let].end(); ++it) {
            if (*it == 0) {
                code = code << 1;
            } else {
                code = (code << 1) | 1;
            }
            bitcount++;
            if (bitcount == 8) {
                output.write(&code, sizeof(char));
                bitcount = 0;
                code = 0;
            }
        }
        input.get(let);
    }
    if (bitcount != 0) {
        while (bitcount != 8) {
            code = code << 1;
            bitcount++;
        }
        output.write(&code, sizeof(char));
    }
    output.close();
    input.close();
    std::cout << "Compression complete!" << std::endl;
    return;
}

std::vector<bool> huffman_code::char_to_byte(char c){
    std::vector<bool> res(8);
    for (int32_t i = 0; i < 8; ++i) {
        if ((c & (1 << i)) == 0) {
            res[7 - i] = 0;
        } else {
            res[7 - i] = 1;
        }
    }
    return res;
}

huffman_node* huffman_code::rebuild_tree() {
    std::cout << "Building tree..." << std::endl;
    std::stack<huffman_node*> stack;
    char q = '1';
    if (code_tree[0] == q) {
        huffman_node* root = new huffman_node(code_tree[1], 1);
        return root;
    } else {
        huffman_node* root = new huffman_node();
        stack.push(root);
        for (uint32_t i = 1; i < code_tree.size(); ++i) {
            while (!stack.empty() && stack.top()->get_left() != std::nullptr_t() &&
                   stack.top()->get_right() != std::nullptr_t()) {
                stack.pop();
            }
            if (code_tree[i] == '1') {
                if (stack.top()->get_left() == std::nullptr_t()) {
                    auto b = new huffman_node(code_tree[i + 1], 1);
                    ++i;
                    stack.top()->left = b;
                } else {
                    auto b = new huffman_node(code_tree[i + 1], 1);
                    ++i;
                    stack.top()->right = b;
                }
            } else {
                if (stack.top()->get_left() == std::nullptr_t()) {
                    auto b = new huffman_node();
                    stack.top()->left = b;
                    stack.push(b);
                } else {
                    auto b = new huffman_node();
                    stack.top()->right = b;
                    stack.push(b);
                }
            }
        }
        return root;
    }
    std::cout << "Tree build successful" << std::endl;

}

void huffman_code::decompress() {
    std::ifstream input;
    std::ofstream output;
    input.open(data, std::ios::in | std::ios::binary);
    output.open(out);

    char let = 'a';
    input.read((char*) &numb_of_characters, sizeof(int32_t));
    input.read((char*) &size_of_tree, sizeof(int32_t));

    code_tree.resize(size_of_tree);
    for (int32_t i = 0; i < size_of_tree; ++i) {
        input.get(let);
        code_tree[i] = let;
    }
    auto root = rebuild_tree();
    auto search = root;
    int32_t count = 0;
    input.get(let);

    while (!input.eof()) {
        auto code = char_to_byte(let);
        for (int32_t i = 0; i < 8; ++i) {
            if (search->get_left() == std::nullptr_t() &&
                search->get_right() == std::nullptr_t()) {
                output << search->get_letter();
                search = root;
            }
            if (code[i] == 1) {
                search = search->get_right();
            } else {
                search = search->get_left();
            }
            if (count > numb_of_characters) {
                break;
            }
            count++;
        }
        input.get(let);
    }
    output.close();
    input.close();
    std::cout << "Decompression complete!" << std::endl;

    return;
}