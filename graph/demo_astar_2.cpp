#include <iostream>
#include <vector>
#include <limits.h>
#include <memory.h>
#include <assert.h>
#include <string.h>
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

class Vertex {
    public:
        int id;     // 顶点编号ID
        int dist;   // 从起始顶点到这个顶点的距离，也就是g(i)
        int f;      // 新增: f(i) = g(i) + h(i)
        int x;
        int y;

        Vertex(int id, int x ,int y): id(id), x(x), y(y)
        {
            f = INT_MAX;
            dist = INT_MAX;
        }
};

class PriorityQueue // 根据vertex.f 构建小顶堆
{   
    public:
        Vertex **nodes;
        int count;
        int cal = 0;
        int topIndex = 1;
        int *mapArea;
        PriorityQueue(int v):count(v + 1)
        {
            nodes = new Vertex*[count];
            assert(nodes != NULL);
            mapArea = new int(count);
            assert(mapArea != NULL);
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
            assert(nodes[topIndex] != NULL);
            cal--;
            adjustUp(cal, topIndex);
        }

        void add(Vertex *vertex)
        {
            assert(vertex != NULL);
            if (cal >= count) {
                cout << "数据已满，无法再添加数据" << endl;
                return;
            }
            cal++;
            cout << "cal:" << cal << endl;
            nodes[cal] = vertex;
            mapArea[vertex->id] = cal;
            int i = cal;
            adjustDown(i);
        }

        void update(Vertex *vertex)
        {
            assert(vertex != NULL);
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

        void clear() {
            cal = 0;
            for (int i = 0; i < count; i++) {
                delete [] nodes[i];
                nodes[i] = nullptr;
            }
            delete [] nodes;
            nodes = nullptr;
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
            while (i / 2 > 0 && nodes[i]->f < nodes[i / 2]->f) {
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

                if (left <= total && nodes[parent]->f > nodes[left]->f) maxPos = left;
                if (right <= total && nodes[maxPos]->f < nodes[right]->f) maxPos = right;

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
        Vertex **vertexes;
    public:
        Graph(int num):v(num)
        {
            for (int i = 0; i < num; i++) {
                vector<Edge> tmp;
                adj.push_back(tmp);
            }
            vertexes = new Vertex*[v];
        }
        ~Graph() {
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

        // 新增一个方法，添加顶点的坐标
        void addVetex(int id, int x, int y)
        {
            vertexes[id] = new Vertex(id, x, y);
        }

        int hManhattan(Vertex *startPos, Vertex *endPos)
        {
            return abs(startPos->x - endPos->x) + abs(startPos->y - endPos->y);
        }

        void astar(int s, int t)                                         // 从顶点s到顶点t的路径
        {  
            int *predecessor = new int(v);
            // 按照vertex的f值构建
            PriorityQueue *queue = new PriorityQueue(v);
            bool *inqueue = new bool(v);

            vertexes[s]->dist = 0;
            vertexes[s]->f = 0;
            queue->add(vertexes[s]);
            inqueue[s] = true;

            while (!queue->isEmpty()) {
                Vertex *minVertex = queue->poll();                      // 取堆顶元素并删除
                for (int i = 0; i < adj[minVertex->id].size(); ++i) {
                    Edge e = adj[minVertex->id][i];                     // 取出一条minVetex相连的边
                    Vertex *nextVertex = vertexes[e.tid];               // minVertex --> nextVertex

                    if (minVertex->dist + e.w < nextVertex->dist) {     // 更新next的dist, f
                        nextVertex->dist = minVertex->dist + e.w;
                        nextVertex->f = nextVertex->dist + hManhattan(nextVertex, vertexes[t]);
                        predecessor[nextVertex->id] = minVertex->id;

                        if (inqueue[nextVertex->id] == true) {
                            queue->update(nextVertex);
                        } else {
                            queue->add(nextVertex);
                            inqueue[nextVertex->id] = true;
                        }
                    }
                    
                    if (nextVertex->id == t) {   // 只要到达t就可以结束while了
                        queue->clear();          // 清空queue,才能退出while循环
                        break;
                    }
                }
            }

            // 输出路径
            print(s, t, predecessor);
            delete predecessor;
            delete inqueue;
        }

        void print(int s, int t, int *predecessor)
        {
            if (s == t) return;
            print(s, predecessor[t], predecessor);
            cout << "->" << t << endl;
        }
};

void test_add_edge(Graph *g) {
    g->addEdge(0, 1, 20);
    g->addEdge(0, 4, 60);
    g->addEdge(0, 5, 60);
    g->addEdge(0, 6, 60);
    g->addEdge(1, 2, 20);
    g->addEdge(2, 3, 10);
    // g->addEdge(3, 13, 30);
    // g->addEdge(13, 6, 50);
    g->addEdge(4, 8, 50);
    g->addEdge(5, 8, 70);
    g->addEdge(5, 9, 80);
    g->addEdge(5, 10, 50);
    g->addEdge(6, 7, 50);
    // g->addEdge(7, 11, 50);
    // g->addEdge(11, 10, 60);
    g->addEdge(8, 9, 50);
    g->addEdge(9, 10, 60);

}

void test_add_vetex(Graph *g) {
    g->addVetex(0, 320, 630);
    g->addVetex(1, 300, 630);
    g->addVetex(2, 280, 625);
    g->addVetex(3, 270, 630);
    g->addVetex(4, 320, 700);
    g->addVetex(5, 360, 620);
    g->addVetex(6, 320, 590);
    g->addVetex(7, 370, 620);
    g->addVetex(8, 350, 730);
    g->addVetex(9, 390, 690);
    g->addVetex(10, 400, 620);
}

int main() {
    int cap = 12;
    Graph *g = new Graph(cap);
    test_add_edge(g);
    test_add_vetex(g);

    g->astar(0, 10);
    delete g;
    return 0;
}