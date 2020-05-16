#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#define CH_SIZE 3   // 汉字大小linux为3，win为2
using namespace std;

struct trie_node
{
    trie_node()
    {
        count = 0;
    }
    int count;  // 表示以该汉字结束的字符串个数
    map<string, trie_node*> child;  // 键值为当前汉字，实值为后面汉字节点的指针
};

class trie
{
    public:
        trie();
        ~trie();
        void insert_str(string str);        // 插入字符串
        trie_node *search_str(string str);  // 查询字符串
        trie_node *search_str_pre(string str_pre);  // 查询字符串前缀
        void delete_str(string str);        // 删除字符串
        vector<string> get_str_pre(string str); // 返回所有前缀为str的字符串
        void clear();   // 清空
    private:
        void add_str(trie_node *root, string pre_str, vector<string> &ret); // 递归添加以pre_str为前缀的字符串到ret集合
    private:
        struct trie_node *root;
};

trie::trie() 
{
    root = new trie_node();
}

trie::~trie()
{

}

// 插入字符串
void trie::insert_str(string str)
{
    if (root == NULL || str == "") {
        return ;
    }

    trie_node *cur_node = root;

    for (int i = 0; i < str.size();) {
        string sub_str = str.substr(i, CH_SIZE);
        map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);

        if (iter == cur_node->child.end()) {    // 如果在map中没有找到则插入新节点
            trie_node *tmp_node = new trie_node();
            cur_node->child.insert(pair<string, trie_node*>(sub_str, tmp_node));
            cur_node = tmp_node;
        } else {                                // 如果找到了value即为下一个节点的指针
            cur_node = iter->second;
        }
        i = i + CH_SIZE;
    }
    cur_node->count++;
}

// 删除字符串
void trie::delete_str(string str) 
{
    trie_node *find_node = search_str(str);
    if (find_node) {
        find_node->count--;
    }
}

// 查询字符串前缀
trie_node *trie::search_str_pre(string str)
{
    if (str == "") {
        return root;
    }

    if (NULL == root) {
        return NULL;
    }

    trie_node *cur_node = root;

    int i;
    for (i = 0; i < str.size();) {
        string sub_str = str.substr(i, CH_SIZE);
        map<string, trie_node*>::iterator iter = cur_node->child.find(sub_str);

        if (iter == cur_node->child.end()) {
            return NULL;
        } else {
            cur_node = iter->second;
        }
        i = i + CH_SIZE;
    }

    if (i == str.size()) {
        return cur_node;
    } else {
        return NULL;
    }
}

// 查询字符串
trie_node *trie::search_str(string str)
{
    trie_node *find_pre_node = search_str_pre(str);

    if (find_pre_node != NULL) {
        if (find_pre_node->count == 0) {
            return NULL;
        } else {
            return find_pre_node;
        }
    }
}

// 清空
void trie::clear()
{
    queue<trie_node *>que;

    for (map<string, trie_node*>::iterator iter = root->child.begin(); iter != root->child.end(); iter++) {
        que.push(iter->second);
    }

    while (!que.empty()) {
        trie_node *del_node = que.front();
        for (map<string, trie_node*>::iterator iter = del_node->child.begin(); iter != del_node->child.end(); iter++) {
            que.push(iter->second);
        }
        que.pop();
        delete del_node;
    }
}

// 递归添加以pre_str为前缀的字符串到ret集合
void trie::add_str(trie_node *root, string pre_str, vector<string> &ret)
{
    for (map<string, trie_node*>::iterator iter = root->child.begin(); iter != root->child.end(); iter++) {
        add_str(iter->second, pre_str + iter->first, ret);
    }

    if (root->count != 0) {
        ret.push_back(pre_str);
    }
}

// 返回所有前缀为str的字符串
vector<string> trie::get_str_pre(string str)
{
    vector<string> ret;
    trie_node *find_node = search_str_pre(str);

    if (find_node != NULL) {
        add_str(find_node, str, ret);
    }

    return ret;
}

void test_case_1() {
    trie t;
    int n;
    string str;
    vector<string> ret;
    vector<string> strs = {
        "经济学", "计算机", "幼儿教育", "哲学", "管理学", "文学", "嵌入式", "大数据", "人工智能", "地质学",
        "数学", "数据", "数据集", "数据库", "数理化", "测试", "计量", "记事本", "计算器"
    };

    for (int i = 0; i < strs.size(); i++) {
        t.insert_str(strs[i]);
    }

    ret = t.get_str_pre(strs[18]);

    for (vector<string>::iterator iter = ret.begin(); iter != ret.end(); iter++) {
        cout << *iter << endl;
    }

    t.clear();
}

int main() {

    test_case_1();
    return 0;
}