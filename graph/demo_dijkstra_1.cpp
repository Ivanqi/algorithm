#include <iostream>
#include <vector>
#include <limits.h>
#include <memory.h>
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
        int *mapArea;
        PriorityQueue(int v):count(v + 1)
        {
            nodes = new Vertex*[v + 1];
            mapArea = new int(v + 1);
        }

        Vertex *poll()
        {
            Vertex *topElem = top();
            pop();
            if (topElem) {
                mapArea[topElem->id] = 0;
            }
            return topElem;
        }

        Vertex *top()
        {
            return nodes[topIndex];
        }

        void pop()
        {
            swap(topIndex, cal);
            cal--;
            adjustUp(cal, topIndex);
        }

        void add(Vertex *vertex)
        {
            if (cal >= count) {
                cout << "数据已满，无法再添加数据" << endl;
                return;
            }
            cal++;
            nodes[cal] = vertex;
            mapArea[vertex->id] = cal;
            int i = cal;
            adjustDown(i);
        }

        void update(Vertex *vertex)
        {
            int vertexId = vertex->id;
            int index = mapArea[vertexId];
            if (index == 0) {
                cout << "无法找到对应的顶点" << endl;
                return;
            }
            cout << "update vertex.id:" << vertex->id << endl;
            nodes[index] = vertex;
            adjustDown(index);
        }

        bool isEmpty()
        {
            if (cal == 0) return true;
            else return false;
        }
    private:
        void swap(int a, int b) 
        {
            Vertex *tmp = nodes[a];
            nodes[a] = nodes[b];
            nodes[b] = tmp;
        }

        void adjustDown(int i)  // 自下而上
        {
            while (i / 2 > 0 && nodes[i]->dist < nodes[i / 2]->dist) {
                swap(i, i / 2);
                i = i / 2;
            }
        }

        void adjustUp(int total, int parent) // 自上而下堆化
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

        /**
         * while 循环最多会执行V次(V表示顶点的个数)，而内部的for循环的执行次数不确定，跟每个顶点的相邻边的个数有关，可以记做E0, E1, E2, ..., E(V - 1)
         * for 循环内部代码涉及从优先级队列取数据，往优先级队列中添加数据，更新优先级队列等三个操作时间复杂度为O(logV)
         * 综合两部分：时间复杂度O(E * logV)
         */
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

void test_add_edge(Graph *g) {
    g->addEdge(0, 1, 10);
    g->addEdge(0, 4, 15);
    g->addEdge(1, 2, 15);
    g->addEdge(1, 3, 2);
    g->addEdge(3, 5, 12);
    g->addEdge(2, 5, 5);
    g->addEdge(4, 5, 10);
}

int main() {
    int cap = 6;
    Graph *g = new Graph(cap);
    test_add_edge(g);

    g->dijkstra(0, 5);
    
    return 0;
}