#include "AcAutoMata.h"

void test_case_1() {

    string word = "你好啊";

    string res;
    Unicode uni = TransCode::decode(word);
    TransCode::encode(uni.begin(), uni.end(), res);

    std::cout << res << std::endl;
}

void test_case_2() {
    AcAutoMata ac;
    
    ac.insertNode(TransCode::decode("朋友"));
    ac.insertNode(TransCode::decode("友情"));
    ac.insertNode(TransCode::decode("爱情"));
    ac.insertNode(TransCode::decode("请求"));
    ac.insertNode(TransCode::decode("我是谁"));
    ac.insertNode(TransCode::decode("是谁"));
    ac.insertNode(TransCode::decode("谁"));
    ac.buildFailurePointer();
    ac.printfFailurePointer();

    string matchStr = "谁在哪里儿，赶快回答. 朋友, 请回答，别忘了我们的友情";
    Unicode matchUni = TransCode::decode(matchStr);
    string ret = ac.match(matchUni.begin(), matchUni.end(), matchStr, '*');
    std::cout << "ret: " << ret << std::endl;
}

void test_case_3() {
    
    AcAutoMata ac;
    ac.insertNode(TransCode::decode("red"));
    ac.insertNode(TransCode::decode("国"));
    ac.insertNode(TransCode::decode("Short"));
    ac.insertNode(TransCode::decode("为"));

    string matchStr = "Review red history，传承红色基因，感悟革命先辈捐躯赴国难的崇高品格，延续共产党人精神血脉！欢迎您用积极生动的Short video works，描绘中国共产党百年征程中历久弥坚的百年初心，讲述为民族独立和人民解放、为祖国建设发展而奋斗的英雄故事";
    Unicode matchUni = TransCode::decode(matchStr);
    string ret = ac.match(matchUni.begin(), matchUni.end(), matchStr, '*');
    std::cout << "ret: " << ret << std::endl;
}

int main() {

    test_case_3();
    return 0;
}