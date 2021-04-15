#include "DictTrie.h"

static const char* const DICT_FILE = "../testdata/extra_dict/dict.small.utf8";
static const char* const USER_FILE = "../testdata/userdict.utf8";

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
    assert(2u == du->word.size());
    assert(26469u == du->word[0]);
    assert(21040u == du->word[1]);
    assert("v" == du->tag);

    word = "清华大学";
    LocalVector<pair<size_t, const DictUnit*>> res;
    const char *words[] = {"清", "清华", "清华大学"};

    for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
        assert(DecodeRunesInString(words[i], uni));
        res.push_back(make_pair(uni.size() - 1, trie.Find(uni.begin(), uni.end())));
    }

    vector<pair<size_t, const DictUnit*>> vec;
    vector<struct Dag> dags;

    assert(DecodeRunesInString(word, uni));
    trie.Find(uni.begin(), uni.end(), dags);

    assert(dags.size() == uni.size());
    assert(dags.size() != 0u);

}

void test_case_3() {

    DictTrie trie(DICT_FILE, USER_FILE);
    string word = "云计算";

    RuneStrArray unicode;
    assert(DecodeRunesInString(word, unicode));

    const DictUnit *unit = trie.Find(unicode.begin(), unicode.end());
    assert(unit != NULL);

    word = "蓝翔";
    assert(DecodeRunesInString(word, unicode));

    unit = trie.Find(unicode.begin(), unicode.end());
    assert(unit != NULL);
    assert(unit->tag == "nz");

    word = "区块链";
    assert(DecodeRunesInString(word, unicode));

    unit = trie.Find(unicode.begin(), unicode.end());
    assert(unit != NULL);
    assert(unit->tag == "nz");
}

void test_case_4() {

    DictTrie trie(DICT_FILE, USER_FILE, DictTrie::WordWeightMax);
    string word = "云计算";

    RuneStrArray unicode;
    assert(DecodeRunesInString(word, unicode));

    const DictUnit *unit = trie.Find(unicode.begin(), unicode.end());
    assert(unit != NULL);
}

void test_case_5() {

    DictTrie trie(DICT_FILE, USER_FILE);

    {
        string word = "清华大学";
        RuneStrArray unicode;
        assert(DecodeRunesInString(word, unicode));

        vector<struct Dag> res;
        trie.Find(unicode.begin(), unicode.end(), res);

        size_t nexts_sizes[] = {3, 2, 2, 1};
        assert(res.size() == sizeof(nexts_sizes) / sizeof(nexts_sizes[0]));

        for (size_t i = 0; i < res.size(); i++) {
            assert(res[i].nexts.size() == nexts_sizes[i]);
        }
    }
}

int main() {

    test_case_4();
    return 0;
}