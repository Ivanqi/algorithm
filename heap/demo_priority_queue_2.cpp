#include <cstdio>
#include <string>
#include <queue>
using namespace std;

/**
 *
 */
int main() {
    char com[20];
    // 使用标准库中的priority_queue
    priority_queue<int> PQ;

    while (1) {
        scanf("%s", com);
        if (com[0] == 'i') {
            int key;
            scanf("%d", &key);  // 使用比cin速度更快的scanf
            PQ.push(key);
        } else if (com[1] == 'x') {
            printf("%d\n", PQ.top());
            PQ.top();
        } else if (com[0] == 'e') {
            break;
        }
    }
    return 0;
}