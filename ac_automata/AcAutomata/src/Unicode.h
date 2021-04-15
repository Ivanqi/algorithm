#ifndef UNICODE_H
#define UNICODE_H

#include "LocalVector.h"

#include <string>
using std::string;

typedef uint32_t Rune;

struct RuneStr
{
    Rune rune;
    uint32_t offset;
    uint32_t len;
    uint32_t unicode_offset;
    uint32_t unicode_length;
    
    RuneStr()
        : rune(0), offset(0), len(0), unicode_offset(0), unicode_length(0)
    {

    }

    RuneStr(Rune r, uint32_t o, uint32_t l)
        : rune(r), offset(o), len(l), unicode_offset(0), unicode_length(0)
    {

    }

    RuneStr(Rune r, uint32_t o, uint32_t l, uint32_t unicode_offset, uint32_t unicode_length)
        : rune(r), offset(o), len(l), unicode_offset(unicode_offset), unicode_length(unicode_length)
    {

    }
};

typedef LocalVector<Rune> Unicode;
typedef LocalVector<struct RuneStr> RuneStrArray;

struct RuneStrLite
{
    uint32_t rune;
    uint32_t len;

    RuneStrLite()
        :rune(0), len(0)
    {

    }

    RuneStrLite(uint32_t r, uint32_t l)
        :rune(r), len(l)
    {

    }
};

/**
 * unicode 转化成utf-8编码
 * 参考资料
 *  https://www.cnblogs.com/benbenalin/tag/Unicode/
 *  https://www.cnblogs.com/cthon/p/9297232.html
 * 
 * UTF-8编码规则
 *  1. 对于单字节的符号，字节的第一位设为0，后面7为这个符号的unicode码，因此对于英语字母，UTF-8编码和ASCII码是相同的
 *  2. 对于n字节的符号(n > 1)，第一个字节的前n位都设位1。第n + 1位都设为0，后面字节的前面一律设为0，剩下的没有提及的二进制位，全部为这个符号的unicode码
 * 
 * |------- Unicode ------- | -- UTF-8 --
 * ---------------------------- 
 * | 0000 0000  - 0000 007F |                                                    0xxx xxxx
 * | 0000 0000  - 0000 07FF |                                          110x xxxx 10xx xxxx
 * | 0000 0000  - 0000 FFFF |                                1110 xxxx 10xx xxxx 10xx xxxx
 * | 0001 0000  - 0010 FFFF |                      1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
 * | 0020 0000  - 03FF FFFF |            1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
 * | 0400 0000  - 7FFF FFFF |  1111 110x 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx    
 * 
 * 例子:
 *  已知‘严’的unicoe是4E25(100 1110 0010 0101)，根据上表，可以发现4E25处在第三行的范围内(0000 0000  - 0000 FFFF)
 *  因此'严'的UTF-8编码需要三个字节，即格式是"1110 xxxx 10xx xxxx 10xx xxxx"    
 *  然后从'严'的最后一个二进制位开始，依次从后向前填入格式中的x，多处的位补0
 *  这样就得到了,'严'的UTF-8编码 "1110 0100 1011 1000 1010 0101"，转换成十六进制就是E4B8A5      
 */



// utf8 转 unicode
inline RuneStrLite DecodeRuneInString(const char *str, size_t len)
{
    RuneStrLite rp(0, 0);
    if (str == NULL || len == 0) {
        return rp;
    }

    if (!(str[0] & 0x80)) { // 0xxxxxxx ASCII码表范围, 代标1位
        // 7bit, total 7bit
        rp.rune = (uint8_t)(str[0]) & 0x7f;
        rp.len = 1;
    } else if ((uint8_t)str[0] <= 0xdf && 1 < len) {
        // 110xxxxxx
        // 5bit, total 5bit
        rp.rune = (uint8_t)(str[0]) & 0x1f; // 0x1F = 31 = 0001 1111

        // 6bit, total 11bit
        rp.rune <<= 6;  // 移动6位
        rp.rune |= (uint8_t)(str[1]) & 0x3f;
        rp.len = 2;
    } else if ((uint8_t)str[0] <= 0xef && 2 < len) {    // 1110xxxxxx
        // 4bit, total 4bit
        rp.rune = (uint8_t)(str[0]) & 0x0f;

        // 6bit, total 10bit
        rp.rune <<= 6;
        rp.rune |= (uint8_t)(str[1]) & 0x3f;

        // 6bit, total 16bit
        rp.rune <<= 6;
        rp.rune |= (uint8_t)(str[2]) & 0x3f;

        rp.len = 3;
    } else if ((uint8_t)str[0] <= 0xf7 && 3 < len) {    // 11110xxxx
        // 3bit, total 3bit
        rp.rune = (uint8_t)(str[0]) & 0x07;

        // 6bit, total 9bit
        rp.rune <<= 6;
        rp.rune |= (uint8_t)(str[1]) & 0x3f;

        // 6bit, total 15bit
        rp.rune <<= 6;
        rp.rune |= (uint8_t)(str[2]) & 0x3f;

        // 6bit, total 21bit
        rp.rune <<= 6;
        rp.rune |= (uint8_t)(str[3]) & 0x3f;

        rp.len = 4;
    } else {
        rp.rune = 0;
        rp.len = 0;
    }

    return rp;
}

inline bool DecodeRunesInString(const char *s, size_t len, RuneStrArray& runes)
{
    runes.clear();
    runes.reserve(len / 2);

    for (uint32_t i = 0, j = 0; i < len;) {
        RuneStrLite rp = DecodeRuneInString(s + i, len - i);
        if (rp.len == 0) {
            runes.clear();
            return false;
        }

        RuneStr x(rp.rune, i, rp.len, j, 1);
        runes.push_back(x);

        i += rp.len;
        ++j;
    }

    return true;
}

inline bool DecodeRunesInString(const char *s, size_t len, Unicode& unicode)
{
    unicode.clear();
    RuneStrArray runes;

    if (!DecodeRunesInString(s, len, runes)) {
        return false;
    }

    unicode.reserve(runes.size());
    for (size_t i = 0; i < runes.size(); i++) {
        unicode.push_back(runes[i].rune);
    }

    return true;
}

inline bool DecodeRunesInString(const string& s, RuneStrArray& runes) 
{
    return DecodeRunesInString(s.c_str(), s.size(), runes);
}

inline bool DecodeRunesInString(const string& s, Unicode& unicode)
{
    return DecodeRunesInString(s.c_str(), s.size(), unicode);
}

inline Unicode DecodeRunesInString(const string& s)
{
    Unicode result;
    DecodeRunesInString(s, result);
    return result;
}

#endif