#include <iostream>
#include <string>
using namespace std;

/**
 * 参考资料: https://www.cnblogs.com/by-dream/p/6429615.html
 * 算法思想:
 *  正向最大匹配算法: 从左到右将待分词文本中的几个连续字符与词表匹配，如果匹配上，则切分出一个词。
 *  但这里有一个问题: 要做到最大匹配，并不是第一次匹配到就可以切分的
 *  例子:
 *      待分词文本: sentence[] = {"计"，"算"，"语"，"言"，"学"，"课"，"程"，"有"，"意"，"思"}
 *      词表: dict[]={"计算", "计算语言学", "课程", "有", "意思"}  （真实的词表中会有成千上万个已经平时我们使用的分好的词语）
 *  流程
 *      (1) 从sentence[1]开始,当扫描到sentence[2]的时候，发现"计算"已经在词表dict[]中。
 *          但还不能切分出来，因为不知道后面的词语能不能组成更长的词(最大匹配)
 * 
 *      (2) 继续扫描sentence[3], 发现"计算语"并不是dict[]中的词
 *          但是我们还不能确定是否前面找到的"计算语"已经是最大的词了，因此"计算语"是dict[2]的前缀
 *      
 *      (3) 扫描sentence[4], 发现"计算语言"并不是dict[]中的词. 但是是dict[2]的前缀。继续扫描
 * 
 *      (4) 扫描sentence[5], 发现"计算语言学"是dict[]中的词.继续扫描下去
 * 
 *      (5) 当扫描content[6]的时候, 发现"计算语言学课"并不是词表中的词，也不是词的前缀。因此可以切分出前面的最大的词 -- "计算语言学"
 *          由此可见，最大匹配出的词必须保证下一个扫描不是词表中的词或词的前缀才可以结束
 */

// 宏，计算数组个数
#define GET_ARRAY_LEN(array, len) {len = (sizeof(array) / sizeof(array[0]));}

string dict[] = {"计算", "计算语言学", "课程", "有", "意思"};

// 是否为词表中的词或者词表中词的前缀
bool inDict(string str) {
    bool res = false;
    int i;
    int len = 0;

    GET_ARRAY_LEN(dict, len);

    for (i = 0; i < len; i++) {
        // 是否和词表词相等或者是词表前缀
        if (str == dict[i].substr(0, str.length())) {
            res = true;
        }
    }

    return res;
}

int main() {

    string sentence = "计算语言学课程真有意思";
    string word = "一";
    int wordlen = word.length();

    int i;
    string s1 = "";

    for (i = 0; i < sentence.length(); i = i + wordlen) {
        string tmp = s1 + sentence.substr(i, wordlen);
        // cout << "tmp: " << tmp << endl; 

        if (inDict(tmp)) {
            s1 = s1 + sentence.substr(i, wordlen);
        } else {
            cout << "分词结果: " << s1 << endl;
            s1 = sentence.substr(i, wordlen);
        }
    }

    cout << "分词结果: " << s1 << endl;

    return 0;
}