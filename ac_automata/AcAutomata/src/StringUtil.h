#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <vector>

inline std::string string_format(const char *fmt, ...)
{
    int size = 256;
    std::string str;

    va_list ap;

    while (1) {
        str.resize(size);
        va_start(ap, fmt);

        int n = vsnprintf((char *)str.c_str(), size, fmt, ap);
        va_end(ap);

        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }

        if (n > -1) {
            size = n + 1;
        } else {
            size *= 2;
        }
    }

    return str;
}

inline bool split(const std::string& src, std::vector<std::string>& res, const string& pattern, size_t offset = 0, size_t len = std::string::npos)
{
    res.clear();
    size_t Start = 0;
    size_t end = 0;
    string sub;

    while(Start < src.size()) {
        end = src.find_first_of(pattern, Start);

        if (string::npos == end || res.size() >= len) {
            sub = src.substr(Start);
            res.push_back(sub);
            return true;
        }

        sub = src.substr(Start, end - Start);
        res.push_back(sub);
        Start = end + 1;
    }

    return true;
}

inline vector<std::string> split(const std::string& src, const std::string& pattern, size_t len = std::string::npos)
{
    vector<std::string> res;
    split(src, res, pattern, len);
    return res;
}

template <class Uint16Container>
bool Utf8ToUnicode(const char* const str, size_t len, Uint16Container& vec)
{
    if (!str) {
        return false;
    }

    char ch1, ch2;
    uint16_t tmp;
    vec.clear();

    for (size_t i = 0; i < len;) {
        if (!(str[i] & 0x80)) {     // 0xxxxxxx
            vec.push_back((str[i] & 0x7f));
            i++;
        } else if ((uint8_t)str[i] <= 0xdf && i + 1 < len) {    // 110xxxxxx
            ch1 = (str[i] >> 2) & 0x07;
            ch2 = (str[i + 1] & 0x3f) | ((str[i] & 0x03) << 6);
            tmp = (((uint16_t(ch1) & 0x00ff) << 8) | (uint16_t(ch2) & 0x00ff));
            vec.push_back(tmp);
            i += 2;
        } else if ((uint8_t)str[i] <= 0xef && i + 2 < len) {
            // 方法一
            // ch1 = ((uint8_t)str[i] << 4) | ((str[i + 1] >> 2) & 0x0f );
            // ch2 = (((uint8_t)str[i + 1] << 6) & 0xc0) | (str[i+2] & 0x3f);
            // tmp = (((uint16_t(ch1) & 0x00ff ) << 8) | (uint16_t(ch2) & 0x00ff));
            
            // 方法二
            tmp = (uint8_t)(str[i]) & 0x0f;

            tmp <<= 6;
            tmp |= (uint8_t)(str[i + 1]) & 0x3f;

            tmp <<= 6;
            tmp |= (uint8_t)(str[i + 2]) & 0x3f;

            vec.push_back(tmp);
            i += 3;
        } else {
            return false;
        }
    }

    return true;
}

template <class Uint16Container>
bool utf8ToUnicode(const string& str, Uint16Container& vec)
{
    return Utf8ToUnicode(str.c_str(), str.size(), vec);
}

template <class Uint16ContainerConIter>
void unicodeToUtf8(Uint16ContainerConIter begin, Uint16ContainerConIter end, std::string& res)
{
    res.clear();
    uint16_t ui;

    while (begin != end) {
        ui = *begin;

        if (ui <= 0x7f) {
            res += char(ui);
        } else if (ui <= 0x7ff) {
            res += char(((ui >> 6) & 0x1f) | 0xc0);
            res += char((ui & 0x3f) | 0x80);
        } else {
            res += char(((ui >> 12) & 0x0f) | 0xe0);
            res += char(((ui >> 6) & 0x3f) | 0x80);
            res += char((ui & 0x3f) | 0x80);
        }

        begin++;
    }
}


#endif