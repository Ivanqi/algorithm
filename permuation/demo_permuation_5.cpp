#include <iostream>
#include <string>

using std::string;
using namespace std;

void BreakPassword(string Words, int PasswordLen, string result)
{
    if (result.length() == PasswordLen) {
        // C++种string类型不能直接输出，需加头文件 #include<string> 。不能用#include<string.h>
        cout << result << "\n";
        return ;
    }

    for (int i = 0; i < Words.length(); i++) {
        string newResult = result;
        newResult.insert(newResult.end(), Words[i]);
        BreakPassword(Words, PasswordLen, newResult);
    }
}

int main (void) {
    int passwordLen = 4;
    string words("abcde");
    string result = "";

    BreakPassword(words, passwordLen, result);
    return 0;
}