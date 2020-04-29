#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

struct Point {
    Point() {}
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    int x = 0;
    int y = 0;

    bool operator == (const Point &pos) {
        return this->x == pos.x && this->y == pos.y;
    }
};

struct Data {
    Data() {}
    Data(Point pos, double h, double g, Data *parent) {
        this->pos = pos;
        this->h = h;
        this->g = g;
        this->parent = parent;
    }

    Point pos = Point(0, 0);
    double h = 0;
    double g = 0;
    Data *parent = nullptr;

    double f() {
        return this->g + this->h;
    }
};

struct MinHeap {
    vector<Data*> m_vec;
    map<string, Data*> m_map;
    int index = 0;

    Data *getMinAndRemove() {
        if (isEmpty()) {
            return nullptr;
        }

        sort();
        auto first = m_vec.at(0);
        auto last = m_vec.at(index - 1);
        m_vec[0] = last;
        --index;
        return first;
    }

    bool isEmpty() {
        return index <= 0;
    }

    string getKey(Point pos) {
        char buff[32] = {0};
        sprintf(buff, "%d-%d", pos.x, pos.y);
        return buff;
    }

    Data *find(Point pos) {
        auto it = m_map.find(getKey(pos));
        if (it != m_map.end()) {
            return it->second;
        }
        return nullptr;
    }

    void add(Data *data) {
        if (index < m_vec.size()) {
            m_vec[index] = data;
        } else {
            m_vec.push_back(data);
        }
        index = m_vec.size();
        m_map[getKey(data->pos)] = data;
    }

    void sort() {
        std::sort(m_vec.begin(), m_vec.end(), [](Data *a, Data *b){return a->f() < b->f(); });
    }

    void release() {
        for (auto it = m_map.begin(); it != m_map.end();) {
            Data *tmp = it->second;
            it = m_map.erase(it);
            delete tmp;
        }
    }
};

typedef double (*distance_func)(Point *startPos, Point *endPos);
// 曼哈顿距离，小于等于实际值
// 曼哈顿距离: 横向+纵向的距离
double hManhattanDistance(Point *startPos, Point *endPos) {
    return abs(startPos->x - endPos->x) + abs(startPos->y - endPos->y);
}

// 欧式距离,小于等于实际值
// 欧式距离:两点间的直线距离
double hEuclidianDistance(Point *startPos, Point *endPos) {
    return sqrt(pow(startPos->x - endPos->x, 2) + pow(startPos->y - endPos->y, 2));
}

// 欧式距离平方,大于等于实际值
double hPowEuclidianDistance(Point *startPos, Point *endPos) {
    return pow(startPos->x - endPos->x, 2) + pow(startPos->y - endPos->y, 2);
}

// BFS的h值,恒为0
double hBFS(Point *startPos, Point *endPos) {
    return 0;
}


class AStar
{
    private:
        const char START = 'S';
        const char END = 'E';
        const char VISITED = '-';
        const char SPACE = '.';
        Point startPos;
        Point endPos;
        char arr[30][30] = {{0}};
        int index = 0;
        int max_l = 0;
        //地图字符串
        const string s_arr[10] = {
            "....................",
            "....................",
            "...II.......IIIII...",
            ".....I......I....I..",
            ".....I......I..E....",
            ".....I......I....I..",
            "...S.I......IIIII...",
            "...II...............",
            "....................",
            "....................",
        };
    public:
        
        // 用地图字符串产生地图数据
        void genMap() {
            for (int i = 0; i < 10; ++i) {
                string str = s_arr[i];
                index = 0;

                for (int j = 0; j < str.size(); ++j) {
                    char c = str.at(j);
                    if (j != ' ') {
                        arr[i][j] = c;
                        if (c == START) {
                            startPos = Point(i, index);
                        } else if (c == END) {
                            endPos = Point(i, index);
                        }
                        ++index;
                    }
                }
                if (max_l == 0) {
                    max_l = index;
                }
            }
        }

        void search(distance_func distanceFunc) {
            // 可以拓展的四个方向
            int directs[][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

            MinHeap heap;
            heap.add(new Data(startPos, 0, 0, nullptr));

            bool finish = false;
            Data *lastData = nullptr;       // 记录最后一个点的数据，用来反推路径，若为空则无路径可到达

            while (!finish && !heap.isEmpty()) {
                Data *data = heap.getMinAndRemove();            // 取出f值最小的点
                if (arr[data->pos.x][data->pos.y] == SPACE) {
                    arr[data->pos.x][data->pos.y] = VISITED;    // 将取出的点标识为已访问点
                }

                for (auto dir:directs) {    // 遍历四个方向的点
                    Point newPos = Point(data->pos.x + dir[0], data->pos.y + dir[1]);
                    if (newPos.x > 0 && newPos.x < 10 && newPos.y > 0 && newPos.y < max_l) {
                        char c = arr[newPos.x][newPos.y];
                        if (c == END) {     // 如果是终点，则跳出循环，不用再找
                            finish = true;
                            lastData = data;
                            break;
                        }

                        if (c != SPACE) {   // 如果不是空地，就不需要再扩展
                            continue;
                        }

                        auto inQueueData = heap.find(newPos);
                        if (inQueueData) {           // 如果在堆内，则更新g值
                            if (inQueueData->g > data->g + 1) {
                                inQueueData->g = data->g + 1;
                                inQueueData->parent = data;
                            }
                        } else {
                            double h = distanceFunc(&newPos, &endPos);
                            heap.add(new Data(newPos, h, data->g + 1, data));
                        }
                    }
                }
            }

            if (lastData) {
                // 反向找出路径
                while (lastData->parent) {
                    arr[lastData->pos.x][lastData->pos.y] = '@';
                    lastData = lastData->parent;
                }
            }
            heap.release();
        }

        // 打印地图
        void printMap() {
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < max_l; ++j) {
                    cout << arr[i][j];
                }
                cout << endl;
            }
        }
};

void test_case_ManhattanDistance(AStar aS) {
    printf("\n --开始搜索: 曼哈顿距离-- \n");
    
    aS.search(hManhattanDistance);
    aS.printMap();
}

void test_case_EuclidianDistancee(AStar aS) {
    printf("\n --开始搜索: 欧式距离-- \n");
    
    aS.search(hEuclidianDistance);
    aS.printMap();
}

void test_case_PowEuclidianDistance(AStar aS) {
    printf("\n --开始搜索: 欧式距离平方-- \n");
    
    aS.search(hPowEuclidianDistance);
    aS.printMap();
}

void test_case_BFS(AStar aS) {
    printf("\n --开始搜索: BFS-- \n");
    
    aS.search(hBFS);
    aS.printMap();
}

int main() {

    AStar aS;
    aS.genMap();
    aS.printMap();
    test_case_ManhattanDistance(aS);
    test_case_EuclidianDistancee(aS);
    test_case_PowEuclidianDistance(aS);
    test_case_BFS(aS);
   
    return 0;
}