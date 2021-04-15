#ifndef DICTTRIE_H
#define DICTTRIE_H

#include "Trie.h"
#include <vector>
#include <deque>
#include <set>

using std::deque;
using std::set;

const double MIN_DOUBLE = -3.14e+100;
const double MAX_DOUBLE = 3.14e+100;
const size_t DICT_COLUMN_NUM = 3;
const char* const UNKNOWN_TAG = "";

class DictTrie
{
    public:
        enum UserWordWeightOption
        {
            WordWeightMin,
            WordWeightMedian,
            WordWeightMax
        };
    
    private:
        vector<DictUnit> static_node_infos_;
        deque<DictUnit> active_node_infos_;
        Trie *trie_;

        double freq_sum_;
        double min_weight_;
        double max_weight_;
        double median_weight_;
        double user_word_default_weight_;
        unordered_set<Rune> user_dict_single_chinese_word_;
    
    public:
        DictTrie(const string& dict_path, const string& user_dict_paths = "", UserWordWeightOption user_word_weight_opt = WordWeightMedian);

        ~DictTrie();

        bool InsertUserWord(const string& word, const string& tag = UNKNOWN_TAG);

        bool InsertUserWord(const string& word, int freg, const string& tag = UNKNOWN_TAG);

        const DictUnit* Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const;

        void Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<struct Dag>& res, size_t max_word_len = MAX_WORD_LENGTH) const;

        bool Find(const string& word);

        bool IsUserDictSingleChineseWord(const Rune& word) const;

        double GetMinWeight() const;

        void InserUserDictNode(const string& line);

        void LoadUserDict(const vector<string>& buf);

        void LoadUserDict(const set<string>& buf);

        void LoadUserDict(const string& filePaths);
    
    private:
        void Init(const string& dict_path, const string& user_dict_paths, UserWordWeightOption user_word_weight_opt);

        void CreateTrie(const vector<DictUnit>& dictUnits);

        bool MakeNodeInfo(DictUnit& node_info, const string& word, double weight, const string& tag);

        void LoadDict(const string& filePath);

        static bool WeightCompare(const DictUnit& lhs, const DictUnit& rhs);

        void SetStaticWordWeights(UserWordWeightOption option);

        double CalcFreqSum(const vector<DictUnit>& node_infos) const;

        void CalculateWeight(vector<DictUnit>& node_infos, double sum) const;

        void Shrink(vector<DictUnit>& units) const;
};

#endif