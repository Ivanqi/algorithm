#ifndef TRIE_H
#define TRIE_H

#include "LocalVector.h"
#include "Unicode.h"

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


struct Dag
{
    RuneStr runestr;
    LocalVector<pair<size_t, const DictUnit*>> nexts;
    const DictUnit *pInfo;
    double weight;
    size_t nextPos;

    Dag()
        : runestr(), pInfo(NULL), weight(0.0), nextPos(0)
    {

    }
};

typedef Rune TrieKey;

class TrieNode
{
    public:
        typedef unordered_map<TrieKey, TrieNode*> NextMap;
        NextMap *next;
        const DictUnit *ptValue;

    public:
        TrieNode()
            :next(NULL), ptValue(NULL)
        {

        }
};

class Trie
{
    private:
        TrieNode *root_;

    public:
        Trie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers);

        ~Trie();

        const DictUnit* Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const;

        void Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<struct Dag>&res, size_t max_word_len = MAX_WORD_LENGTH) const;

        void InsertNode(const Unicode& key, const DictUnit* ptValue);

    private:
        void CreateTrie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers);

        void DeleteNode(TrieNode* node);
};

#endif