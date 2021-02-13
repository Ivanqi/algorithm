#include<iostream>
#include<string>
using namespace std;

// 宏，计算数组个数
#define GET_ARRAY_LEN(array,len){len=(sizeof(array)/sizeof(array[0]));}

// 定义最大词长
#define MAX_WORD_LENGTH 5

string dict[] = {"计算", "计算语言学", "课程", "意思"};

// 是否为词表中的词
bool inDict(string str)
{
    bool res = false;
    int i;
    int len = 0;
    
    GET_ARRAY_LEN(dict, len);

    for (i = 0; i<len; i++)
    {
        if( str == dict[i]) 
        {
            res = true;
        }
    }
    return res;
}


int main()
{
    string sentence = "计算语言学课程有意思";
    string word = "一";
    int wordlen = word.length();
    
    int i;
    
    for (i = 0; i<sentence.length(); )
    {
        int dealstrbegin = sentence.length()-wordlen*MAX_WORD_LENGTH-i;
        int dealstrlen = wordlen*MAX_WORD_LENGTH;
        // 截取的要处理的字符串
        string dealstr = sentence.substr(dealstrbegin, dealstrlen);

        int j;
        for (j = 0; j<MAX_WORD_LENGTH; j++)
        {
            int fb = j*wordlen;
            int fl = wordlen*(MAX_WORD_LENGTH-j);
            // 去掉签名的j个字
            string tmp = dealstr.substr(fb, fl);

            if(inDict(tmp) || j==MAX_WORD_LENGTH-1 )
            {
                cout<<"分词结果："<<tmp<<endl;
                i=i+fl;
                break;
            }
        }
    }
}
