#include "AcAutoMate.h"

void test_case_1() {

    string word = "你好啊";

    string res;
    Unicode uni = TransCode::decode(word);
    TransCode::encode(uni.begin(), uni.end(), res);

    std::cout << res << std::endl;
}

void test_case_2() {

    vector<Unicode> keys;
    vector<const DictUnit*> values;

    keys.push_back(TransCode::decode("你好啊"));
    values.push_back((const DictUnit*)(NULL));

    AcAutoMate ac(keys, values);
}

int main() {

    test_case_2();
    return 0;
}