#ifndef ACAUTOMATE_H
#define ACAUTOMATE_H

#include "TransCode.h"

#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
using std::string;
using std::unordered_map;
using std::queue;
using std::vector;
using std::unordered_set;

typedef Unicode::value_type TrieKey;

class TrieNode
{
    public:
        typedef unordered_map<TrieKey, TrieNode*> NextMap;
        TrieNode *fail; // 失败指针
        NextMap *next;
        UnicodeValueType word;
        bool isEnding;  // 结尾字符为true
        int length;

    public:
        TrieNode()
            :fail(NULL), next(NULL)
        {

        }

        const TrieNode* findNext(TrieKey key) const
        {
            if (next == NULL) {
                return NULL;
            }

            NextMap::const_iterator iter = next->find(key);
            if (iter == next->end()) {
                return NULL;
            }

            return iter->second;
        }
};

class AcAutoMate
{
    private:
        TrieNode *root_;
    
    public:
        AcAutoMate()
        {
            root_ = new TrieNode;
        }

        ~AcAutoMate();

    public:
        const TrieNode* find(Unicode::const_iterator begin, Unicode::const_iterator end) const;

        void buildFailurePointer();

        void printfFailurePointer();

        void insertNode(const Unicode& key);

        string match(Unicode::const_iterator begin, Unicode::const_iterator end, string matchStr, string replaceStr);

        string replaceFun(unordered_map<int, int> check, string text, string replaceStr);

        void deleteNode(TrieNode* node);
};

#endif