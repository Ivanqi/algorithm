#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <assert.h>
using namespace std;

class Graph // 无向图
{
    private:
        int v;  // 顶点个数
        vector<vector<int>> adj;   // 邻接表
        vector<bool> visited;
        vector<int> prev;

        void print(int s, int t)
        {
            if (prev[t] != -1 && t != s) { 
                print(s, prev[t]); 
            }
            cout << t << " " << endl;
        }

    public:
        Graph(int num) :v(num)
        {
            for (int i = 0; i < num;i++) {
                visited.push_back(false);
                prev.push_back(-1);
                vector<int> tmp;
                adj.push_back(tmp); 
            }
        }

        void addEdge(int s, int d)  //  无向图一条边存两次
        {
            if (s > v || d > v) {
                cout << "不能超出最大值" << endl;
                return;
            }
            adj[s].push_back(d);
            adj[d].push_back(s);
        }

        void printGraph()
        {
            cout << "";
            for (int i = 0; i < adj.size(); i++) {
                cout << "\ni :" << i << "\n";
                vector<int> tmp = adj[i];
                for (int j = 0; j < tmp.size(); j++) {
                    cout << "\t j: " << tmp[j] << " ";
                }
            }
            cout << endl;
        }
        
        void bfs(int s, int t) 
        {
            if (s == t) return;
            visited[s] = true;
            queue<int> Q;
            Q.push(s);

            while (!Q.empty()) {
                int front = Q.front();
                Q.pop();
                for (int i = 0; i < adj[front].size(); i++) {
                    int p = adj[front][i];
                    // cout << "visited["<< p << "]: " << visited[p] << endl;
                    if (!visited[p]) {
                        prev[p] = front;
                        // cout << "prev["<< p << "]: " << prev[p] << endl;
                        if (p == t) {
                            print(s, t);
                        }
                        visited[p] = true;
                        Q.push(p);
                    }
                }
            }
        }

        void n_degree_friends_bfs(int s, int t, int degree)
        {
            if (s == t) return;
            visited[s] = true;
            queue<int> Q;
            Q.push(s);
            int degree_num = 0;

            vector<vector<int>> degree_adj;

            while (!Q.empty()) {
                if (degree_num == degree + 1) break;
                int front = Q.front();
                Q.pop();

                vector<int> tmp;
                for (int i = 0; i < adj[front].size(); i++) {
                    int p = adj[front][i];
                    if (!visited[p]) {
                        visited[p] = true;
                        Q.push(p);
                        tmp.push_back(p);
                    }
                }
                degree_adj.push_back(tmp);
                degree_num++;
            }
            
            for (int i = 0; i < degree_adj.size(); i++) {
                vector<int> tmp = degree_adj[i];
                for (int j = 0; j < tmp.size(); j++) {
                    cout << tmp[j] << " ";
                }
                cout << endl;
            }
        }

        void dfs(int s, int t)
        {
            if (s == t) return;
            visited[s] = true;
            stack<int> st;
            bool found = false;
            st.push(s);

            while (!st.empty()) {
                found = false;
                int v = st.top();
                // cout << "v_size:" << v << endl;
                for (int i = 0; i < adj[v].size(); i++) {
                    int p = adj[v][i];
                    if (!visited[p]) {
                        // cout << "p:" << p << endl;
                        visited[p] = true;
                        st.push(p);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << v << " ";
                    st.pop();
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

void test_bfs(Graph *g) {
    g->bfs(0, 7);
}

void test_n_degree_friends_bfs(Graph *g) {
    g->n_degree_friends_bfs(0, 7, 3);
}

void test_dfs(Graph *g) {
    g->printGraph();
    g->dfs(0, 7);
}


int main() {
    Graph *g = new Graph(8);

    test_add_edge(g);
    test_n_degree_friends_bfs(g);
    
}

