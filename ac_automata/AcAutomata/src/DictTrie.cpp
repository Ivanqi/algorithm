#include "DictTrie.h"
#include "StringUtil.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>

DictTrie::DictTrie(const string& dict_path, const string& user_dict_paths, UserWordWeightOption user_word_weight_opt)
{
    Init(dict_path, user_dict_paths, user_word_weight_opt);
}

DictTrie::~DictTrie()
{
    delete trie_;
}

bool DictTrie::InsertUserWord(const string& word, const string& tag)
{
    DictUnit node_info;
    if (!MakeNodeInfo(node_info, word, user_word_default_weight_, tag)) {
        return false;
    }

    active_node_infos_.push_back(node_info);
    trie_->InsertNode(node_info.word, &active_node_infos_.back());
    return true;
}

bool DictTrie::InsertUserWord(const string& word, int freq, const string& tag)
{
    DictUnit node_info;
    double weight = freq ? log(1.0 * freq / freq_sum_) : user_word_default_weight_;

    if (!MakeNodeInfo(node_info, word, weight, tag)) {
        return false;
    }

    active_node_infos_.push_back(node_info);
    trie_->InsertNode(node_info.word, &active_node_infos_.back());

    return true;
}

const DictUnit* DictTrie::Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const
{
    return trie_->Find(begin, end);
}

void DictTrie::Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<struct Dag>& res, size_t max_word_len) const
{
    trie_->Find(begin, end, res, max_word_len);
}

bool DictTrie::Find(const string& word)
{
    RuneStrArray runes;

    if (!DecodeRunesInString(word, runes)) {
        printf("Decode failed\n");
    }

    const DictUnit *tmp = NULL;
    tmp = Find(runes.begin(), runes.end());
    if (tmp == NULL) {
        return false;
    } else {
        return true;
    }

    return true;
}

bool DictTrie::IsUserDictSingleChineseWord(const Rune& word) const
{
    return (user_dict_single_chinese_word_.find(word) != user_dict_single_chinese_word_.end());
}

double DictTrie::GetMinWeight() const
{
    return min_weight_;
}

void DictTrie::InserUserDictNode(const string& line)
{
    vector<string> buf;
    DictUnit node_info;
    split(line, buf, " ");

    if (buf.size() == 1) {
        MakeNodeInfo(node_info, buf[0], user_word_default_weight_, UNKNOWN_TAG);
    } else if (buf.size() == 2) {
        MakeNodeInfo(node_info, buf[0], user_word_default_weight_, buf[1]);
    } else if (buf.size() == 3) {
        int freq = atoi(buf[1].c_str());
        assert(freq_sum_ > 0.0);

        double weight = log(1.0 * freq / freq_sum_);
        MakeNodeInfo(node_info, buf[0], weight, buf[2]);
    }

    static_node_infos_.push_back(node_info);
    if (node_info.word.size() == 1) {
        user_dict_single_chinese_word_.insert(node_info.word[0]);
    }
}

void DictTrie::LoadUserDict(const vector<string>& buf)
{
    for (size_t i = 0; i < buf.size(); i++) {
        InsertUserWord(buf[i]);
    }
}

void DictTrie::LoadUserDict(const set<string>& buf)
{
    std::set<string>::const_iterator iter;
    for (iter = buf.begin(); iter != buf.end(); iter++) {
        InserUserDictNode(*iter);
    }
}

void DictTrie::LoadUserDict(const string& filePaths)
{
    vector<string> files = split(filePaths, "|;");
    size_t lineno = 0;
    for (size_t i = 0; i < files.size(); i++) {
        ifstream ifs(files[i].c_str());
        string line;

        if (!ifs.is_open()) {
            printf("ifs open %s failed\n", files[i].c_str());
            continue;
        }

        for (; getline(ifs, line); lineno++) {
            if (line.size() == 0) {
                continue;
            }
            InserUserDictNode(line);
        }
    }
}

void DictTrie::Init(const string& dict_path, const string& user_dict_paths, UserWordWeightOption user_word_weight_opt)
{
    LoadDict(dict_path);
    freq_sum_ = CalcFreqSum(static_node_infos_);
    CalculateWeight(static_node_infos_, freq_sum_);
    SetStaticWordWeights(user_word_weight_opt);

    if (user_dict_paths.size()) {
        LoadUserDict(user_dict_paths);
    }

    Shrink(static_node_infos_);
    CreateTrie(static_node_infos_);
}

void DictTrie::CreateTrie(const vector<DictUnit>& dictUnits)
{
    assert(dictUnits.size());
    vector<Unicode> words;
    vector<const DictUnit*> valuePointers;

    for (size_t i = 0; i < dictUnits.size(); i++) {
        words.push_back(dictUnits[i].word);
        valuePointers.push_back(&dictUnits[i]);
    }

    trie_ = new Trie(words, valuePointers);
}

bool DictTrie::MakeNodeInfo(DictUnit& node_info, const string& word, double weight, const string& tag)
{
    if (!DecodeRunesInString(word, node_info.word)) {
        printf("Decode %s failed\n", word.c_str());
        return false;
    }

    node_info.weight = weight;
    node_info.tag = tag;
    return true;
}

void DictTrie::LoadDict(const string& filePath)
{
    ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
        printf("ifs open %s failed\n", filePath.c_str());
    }

    string line;
    vector<string> buf;

    DictUnit node_info;
    for (size_t lineno = 0; getline(ifs, line); lineno++) {
        split(line, buf, " ");
        if (buf.size() < DICT_COLUMN_NUM) {
            printf("split result illegal line: %s\n", line.c_str());
            continue;
        }

        MakeNodeInfo(node_info, buf[0], atof(buf[1].c_str()), buf[2]);

        static_node_infos_.push_back(node_info);
    }
}

bool DictTrie::WeightCompare(const DictUnit& lhs, const DictUnit& rhs)
{
    return lhs.weight < rhs.weight;
}

void DictTrie::SetStaticWordWeights(UserWordWeightOption option)
{
    assert(!static_node_infos_.empty());

    vector<DictUnit> x = static_node_infos_;

    std::sort(x.begin(), x.end(), WeightCompare);

    min_weight_ = x[0].weight;
    max_weight_ = x[x.size() - 1].weight;
    median_weight_ = x[x.size() / 2].weight;

    switch (option) {
        case WordWeightMin:
            user_word_default_weight_ = min_weight_;
            break;
        
        case WordWeightMedian:
            user_word_default_weight_ = median_weight_;
            break;
        
        default:
            user_word_default_weight_ = max_weight_;
            break;
    }
}

double DictTrie::CalcFreqSum(const vector<DictUnit>& node_infos) const
{
    double sum = 0.0;
    for (size_t i = 0; i < node_infos.size(); i++) {
        sum += node_infos[i].weight;
    }

    return sum;
}

void DictTrie::CalculateWeight(vector<DictUnit>& node_infos, double sum) const
{
    assert(sum > 0.0);
    for (size_t i = 0; i < node_infos.size(); i++) {
        DictUnit& node_info = node_infos[i];
        assert(node_info.weight > 0.0);
        node_info.weight = log(double(node_info.weight) / sum);
    }
}

void DictTrie::Shrink(vector<DictUnit>& units) const
{
    vector<DictUnit>(units.begin(), units.end()).swap(units);
}