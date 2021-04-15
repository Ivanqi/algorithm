#include "Trie.h"

Trie::Trie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers)
    :root_(new TrieNode)
{
    CreateTrie(keys, valuePointers);
}

Trie::~Trie()
{
    DeleteNode(root_);
}

const DictUnit* Trie::Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const
{
    if (begin == end) {
        return NULL;
    }

    const TrieNode *ptNode = root_;
    TrieNode::NextMap::const_iterator citer;

    for (RuneStrArray::const_iterator it = begin; it != end; it++) {
        if (NULL == ptNode->next) {
            return NULL;
        }
        citer = ptNode->next->find(it->rune);
        bool ret = (ptNode->next->end() == citer);
        if (ret) {
            return NULL;
        }

        ptNode = citer->second;
    }

    return ptNode->ptValue;
}

void Trie::Find(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<struct Dag>&res, size_t max_word_len) const
{
    assert(root_ != NULL);
    res.resize(end - begin);

    const TrieNode *ptNode = NULL;
    TrieNode::NextMap::const_iterator citer;

    for (size_t i = 0; i < size_t(end - begin); i++) {
        // 获得 unicode 的值
        res[i].runestr = *(begin + i);

        // 查找，遍历
        if (root_->next != NULL && root_->next->end() != (citer = root_->next->find(res[i].runestr.rune))) {
            ptNode = citer->second;
        } else {
            ptNode = NULL;
        }

        // 把获得的信息放入res中
        if (ptNode != NULL) {
            res[i].nexts.push_back(pair<size_t, const DictUnit*>(i, ptNode->ptValue));
        } else {
            res[i].nexts.push_back(pair<size_t, const DictUnit*>(i, static_cast<const DictUnit*>(NULL)));
        }

        for (size_t j = i + 1; j < size_t(end - begin) && (j - i + 1) <= max_word_len; j++) {
            if (ptNode == NULL || ptNode->next == NULL) {
                break;
            }

            citer = ptNode->next->find((begin + j)->rune);
            if (ptNode->next->end() == citer) {
                break;
            }

            ptNode = citer->second;
            if (NULL != ptNode->ptValue) {
                res[i].nexts.push_back(pair<size_t, const DictUnit*>(j, ptNode->ptValue));
            }
        }
    }
}

void Trie::InsertNode(const Unicode& key, const DictUnit* ptValue)
{
    if (key.begin() == key.end()) {
        return;
    }

    TrieNode::NextMap::const_iterator kmIter;
    TrieNode *ptNode = root_;

    for (Unicode::const_iterator citer = key.begin(); citer != key.end(); ++citer) {
        if (NULL == ptNode->next) {
            ptNode->next = new TrieNode::NextMap;
        }

        kmIter = ptNode->next->find(*citer);

        if (ptNode->next->end() == kmIter) {
            TrieNode *nextNode = new TrieNode;

            ptNode->next->insert(make_pair(*citer, nextNode));
            ptNode = nextNode;
        } else {
            ptNode = kmIter->second;
        }
    }

    assert(ptNode != NULL);
    ptNode->ptValue = ptValue;
}

void Trie::CreateTrie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers)
{
    if (valuePointers.empty() || keys.empty()) {
        return;
    }

    assert(keys.size() == valuePointers.size());

    for (size_t i = 0; i < keys.size(); i++) {
        InsertNode(keys[i], valuePointers[i]);
    }
}

void Trie::DeleteNode(TrieNode* node)
{
    if (NULL == node) {
        return;
    }

    if (NULL != node->next) {
        for (TrieNode::NextMap::iterator it = node->next->begin(); it != node->next->end(); ++it) {
            DeleteNode(it->second);
        }

        delete node->next;
    }
}