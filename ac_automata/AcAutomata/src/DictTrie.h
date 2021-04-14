#ifndef DICTTRIE_H
#define DICTTRIE_H

#include "Trie.h"

const double MIN_DOUBLE = -3.14e+100
const double MAX_DOUBLE = 3.14e+100;
const size_t DICT_COLUMN_NUM = 3;
const char* const UNKONWN_TAG = "";

class DictTrie
{
    private:
        vector<DictUnit> nodeInfos_;
        Trie *trie_;

        double minWeight_;
        unordered_set<Unicode::value_type> userDictSingleChineseWord_;
    
    public:
        DictTrie();

        DictTrie(const string& dictPath, const string& userDictPath = "");

        ~DictTrie();

        void init(const string& dictPath, const string& userDictPath = "");

        const DictUnit* find(Unicode::const_iterator begin, Unicode::const_iterator end) const;

        bool find(Unicode::const_iterator begin, Unicode::const_iterator end, DagType& dag, size_t offset = 0) const;

        void find(Unicode::const_iterator begin, Unicode::const_iterator end, vector<SegmentChar>& res);

        bool isUserDictSingleChineseWord(const Unicode::value_type& word) const;

        double getMinWeight() const;

    private:
        Trie* createTrie_(const vector<DictUnit>& dictUnits);

        void loadUserDict_(const string& filePath, double defaultWeight, const string& defaultTag);

        void loadDict_(const string& filePath);

        double findMinWeight_(const vector<DictUnit>& nodeInfos) const;

        double findMaxWeight_(const vector<DictUnit>& nodeInfos) const;

        void calculateWeight_(vector<DictUnit>& nodeInfos) const;

        void shrink_(vector<DictUnit>& units) const;
};

#endif