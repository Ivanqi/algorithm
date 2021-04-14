#ifndef TRANSCODE_H
#define TRANSCODE_H

#include "LocalVector.h"
#include "StringUtil.h"

typedef uint16_t UnicodeValueType;
typedef LocalVector<UnicodeValueType> Unicode;

namespace TransCode
{
    inline bool decode(const std::string& str, Unicode& res)
    {
        return utf8ToUnicode(str, res);
    }

    inline bool encode(Unicode::const_iterator begin, Unicode::const_iterator end, std::string& res) 
    {
        unicodeToUtf8(begin, end, res);
    }
};

#endif