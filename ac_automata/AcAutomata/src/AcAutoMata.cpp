#include "AcAutoMata.h"

AcAutoMata::~AcAutoMata()
{
    if (root_) {
        deleteNode(root_);
    }
}

const TrieNode* AcAutoMata::find(Unicode::const_iterator begin, Unicode::const_iterator end) const
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

    return ptNode;
}

void AcAutoMata::buildFailurePointer()
{
    queue<TrieNode*> que;
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

void AcAutoMata::printfFailurePointer()
{
    queue<TrieNode*> que;
    assert(root_->next);

    for (TrieNode::NextMap::iterator iter = root_->next->begin(); iter != root_->next->end(); iter++) {
        que.push(iter->second);
    }

    while (!que.empty()) {
        TrieNode *now = que.front();
        que.pop();

        std::cout << now->word;

        if (now->fail == NULL) {
            continue;
        } else {
            std::cout << "| fail word:" << now->fail->word << std::endl;
        }

        if (now->next == NULL) {
            continue;
        }

        for (TrieNode::NextMap::iterator iter = now->next->begin(); iter != now->next->end(); iter++) {
            que.push(iter->second);
        }

    }
}

void AcAutoMata::insertNode(const Unicode& key)
{
    TrieNode *ptNode = root_;

    TrieNode::NextMap::const_iterator kmIter;

    int len = 0;

    for (Unicode::const_iterator citer = key.begin(); citer != key.end(); citer++) {
        if (NULL == ptNode->next) {
            ptNode->next = new TrieNode::NextMap;
        }

        kmIter = ptNode->next->find(*citer);
        len += calcUnicodeLen(citer);

        if (ptNode->next->end() == kmIter) {
            TrieNode *nextNode = new TrieNode;
            nextNode->word = (*citer);
            nextNode->next = NULL;

            (*ptNode->next)[*citer] = nextNode;
            ptNode = nextNode;
        } else {
            ptNode = kmIter->second;
        }
    }

    ptNode->isEnding = true;
    ptNode->length = len;
}

string AcAutoMata::match(Unicode::const_iterator begin, Unicode::const_iterator end, string matchStr, char replaceStr)
{
    TrieNode *ptNode = root_;
    string res;
    int i = 0;
    unordered_map<int, int> check;

    for (Unicode::const_iterator citer = begin; citer != end; citer++) {
        /**
         * 使用 fail指针对 p的值进行矫正
         * 比如查询到某个模式串, p 值不是指向root. 然后主串继续遍历，主串的值在p上匹配不到值，需要fail指针把p指向root
         */
        while (ptNode != NULL && ptNode != root_ && ptNode->next != NULL && ptNode->next->find(*citer) == ptNode->next->end()) {
            ptNode = ptNode->fail;  // 失败指针发挥作用的地方
        }

        if (ptNode != NULL && ptNode->next != NULL) {
            if (ptNode->next->find(*citer) != ptNode->next->end()) {
                ptNode = ptNode->next->find(*citer)->second;
            }
        } else {
            ptNode = NULL;
        }

        if (ptNode == NULL) ptNode = root_; // 没有匹配，从root开始重新匹配

        TrieNode *tmp = ptNode;
        int len = calcUnicodeLen(citer);
        while (tmp != NULL && tmp != root_) {
            if (tmp->isEnding == true) {
                int pos = i - tmp->length + len;
                std::cout << "匹配起始下标: " << "i: " << i << " | pos: " << pos << "; 长度: " << tmp->length << std::endl;
                check[pos] = tmp->length;
            }
            tmp = tmp->fail;
        }

        i += len;
    }

    return replaceFun(check, matchStr, replaceStr);
}

string AcAutoMata::replaceFun(unordered_map<int, int> check, string text, char replaceStr)
{
    unordered_map<int, int>::iterator it;
    for (it = check.begin(); it != check.end(); it++) {                
        text.replace(it->first, it->second, it->second, replaceStr);
    }
    return text;
}

void AcAutoMata::deleteNode(TrieNode* node)
{
    if (!node) {
        return;
    }

    if (node->next) {
        TrieNode::NextMap::iterator it;
        for (it = node->next->begin(); it != node->next->end(); it++) {
            deleteNode(it->second);
        }

        delete node->next;
    }

    delete node;
}

int AcAutoMata::calcUnicodeLen(Unicode::const_iterator uni)
{
    uint16_t ui = *uni;

    if (ui <= 0x7f) {
        return 1;
    } else if (ui <= 0x7ff) {
        return 2;
    } else {
        return 3;
    }
}