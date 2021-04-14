#include "Trie.h"

void test_case_1() {

    vector<Unicode> keys;
    vector<const DictUnit*> values;
    Trie trie(keys, values);
}

void test_case_2() {
    vector<Unicode> keys;
    vector<const DictUnit*> values;
    keys.push_back(DecodeRunesInString("你是风儿"));
    values.push_back((const DictUnit*)(NULL));

    Trie trie(keys, values);
}


int main() {

    test_case_2();
    return 0;
}
