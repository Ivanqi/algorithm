#include <iostream>
#include <string>
using namespace std;

/**
 * 参考资料: 
 *      https://www.cnblogs.com/by-dream/p/6429615.html
 *      https://www.cnblogs.com/xuelisheng/p/9712973.html
 *  
 * 算法思想:
 *  逆向匹配算法大致思路是从右到左开始切分
 *  例子
 *      待分词句子: sentence[] = {"计算语言学课程有意思"}
 *      词表: dict[] = {"计算", "计算语言学", "课程", "有", "意思"}
 *  首先定义一个最大分割长度5，从右往左开始分割
 *      (1) 首先取出来的候选词W是"课程真有意思"
 *      
 *      (2) 查词表, W不在词表中，将W最左边的第一个字去掉，得到W"程有意思"
 * 
 *      (3) 查词表，W也不在词表中，将W最左边的第一个字去掉，得到W“有意思”
 * 
 *      (4) 查词表，W也不在词表中，将W最左边的第一个字再去掉，得到W“意思”
 * 
 *      (5) 查词表，W在词表中，就将W从整个句子中拆分出来，此时原句子为“计算语言学课程有”
 *  
 *      (6) 根据分割长度5，截取句子内容，得到候选句W是“语言学课程有”；
 *      
 *      (7) 查词表，W不在词表中，将W最左边的第一个字去掉，得到W“言学课程有”；
 * 
 *      (8) 查词表，W也不在词表中，将W最左边的第一个字去掉，得到W“学课程有”
 * 
 *      (9) 依次类推，直到W为“有”一个词的时候，这时候将W从整个句子中拆分出来，此时句子为“计算语言学课程”
 * 
 *      (10) 根据分割长度5，截取句子内容，得到候选句W是“算语言学课程”；
 * 
 *      (11) 查词表，W不在词表中，将W最左边的第一个字去掉，得到W“语言学课程”
 * 
 *      (12) 依次类推，直到W为“课程”的时候，这时候将W从整个句子中拆分出来，此时句子为“计算语言学”
 * 
 *      (13) 根据分割长度5，截取句子内容，得到候选句W是“计算语言学”；
 * 
 *      (14) 查词表，W在词表，分割结束
 */

// 宏，计算组成个数
#define GET_ARRAY_LEN(array, len) {len = (sizeof(array) / sizeof(array[0]));}

// 定义最长词长
#define MAX_WORD_LENGTH 5

string dict[] = {"计算", "计算语言学", "课程", "意思"};

// 是否为词表中的词
bool inDict(string str) {
    bool res = false;
    int i;
    int len = 0;

    GET_ARRAY_LEN(dict, len);

    for (i = 0; i < len; i++) {
        if (str == dict[i]) {
            res = true;
        }
    }

    return res;
}

int main() {

    string sentence = "计算语言学课程有意思";
    string word = "一";
    int wordlen = word.length();

    int i;

    for (i = 0; i < sentence.length() ;) {
        int dealstrbegin = sentence.length() - wordlen * MAX_WORD_LENGTH - i;
        int dealstrlen = wordlen * MAX_WORD_LENGTH;

        // 截取的要处理的字符串
        string dealstr = sentence.substr(dealstrbegin, dealstrlen);

        int j;

        for (j = 0; j < MAX_WORD_LENGTH; j++) {
            int fb = j * wordlen;
            int fl = wordlen * (MAX_WORD_LENGTH - j);

            // 去掉签名的j个字
            string tmp = dealstr.substr(fb, fl);

            if (inDict(tmp) || j == MAX_WORD_LENGTH - 1) {
                cout << "分词结果: " << tmp << endl;
                i = i + fl;
                break;
            }
        }
    }

    return 0;
}