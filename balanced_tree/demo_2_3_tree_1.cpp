#include <iostream>
#include <ctime>
#include <stack>
#include <cstdlib>
using namespace std;

int new_counter = 0;
int del_counter = 0;

struct Node {
    int _data[3];       // one more data for split
    int _count;         // 1 or 2
    Node* _array[4];    // one more point for split
};

Node* init(int data) {
    Node* root = new Node;
    new_counter++;
    for (int i = 0; i < 3; i++) {
        root->_data[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        root->_array[i] = 0;
    }

    root->_count = 1;
    root->_data[0] = data;
}

void insert(Node* node, Node* parent, int data) {
    // 叶节点删除后留下的空节点
    if (node->_count == 0) {
        node->_data[0] = data;
        node->_count++;
        return;
    }

    if (node->_count == 1) {                // 如果当前节点为2节点，则可直接在当前节点上添加
        if (data < node->_data[0]) {        // 小于2节点中的数据，在左边添加，使其成为3节点
            if (node->_array[0] != 0) {     // 如果有子节点，则向下走
                insert(node->_array[0], node, data);
            } else {
                // 如果没有子节点，则在当前节点上操作
                int temp = node->_data[0];
                node->_data[0] = data;
                node->_data[1] = temp;
                node->_count++;
            }
        } else {    // 大于等于2节点中的数据，在右边添加，使其成为3节点
            if (node->_array[1] != 0) { // 如果有子节点，则向下走
                insert(node->_array[1], node, data);
            } else {
                node->_data[1] = data;
                node->_count++;
            }
        }
    } else {    // 如果当前为3节点
        if (data < node->_data[0]) {
            if (node->_array[0] != 0) {
                insert(node->_array[0], node, data);
            } else {
                // 有序插入
                int temp1 = node->_data[0];
                int temp2 = node->_data[1];
                node->_data[0] = data;
                node->_data[1] = temp1;
                node->_data[2] = temp2;
                node->_count++;
            }
        } else if (data < node->_data[1]) {
            if (node->_array[1] != 0) {
                insert(node->_array[1], node, data);
            } else {
                // 有序插入
                int temp1 = node->_data[1];
                node->_data[1] = data;
                node->_data[2] = temp1;
                node->_count++;
            }
        } else {
            if (node->_array[2] != 0) {
                insert(node->_array[2], node, data);
            } else {
                // 有序插入
                node->_data[2] = data;
                node->_count++;
            }
        }
    }

    // 分裂检测
    if (node->_count == 3) {
        // 构造2个新节点
        Node* node1 = new Node;
        node1->_data[0] = node->_data[0];
        node1->_count = 1;
        node1->_array[0] = node->_array[0];
        node1->_array[1] = node->_array[1];
        node1->_array[2] = 0;
        node1->_array[3] = 0;


        Node* node2 = new Node;
        node2->_data[0] = node->_data[2];
        node2->_count = 1;
        node2->_array[0] = node->_array[2];
        node2->_array[1] = node->_array[3];
        node2->_array[2] = 0;
        node2->_array[3] = 0;

        new_counter += 2;

        for (int i = 1; i < 3; i++) {
            node1->_data[i] = -1;
            node2->_data[i] = -1;
        }

        if (parent == 0) {  // 如果当前节点没有父节点，则增加一层
            // 降为2节点
            node->_data[0] = node->_data[1];
            for (int i = 1; i < 3; i++) {
                node->_data[i] = -1;
            }

            node->_count = 1;
            node->_array[0] = node1;
            node->_array[1] = node2;
            node->_array[2] = 0;
            node->_array[3] = 0;
        } else {    // 如果有父节点，则将分裂出的根插入父节点
            // 判断位置
            if (node->_data[1] < parent->_data[0]) {
                // 1 左边
                if (parent->_array[2] != 0) {
                    parent->_data[2] = parent->_data[1];
                    parent->_data[1] = parent->_data[0];
                    parent->_data[0] = node->_data[1];

                    parent->_array[3] = parent->_array[2];
                    parent->_array[2] = parent->_array[1];
                    parent->_array[1] = node2;
                    parent->_array[0] = node1;
                } else if (parent->_array[1] != 0) {
                    parent->_data[1] = parent->_data[0];
                    parent->_data[0] = node->_data[1];
                    parent->_array[2] = parent->_array[1];
                    parent->_array[1] = node2;
                    parent->_array[0] = node1;
                }
            } else if ((parent->_count > 1 && node->_data[1] < parent->_data[1]) || parent->_count == 1) {
                // 1 右边 2左边
                if (parent->_array[2] != 0) {
                    parent->_data[2] = parent->_data[1];
                    parent->_data[1] = node->_data[1];

                    parent->_array[3] = parent->_array[2];
                    parent->_array[2] = node2;
                    parent->_array[1] = node1;
                } else if (parent->_array[1] != 0) {
                    parent->_data[1] = node->_data[1];
                    parent->_array[2] = node2;
                    parent->_array[1] = node1;
                }
            } else if ((parent->_count > 2 && node->_data[1] < parent->_data[2]) || parent->_count == 2) {
                // 2 右边 3 左边
                if (parent->_array[2] != 0) {
                    parent->_data[2] = node->_data[1];

                    parent->_array[3] = node2;
                    parent->_array[2] = node1;
                }
            }

            parent->_count++;
            delete node;
            del_counter++;
         } 
    }
}

void PreOrderTranversal(Node* root) {
    if (root == 0) return;

    for (int i = 0; i < 2; i++) {
        if (root->_data[i] == -1) break;
        cout << root->_data[i] << ' ';
    }
    cout << " count: " << root->_count << endl;

    for (int i = 0; i < 3; i++) {
        if (root->_array[i] != 0) PreOrderTranversal(root->_array[i]);
    }
}

void InOrderTranversal(Node* root) {
    if (root == 0) return;

    for (int i = 0; i < 3; i++) {
        if (root->_array[i] != 0) {
            InOrderTranversal(root->_array[i]);
            if (i < 2 && root->_data[i] != -1) {
                cout << root->_data << endl;
            }
        }
    }

    if (root->_array[0] == 0) {
        for (int i = 0; i < 2; i++) {
            if (root->_data[i] != -1) cout << root->_data[i] << endl;
        }
    }
}

void PostOrderTranversal(Node* root) {
    for (int i = 0; i < 3; i++) {
        if (root->_array[i] != 0) {
            PostOrderTranversal(root->_array[i]);
            delete root->_array[i];
            del_counter++;
        }
    }
}

int find_counter = 0;
void find(Node* root, int data) {
    find_counter++;
    if (root == 0) {
        cout << "i can't find it !" << endl;
        return;
    }

    int pos = 0;
    for (int i = 0; i < 2; i++) {
        if (root->_data[i] == -1) break;

        if (root->_data[i] == data) {
            cout << "i find data: " << data << " at " << find_counter << " times!" << endl;
        }

        if (root->_data[i] < data) pos = i + 1;
    }

    find(root->_array[pos], data);
}


void del_proc(Node* ori, Node* root, int pos, int counter) {
    counter++;
    if (root->_array[0] == 0) { // 要删除的节点为叶节点
        ori->_data[pos] = root->_data[0];
        root->_data[0] = -1;
        if (root->_count == 2) {
            root->_data[0] = root->_data[1];
            root->_data[1] = -1;
        }
        root->_count--;
        cout << "del ok!" << endl;
        return;
    } else {
        // 若不是叶节点，就递归找到后继
        if (counter == 0) {
            del_proc(ori, root->_array[pos + 1], pos, counter);
        } else {
            del_proc(ori, root->_array[0], pos, counter);
        }
    }
}

void del(Node* root, int data) {
    if (root == 0) cout << "Not Exist!" << endl; return;

    int pos = 0;
    for (int i = 0; i < 2; i++) {
        if (root->_data[i] == -1) break;

        if (root->_data[i] == data) {
            cout << "i: " << i << endl;
            del_proc(root, root, i, 0);
            return;
        }

        if (root->_data[i] < data) pos = i + 1;
    }
    del(root->_array[pos], data);
}

int main (int argc, char* argv[]) {
    srand(time(NULL));

    int times = 100;
    int find_num = 101;

    if (argc > 1) times = atoi(argv[1]);
    if (argc > 2) times = atoi(argv[2]);

    Node* root = init(101);
    for (int i = 0; i < times; i++) {
        insert(root, 0, rand() % 1000);
    }

    PreOrderTranversal(root);

    del(root, find_num);
    find(root , find_num);


    // delete
    PostOrderTranversal(root);
    delete root;
    del_counter++;

    cout << new_counter << ':' << del_counter << endl;
    cout << "finish!!" << endl;

    return 0;
}