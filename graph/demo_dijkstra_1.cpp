#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;

class Edge {
    public:
        int sid;    // 边的起始编号
        int tid;    // 边的终止顶点编号
        int w;      // 权重
        Edge(int sid, int tid, int w): sid(sid), tid(tid),w(w)
        {

        }
};

// 下面这个类是为了dijkstra实现用的
class Vertex {
    public:
        int id;     // 顶点编号ID
        int dist;   // 从起始顶点到这个顶点的距离
        Vertex(int id, int dist):id(id), dist(dist)
        {

        }
};

class PriorityQueue // 根据vertex.dist 构建小顶堆
{   
    public:
        Vertex **nodes;
        int count;
        int cal = 0;
        int topIndex = 1;
        PriorityQueue(int v):count(v + 1)
        {
            nodes = new Vertex*[v + 1];
        }

        Vertex *poll()
        {
            Vertex *topElem = top();
            pop();
            return topElem;
        }

        Vertex *top()
        {
            return nodes[topIndex];
        }

        void pop()
        {
            swap(topIndex, count);
            cal--;
            heapify(cal, topIndex);
        }

        void add(Vertex *vertex)
        {
            update(vertex);
        }

        void update(Vertex *vertex)
        {
            if (cal >= count) {
                cout << "数据已满，无法再添加数据" << endl;
                return;
            }

            cal++;
            nodes[cal] = vertex;
            int i = cal;

            while (i / 2 > 0 && nodes[i]->dist < nodes[i / 2]->dist) {
                swap(i, i / 2);
                i = i / 2;
            }
        }

        bool isEmpty()
        {
            if (cal == 0) return true;
            else return false;
        }
    private:
        void swap(int a, int b) 
        {
            Vertex *tmp = this->nodes[a];
            this->nodes[a] = this->nodes[b];
            this->nodes[b] = tmp;
        }

        void heapify(int total, int parent) // 自上而下堆化
        {   
            int maxPos, left ,right;

            while (1) {
                maxPos = parent;
                left = parent * 2;
                right = left + 1;

                if (left <= total && nodes[parent]->dist > nodes[left]->dist) maxPos = left;
                if (right <= total && nodes[maxPos]->dist < nodes[right]->dist) maxPos = right;

                if (maxPos == parent) break;
                swap(parent, maxPos);
                parent = maxPos;
            }
        }
};

class Graph {
    private:
        int v;  // 顶点数
        vector<vector<Edge>> adj;
    public:
        Graph(int num):v(num)
        {
            for (int i = 0; i < num; i++) {
                vector<Edge> tmp;
                adj.push_back(tmp);
            }
        }

        void addEdge(int s, int t, int w) // 添加一条边
        { 
            if (s > v || t > v) {
                cout << "不能超出最大值" << endl;
                return;
            }
            Edge e(s, t, w); 
            adj[s].push_back(e);
        }

        void dijkstra(int s, int t)             // 从顶点s到顶点t的最短路径
        {          
            int *predecessor = new int(v);      // 用来还原最短路径
            Vertex **vertexes = new Vertex*[v]; // 记录从起始顶点到每个顶点的距离(dist)

            for (int i = 0; i < v; ++i) {
                vertexes[i] = new Vertex(i, INT_MAX);
            }

            PriorityQueue *queue = new PriorityQueue(v);    // 小顶堆
            bool *inqueue = new bool(v);                    // 定义是否进入过队列

            vertexes[s]->dist = 0;
            queue->add(vertexes[s]);
            inqueue[s] = true;

            while (!queue->isEmpty()) {
                Vertex *minVertex = queue->poll();          // 取堆顶元素并删除
                if (minVertex->id == t) break;              // 最短路径产生了

                for (int i = 0; i < adj[minVertex->id].size(); ++i) {
                    Edge e = adj[minVertex->id][i];         // 取出一条minVetex相连的边
                    Vertex *nextVertex = vertexes[e.tid];   // minVertex->nextVertex

                    if (minVertex->dist + e.w < nextVertex->dist) {     // 更新next 的dist
                        nextVertex->dist = minVertex->dist + e.w;
                        predecessor[nextVertex->id] = minVertex->id;

                        if (inqueue[nextVertex->id] == true) {
                            queue->update(nextVertex);                  // 更新队列中的dist值
                        } else {
                            queue->add(nextVertex);
                            inqueue[nextVertex->id] = true;
                        }
                    }
                }
            }
            cout << "s: " << s << endl;
            print(s, t, predecessor);
        }

        void print(int s, int t, int *predecessor)
        {
            if (s == t) return;
            print(s, predecessor[t], predecessor);
            cout << "->" << t << endl;
        }
};

int main() {

    return 0;
}