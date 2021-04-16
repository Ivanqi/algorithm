#include "AcAutoMate.h"

void test_case_1() {

    string word = "你好啊";

    string res;
    Unicode uni = TransCode::decode(word);
    TransCode::encode(uni.begin(), uni.end(), res);

    std::cout << res << std::endl;
}

void test_case_2() {
    AcAutoMate ac;
    
    ac.insertNode(TransCode::decode("朋友"));
    ac.insertNode(TransCode::decode("友情"));
    ac.insertNode(TransCode::decode("爱情"));
    ac.insertNode(TransCode::decode("请求"));
    ac.insertNode(TransCode::decode("我是谁"));
    ac.insertNode(TransCode::decode("是谁"));
    ac.insertNode(TransCode::decode("谁"));
    ac.buildFailurePointer();
    ac.printfFailurePointer();

    string matchStr = "谁在哪里儿，赶快回答. 朋友";
    Unicode matchUni = TransCode::decode(matchStr);
    string ret = ac.match(matchUni.begin(), matchUni.end(), matchStr, "*");
    std::cout << "ret: " << ret << std::endl;
}

int main() {

    test_case_2();
    return 0;
}