#ifndef HASHFUNC_H
#define HASHFUNC_H
#include <string>
#include "md5.h"

class HashFunc
{
    public:
        virtual unsigned int GetKey(std::string str) = 0;
};

#endif