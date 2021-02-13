#include<iostream>
#include<string>
using namespace std;

// 宏，计算数组个数
#define GET_ARRAY_LEN(array,len){len=(sizeof(array)/sizeof(array[0]));}

string dict[] = {"计算", "计算语言学", "课程", "有", "意思"};

// 是否为词表中的词或者是词表中词的前缀
bool inDict(string str)
{
    bool res = false;
    int i;
    int len = 0;
    
    GET_ARRAY_LEN(dict, len);

    for (i = 0; i<len; i++)
    {
        // 是否和词表词相等或者是词表词前缀
        if( str == dict[i].substr(0, str.length()))
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
    string s1 = "";
    
    for (i = 0; i<sentence.length(); i = i+wordlen)
    {
        string tmp = s1 + sentence.substr(i, wordlen);

        if(inDict(tmp))
        {
            s1 = s1 + sentence.substr(i, wordlen);
        }
        else
        {
            cout<<"分词结果："<<s1<<endl;
            s1 = sentence.substr(i, wordlen);
        }
    }
    cout<<"分词结果："<<s1<<endl;
}