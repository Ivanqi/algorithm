#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Graph // 有向无环图
{
    private:
        int v;  // 顶点个数
        vector<vector<int>> adj;    // 邻接表

    public:
        Graph(int num): v(num)
        {
            adj = vector<vector<int>>(num, vector<int>(num, 0));
        }

        void addEdge(int s, int t)  //  s先于t， 边s->t
        {
            if (s > v || t > v) {
                cout << "不能超出最大值" << endl;
                return;
            }
            adj[s].push_back(t);
        }

        void topoSortByKahn()
        {
            vector<int>inDegree(v, 0); // 统计每个顶点的入度
            for (int i = 0; i < v; ++i) {
                for (int j = 0; j < adj[i].size(); ++j) {
                    int w = adj[i][j];  // i -> w
                    if (w == 0) continue;
                    inDegree[w]++;
                }
            }

            queue<int> Q;
            for (int i = 0; i < v; ++i) {
                if (inDegree[i] == 0) Q.push(i);
            }

            while (!Q.empty()) {
                int i = Q.front();
                Q.pop();
                cout << "->" << i << endl;
                for (int j = 0; j < adj[i].size(); ++j) {
                    int k = adj[i][j];
                    inDegree[k]--;
                    if (inDegree[k] == 0) Q.push(k);
                }
            }
        }
};

void test_add_edge(Graph *g) {
    g->addEdge(0, 1);
    g->addEdge(0, 3);
    g->addEdge(1, 2);
    g->addEdge(1, 4);
    g->addEdge(3, 4);
    g->addEdge(2, 5);
    g->addEdge(4, 5);
    g->addEdge(4, 6);
    g->addEdge(5, 7);
    g->addEdge(6, 7);
}

void test_topoSortByKahn(Graph *g) {
    g->topoSortByKahn();
}

int main() {
    Graph *g = new Graph(8);

    test_add_edge(g);
    test_topoSortByKahn(g);
    return 0;
}