#include <iostream>
#include <queue>
#include <assert.h>
#include <map>
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
    public:
        void buildFailurePointer() 
        {
            queue<AcNode*> Q;
            root->fail = NULL;
            Q.push(root);
            
            while (!Q.empty()) {
                // 广度优先遍历
                AcNode *p = Q.front();
                Q.pop();
                if (p != NULL) {
                    // 将其孩子逐个加入列队
                    for (int i = 0 ; i < MAX_NUM; i++) {
                        AcNode *pc = p->children[i];
                        if (pc == NULL) continue;
                        if (p == root) {
                            pc->fail = root;                 // 第一层的节点fail总是指向root
                        } else {
                            AcNode *q = p->fail;            // 第二层以下的节点，其fail是在另一分支上
                            while (q != NULL) {
                                // 遍历它的孩子，看它们有没有与当前孩子相同字符的节点
                                AcNode *qc = q->children[pc->data - 'a'];
                                if (qc != NULL) {
                                    pc->fail = qc;
                                    break;
                                }
                                q = q->fail;
                            }

                            if (q == NULL) {
                                pc->fail = root;
                            }
                        }
                        Q.push(pc);
                    }
                }
            }
        }

        void printfFailurePointer()
        {
            queue<AcNode*> Q;
            Q.push(root);

            while(!Q.empty()) {
                AcNode *p = Q.front();
                Q.pop();

                cout << "data: " << p->data;
                if (p->fail == NULL) {
                    cout << endl;
                } else {
                    cout << " | fail: " << p->fail->data << endl;
                }

                for (int i = 0; i < MAX_NUM; i++) {
                    AcNode *pc = p->children[i];
                    if (pc == NULL) continue;
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

        string match(string text, string replaceStr) // text是主串
        {
            int n = text.length();
            AcNode *p = root;
            map<int, int> check;

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
                        cout << "匹配起始下标: " << pos << "; 长度: " << tmp->length << endl;
                        check[pos] = tmp->length;
                       
                    }
                    tmp = tmp->fail;
                }
                assert(p != NULL);
            }
            return replaceFun(check, text, replaceStr);
        }

        string replaceFun(map<int, int> check, string text, string replaceStr)
        {
            map<int, int>::iterator it;
            for (it = check.begin(); it != check.end(); it++) {
                string tmp;
                for (int i = 0; i < it->second; i++) {
                    tmp += replaceStr;
                }
                text.replace(it->first, it->second, tmp);
            }
            return text;
        }
};

void test_case_1() {
    AcAutoMata *ac = new AcAutoMata;
    ac->insert("abcd");
    ac->insert("bcd");
    ac->insert("ce");
    ac->buildFailurePointer();
    ac->printfFailurePointer();

    string str = "nameabcdcallmebcdyousurece";
    string ret = ac->match(str, "*");
    cout << "原字符串: " << str << endl;
    cout << "替换后为: " << ret << endl;
}

int main() {
    
    test_case_1();
    return 0;
}