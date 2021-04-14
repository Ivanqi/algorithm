#include "Trie.h"

Trie::Trie(const vector<Unicode>&keys, const vector<const DictUnit*>& valuePointers)
{
    root_ = new TrieNode;
    createTrie_(keys, valuePointers);
    build_();   // 创建自动机
}

Trie::~Trie()
{
    if (root_) {
        deleteNode_(root_);
    }
}

const DictUnit* Trie::find(Unicode::const_iterator begin, Unicode::const_iterator end) const
{
    TrieNode::NextMap::const_iterator citer;
    const TrieNode *ptNode = root_;

    for (Unicode::const_iterator it = begin; it != end; it++) {
        // build automation
        assert(ptNode);
        if (NULL == ptNode->next || ptNode->next->end() == (citer = ptNode->next->find(*it))) {
            return NULL;
        }

        ptNode = citer->second;
    }

    return ptNode->ptValue;
}

void Trie::find(Unicode::const_iterator begin, Unicode::const_iterator end, vector<struct SegmentChar>& res)
{
    res.resize(end - begin);
    const TrieNode *now = root_;
    const TrieNode *node;

    // 如果只有 i < end - begin。编译器将会警告
    for (size_t i = 0; i < size_t(end - begin); i++) {
        Unicode::value_type ch = *(begin + i);
        res[i].uniCh = ch;
        assert(res[i].dag.empty());

        res[i].dag.push_back(pair<vector<Unicode>::size_type, const DictUnit*>(i, NULL));

        bool flag = false;

        //  rollback
        while (now != root_) {
            node = now->findNext(ch);
            if (node != NULL) {
                flag = true;
                break;
            } else {
                now = now->fail;
            }
        }

        if (!flag) {
            node = now->findNext(ch);
        }

        if (node == NULL) {
            now = root_;
        } else {
            now = node;
            const TrieNode *temp = now;

            while (temp != root_) {
                if (temp->ptValue) {
                    size_t pos = i - temp->ptValue->word.size() + 1;
                    res[pos].dag.push_back(pair<vector<Unicode>::size_type, const DictUnit*>(i, temp->ptValue));

                    if (pos == i) {
                        res[pos].dag[0].second = temp->ptValue;
                    }
                }

                temp = temp->fail;
                assert(temp);
            }
        }
    }
}

bool Trie::find(Unicode::const_iterator begin, Unicode::const_iterator end, DagType& res, size_t offset) const
{
    const TrieNode *ptNode = root_;
    TrieNode::NextMap::const_iterator citer;

    for (Unicode::const_iterator itr = begin; itr != end; itr++) {
        assert(ptNode);

        if (NULL == ptNode->next || ptNode->next->end() == (citer = ptNode->next->find(*itr))) {
            break;
        }

        ptNode = citer->second;

        if (ptNode->ptValue) {
            if (itr == begin && res.size() == 1) {
                res[0].second = ptNode->ptValue;
            } else {
                res.push_back(pair<vector<Unicode>::size_type, const DictUnit*>(itr - begin + offset, ptNode->ptValue));
            }
        }
    }

    return !res.empty();
}

void Trie::build_()
{
    queue<TrieNode*> que;
    assert(root_->ptValue == NULL);
    assert(root_->next);

    root_->fail = NULL;

    for (TrieNode::NextMap::iterator iter = root_->next->begin(); iter != root_->next->end(); iter++) {
        iter->second->fail = root_;
        que.push(iter->second);
    }

    TrieNode *back = NULL;
    TrieNode::NextMap::iterator backiter;

    while (!que.empty()) {
        TrieNode *now = que.front();
        que.pop();

        if (now->next == NULL) {
            continue;
        }

        for (TrieNode::NextMap::iterator iter = now->next->begin(); iter != now->next->end(); iter++) {
            back = now->fail;
            while (back != NULL) {
                if (back->next && (backiter = back->next->find(iter->first)) != back->next->end()) {
                    iter->second->fail = backiter->second;
                    break;
                }
                back = back->fail;
            }

            if (back == NULL) {
                iter->second->fail = root_;
            }

            que.push(iter->second);
        }
    }
}

void Trie::createTrie_(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers)
{
    if (valuePointers.empty() || keys.empty()) {
        return;
    }

    assert(keys.size() == valuePointers.size());

    for (size_t i = 0; i < keys.size(); i++) {
        insertNode_(keys[i], valuePointers[i]);
    }
}

void Trie::insertNode_(const Unicode& key, const DictUnit* ptValue)
{
    TrieNode *ptNode = root_;

    TrieNode::NextMap::const_iterator kmIter;

    for (Unicode::const_iterator citer = key.begin(); citer != key.end(); citer++) {
        if (NULL == ptNode->next) {
            ptNode->next = new TrieNode::NextMap;
        }

        kmIter = ptNode->next->find(*citer);

        if (ptNode->next->end() == kmIter) {
            TrieNode *nextNode = new TrieNode;
            nextNode->next = NULL;
            nextNode->ptValue = NULL;

            (*ptNode->next)[*citer] = nextNode;
            ptNode = nextNode;
        } else {
            ptNode = kmIter->second;
        }
    }

    ptNode->ptValue = ptValue;
}

void Trie::deleteNode_(TrieNode* node)
{
    if (!node) {
        return;
    }

    if (node->next) {
        TrieNode::NextMap::iterator it;
        for (it = node->next->begin(); it != node->next->end(); it++) {
            deleteNode_(it->second);
        }

        delete node->next;
    }

    delete node;
}