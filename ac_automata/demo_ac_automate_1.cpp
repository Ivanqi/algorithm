#include <iostream>
#include <queue>
#include <assert.h>
using namespace std;
#define MAX_NUM 26

class AcNode
{
    public:
        char data;
        AcNode *children[MAX_NUM] = {nullptr};  // 字符集只包含a ~ z这26个字符
        bool isEndingChar;                      // 结尾字符为true
        int length;                             // 当isEndingChar = true时候，记录模式串长度
        AcNode *fail;                           // 失败指针
        AcNode(char w) 
        {
            this->data = w;
            this->isEndingChar = false;
            this->length = -1;
        }
};

class AcAutoMata
{
    private:
        void buildFailurePointer() 
        {
            queue<AcNode*> Q;
            root->fail = NULL;
            Q.push(root);
            
            while (!Q.empty()) {
                AcNode *p = Q.front();
                Q.pop();

                for (int i = 0 ; i < MAX_NUM; i++) {
                    AcNode *pc = p->children[i];
                    if (pc == NULL) continue;
                    if (p == root) {
                        p->fail = root;
                    } else {
                        AcNode *q = p->fail;
                        while (q != NULL) {
                            AcNode *qc = q->children[p->data - 'a'];
                            if (qc != NULL) {
                                pc->fail = qc;
                                break;
                            }
                            q = q->fail;
                        }
                        if (q == NULL) pc->fail = root;
                    }
                    Q.push(pc);
                }
            }
        }
    public:
        AcNode *root;
        AcAutoMata() {
            root = new AcNode('/');
        }

        void insert(string word) 
        {
            AcNode *p = root;

            for (int i = 0; i < word.length(); i++) {
                int index = word[i] - 'a';
                if (p->children[index] == NULL) {
                    p->children[index] = new AcNode(word[i]);
                }
                p = p->children[index];
            }
            p->isEndingChar = true;
            p->length = word.length();
        }

        void match(string text) // text是主串
        {
            int n = text.length();
            AcNode *p = root;

            for (int i = 0; i < n; ++i) {
                int idx = text[i] - 'a';
                while (p != NULL && p != root && p->children[idx] == NULL) {
                    p = p->fail;            // 失败指针发挥作用的地方
                }
                if (p != NULL) {
                    p = p->children[idx];
                }
                if (p == NULL) p = root;    // 如果没有匹配的，从root开始重新匹配
                AcNode *tmp = p;
                while (tmp != NULL && tmp != root) {       // 打印出可以匹配的模式串
                    if (tmp->isEndingChar == true) {
                        int pos = i - tmp->length + 1;
                        cout << "匹配起始下标: " << pos << "; 长度" << tmp->length << endl;
                    }
                    tmp = tmp->fail;
                }
                assert(p != NULL);
            }
        }
};

int main() {
    AcAutoMata *ac = new AcAutoMata;
    ac->insert("abcd");
    ac->insert("bcd");
    ac->insert("ce");

    ac->match("nameabcdcallmebcdyousurece");
    return 0;
}