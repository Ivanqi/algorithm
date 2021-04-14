#ifndef UNICODE_H
#define UNICODE_H

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

inline RuneStrLite DecodeRuneInString(const char *str, size_t len)
{
    RuneStrLite rp(0, 0);
    if (str == NULL || len == 0) {
        return rp;
    }

    if (!(str[0] & 0x80)) { // 0xxxxxxx
        // 7bit, total 7bit
        rp.rune = (uint8_t)(str[0]) & 0x7f;
        rp.len = 1;
    } else if ((uint8_t)str[0] <= 0xdf && 1 < len) {
        // 110xxxxxx
        // 5bit, total 5bit
        rp.rune = (uint8_t)(str[0]) & 0x1f;

        // 6bit, total 11bit
        rp.rune <<= 6;
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