#include "DictTrie.h"
#include "StringUtil.h"
#include <stdio.h>
#include <algorithm>

DictTrie::DictTrie()
{
    trie_ = NULL;
    minWeight_ = MAX_DOUBLE;
}

DictTrie::DictTrie(const string& dictPath, const string& userDictPath = "")
{
    new (this) DictTrie();
    init(dictPath, userDictPath);
}

DictTrie::~DictTrie()
{
    if (trie_) {
        delete trie_;
    }
}

void DictTrie::init(const string& dictPath, const string& userDictPath = "")
{
    if (trie_ != NULL) {
        printf("trie already initted\n");
    }

    loadDict_(dictPath);
    calculateWeight_(nodeInfos_);
    minWeight_ = findMinWeight_(nodeInfos_);

    if (userDictPath.size()) {
        double maxWeight = findMaxWeight_(nodeInfos_);
        loadUserDict_(userDictPath, maxWeight, UNKONWN_TAG);
    }

    shrink_(nodeInfos_);
    trie_ = createTrie_(nodeInfos_);
    assert(trie_);
}

const DictUnit* DictTrie::find(Unicode::const_iterator begin, Unicode::const_iterator end) const
{
    return trie_->find(begin, end);
}

bool DictTrie::find(Unicode::const_iterator begin, Unicode::const_iterator end, DagType& dag, size_t offset = 0) const
{
    return trie_->find(begin, end, dag, offset);
}

void DictTrie::find(Unicode::const_iterator begin, Unicode::const_iterator end, vector<SegmentChar>& res)
{
    trie_->find(begin, end, res);
}

bool DictTrie::isUserDictSingleChineseWord(const Unicode::value_type& word) const
{
    return isIn(userDictSingleChineseWord_, word);
}

double DictTrie::getMinWeight() const
{
    return minWeight_;
}

Trie* DictTrie::createTrie_(const vector<DictUnit>& dictUnits)
{
    assert(dictUnits.size());
    vector<Unicode> words;
    vector<const Unicode*> valuePointers;

    for (size_t i = 0; i < dictUnits.size(); i++) {
        words.push_back(dictUnits[i].word);
        valuePointers.push_back(&dictUnits[i]);
    }

    Trie *trie = new Trie(words, valuePointers);
    return trie;
}

void DictTrie::loadUserDict_(const string& filePath, double defaultWeight, const string& defaultTag)
{
    ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
        printf("file %s open failed.\n", filePath.c_str());
    }

    string line;
    DictUnit nodeInfo;
    vector<string> buf;
    size_t lineno;

    for (lineno = 0; getline(ifs, line); lineno++) {
        buf.clear();
        split(line, buf, " ");

        if (buf.size() < 1) {
            printf("split [%s] result illegal\n", line.c_str());
        }

        if (TransCode::decode(buf[0], nodeInfo.word)) {
            printf("line[%u:%s] illegal.", lineno, line.c_str());
            continue;
        }

        if (nodeInfo.word.size() == 1) {
            userDictSingleChineseWord_.insert(nodeInfo.word[0]);
        }

        nodeInfo.weight = defaultWeight;
        nodeInfo.tag = (buf.size() == 2 ? buf[1] : defaultTag);
        nodeInfos_.push_back(nodeInfo);
    }

    printf("load userdict[%s] ok, lines[%u]\n", filePath.c_str(), lineno);
}

void DictTrie::loadDict_(const string& filePath)
{
    ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
        printf("file %s open failed.\n", filePath.c_str());
    }

    std::string line;
    std::vector<string> buf;

    DictUnit nodeInfos;
    for (size_t lineno = 0; getline(ifs, line); lineno++) {
        split(line, buf, " ");
        if (buf.size() != DICT_COLUMN_NUM) {
            printf("split result illegal, line: %s, result size: %u", line.c_str(), buf.size());
        }

        if (!TransCode::decode(buf[0], nodeInfos.word)) {
            printf("line[%u:%s] illegal.", lineno, line.c_str());
            continue;
        }

        nodeInfos.weight = atof(buf[1].c_str());
        nodeInfos.tag = buf[2];

        nodeInfos_.push_back(nodeInfos);
    }
}

double DictTrie::findMinWeight_(const vector<DictUnit>& nodeInfos) const
{
    double ret = MAX_DOUBLE;
    for (size_t i = 0; i < nodeInfos.size(); i++) {
        ret = std::min(nodeInfos[i].weight, ret);
    }

    return ret;
}

double DictTrie::findMaxWeight_(const vector<DictUnit>& nodeInfos) const
{
    double ret = MIN_DOUBLE;
    for (size_t i = 0; i < nodeInfos.size(); i++) {
        ret = max(nodeInfos[i].weight, ret);
    }

    return ret;
}

void DictTrie::calculateWeight_(vector<DictUnit>& nodeInfos) const
{
    double sum = 0.0;
    for (size_t i = 0; i < nodeInfos.size(); i++) {
        sum += nodeInfos[i].weight;
    }

    assert(sum);

    for (size_t i = 0; i < nodeInfos.size(); i++) {
        DictUnit& nodeInfo = nodeInfos[i];
        assert(nodeInfo.weight);
        nodeInfo.weight = std::log(double(nodeInfo.weight) / double(sum));
    }
}

void DictTrie::shrink_(vector<DictUnit>& units) const
{
    vector<DictUnit>(units.begin(), units.end()).swap(units);
}