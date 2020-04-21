#include <iostream>
#include <vector>
#include <queue>
#include <memory.h>
using namespace std;

class Graph // 有向无环图
{
    private:
        int v;  // 顶点个数
        vector<vector<int>> adj;    // 邻接表

    public:
        Graph(int num): v(num)
        {
            for (int i = 0; i < num;i++) {
                vector<int> tmp;
                adj.push_back(tmp); 
            }
        }

        void addEdge(int s, int t)  //  s先于t， 边s->t
        {
            if (s > v || t > v) {
                cout << "不能超出最大值" << endl;
                return;
            }
            adj[s].push_back(t);
        }

        /**
         * 时间复杂度: O(V + E). V顶点, E边的个数
         * 每个顶点被访问一次，每个边也都被访问了一次
         */
        void topoSortByKahn()
        {
            vector<int>inDegree(v, 0); // 统计每个顶点的入度
            for (int i = 0; i < v; ++i) {
                for (int j = 0; j < adj[i].size(); ++j) {
                    int w = adj[i][j];  // i -> w
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

        /**
         * 时间复杂度: O(V + E)
         * 每个顶点被访问两次，每条边都被访问一次
         */
        void topoSortByDFS()
        {
            // 先构建逆邻接表，边 s->t表示，s依赖于t, t先于s
            vector<vector<int>> inverseAdj;
            for (int i = 0; i < v; i++) {
                vector<int> tmp;
                inverseAdj.push_back(tmp); 
            }

            for (int i = 0; i < v; ++i) {
                for (int j = 0; j < adj[i].size(); ++j) {
                    int w = adj[i][j];              // i -> w
                    inverseAdj[w].push_back(i);     // w -> i
                }
            }

            bool visited[v];
            memset(visited, false, sizeof(visited));
            for (int i = 0; i < v; ++i) {          // 深度优先遍历图
                if (visited[i] == false) {
                    visited[i] = true;
                    dfs(i, inverseAdj, visited);
                }
            }
        }

        void dfs(int vertex, vector<vector<int>>& inverseAdj, bool *visited) 
        {
            // cout << "vertex:" << vertex << " | size:" << inverseAdj[vertex].size() << endl;
            for (int i = 0; i < inverseAdj[vertex].size(); ++i) {
                int w = inverseAdj[vertex][i];
                if (visited[w] == true) continue;   // 已经访问过了
                visited[w] = true;
                dfs(w, inverseAdj, visited);
            }
            // 先把vertex这个顶点可达的所有顶点都打印出来，再打印它自己
            cout << "->" << vertex << endl;
        }
};

void test_add_edge(Graph *g) {
    g->addEdge(0, 1);
    g->addEdge(1, 2);
    g->addEdge(1, 3);
    g->addEdge(2, 4);
    g->addEdge(4, 5);
    g->addEdge(5, 6);
    g->addEdge(6, 4);
}

void test_topoSortByKahn(Graph *g) {
    g->topoSortByKahn();
}

void test_topoSortByDFS(Graph *g) {
    g->topoSortByDFS();
}

int main() {
    int cap = 7;
    Graph *g = new Graph(cap);
    test_add_edge(g);

    test_topoSortByDFS(g);
    return 0;
}