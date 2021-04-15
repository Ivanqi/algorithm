#include "DictTrie.h"

static const char* const DICT_FILE = "../testdata/extra_dict/dict.small.utf8";


void test_case_1() {

    DictTrie *trie;
    trie = new DictTrie(DICT_FILE);
    delete trie;
}

void test_case_2() {

    string s1, s2;
    DictTrie trie(DICT_FILE);

    string word("来到");
    RuneStrArray uni;
    assert(DecodeRunesInString(word, uni));

    const DictUnit *du = trie.Find(uni.begin(), uni.end());
    assert(du != NULL);
}

int main() {

    test_case_2();
    return 0;
}