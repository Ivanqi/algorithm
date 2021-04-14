#ifndef TRIE_H
#define TRIE_H

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

const size_t MAX_WORD_LENGTH = 512;

struct DictUnit 
{
    Unicode word;
    double weight;
    string tag;
};

// inline ostream& operator << (ostream& os, const DictUnit& unit)
// {
//     string s;
//     s << unit.word;
//     return os << string_format("%s %s %.3lf", s.c_str(), unit.tag.c_str(), unit.weight);
// }

struct Dag
{
    
};

typedef LocalVector<std::pair<size_t, const DictUnit*>> DagType;

struct SegmentChar
{
    uint16_t uniCh;
    DagType dag;
    const DictUnit *pInfo;
    double weight;
    size_t nextPos;

    SegmentChar()
        :uniCh(0), pInfo(NULL), weight(0.0), nextPos(0)
    {

    }

    ~SegmentChar()
    {

    }
};

typedef Unicode::value_type TrieKey;

class TrieNode
{
    public:
        typedef unordered_map<TrieKey, TrieNode*> NextMap;
        TrieNode *fail; // 失败指针
        NextMap *next;
        const DictUnit *ptValue;

    public:
        TrieNode()
            :fail(NULL), next(NULL), ptValue(NULL)
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

class Trie
{
    private:
        TrieNode *root_;
    
    public:
        Trie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers);

        ~Trie();

    public:
        const DictUnit* find(Unicode::const_iterator begin, Unicode::const_iterator end) const;

        void find(Unicode::const_iterator begin, Unicode::const_iterator end, vector<struct SegmentChar>& res);

        bool find(Unicode::const_iterator begin, Unicode::const_iterator end, DagType& res, size_t offset = 0) const;

    private:
        void build_();

        void createTrie_(const vector<Unicode>& keys, const vector<const DictUnit*> & valuePointers);

        void insertNode_(const Unicode& key, const DictUnit* ptValue);

        void deleteNode_(TrieNode* node);
};

#endif