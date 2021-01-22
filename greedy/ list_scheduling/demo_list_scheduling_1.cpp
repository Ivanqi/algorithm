#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

#define size 200

struct node {
    int op;         // add or mult
    int op1;
    int op2;
    int result;     // output register
} input[size];

struct readylist {
    int state;          // list state
    int op;             // previos指令输出寄存器
    int operation;      // 上一条指令是add or mult
    int num;
} list[size];

//  不同周期的可以alu树立那个
struct alu {
    int mult;
    int add;
} alulist[size];

// get data
int In() {
    fstream in;
    int op = 0, op1 = 0, op2 = 0, result = 0, count = 0, end;
    char filename[20];

    printf("\nEnter Input filename(ex: DFG1.txt)");
    scanf("%s", filename);
    in.open(filename);

    if (!in) {
        printf("can not open this filename\n");
        exit(1);
    }

    in >> end;  // 调用数量

    while (in >> op >> op1 >> op2 >> result) {
        input[count].op = op;           // 操纵符 add or mult
        input[count].op1 = op1;         // 寄存器1
        input[count].op2 = op2;         // 寄存器2
        input[count].result = result;   // 结果寄存器
        count++;
    }

    in.close();
    return end;
}

// sort the cycle for output
int compare(const void *a, const void *b) {
    int c = *(int *) a;
    int d = *(int *) b;

    if (c < d) {
        return -1;
    } else if (c == d) {
        return 0;
    } else {
        return 1;
    }
}

void list_scheduling(int end) {
    int state, opmult, opadd;
    state = 0;

    // input number of alus
    printf("\nEnter number of mult alu:");
    scanf("%d", &opmult);

    printf("Enter number of add alu:");
    scanf("%d", &opadd);

    state = 1;

    // 设置不同周期的可用alu数量
    for (int i = 0; i < size; i++) {
        alulist[i].add = opadd;
        alulist[i].mult = opmult;
    }

    // 检测数据的依赖性
    for (int i = 0; i < size; i++) {
        int j;
        for (j = i; j >= 0; j--) {
            // data dependency
            if (input[i].op1 == list[j].op || input[i].result == list[j].op || input[i].op2 == list[j].op) {
                break;
            }
        }

        if (j < 0) {
            state = 1;
        } else {
            if (list[j].operation == 1) {
                state = list[j].state + 1;  // if add means next state can use the date
            } else if (list[j].operation == 2) {
                state = list[j].state + 2;  // if mult means next next state can use the date
            }
        }

        if (input[i].op == 1) {
            while (alulist[state].add <= 0) {
                state++;                    // if no usable go next state
            }
        } else {
            while (alulist[state].mult <= 0) {
                state++;                    // if no usable alu go next state
            }
        }

        list[i].state = state;
        list[i].op = input[i].result;
        list[i].operation = input[i].op;
        list[i].num = i;

        if (input[i].op == 1) {
            alulist[state].add--;   // if add means now state add alu -1
        } else {
            alulist[state].mult--;  // if mult means now state and next state mult alu -1
            alulist[state + 1].mult--;
        }
    }

    qsort(list, end, sizeof(readylist), compare);

    // print output
    printf("\n*     Mult Constranit:%d      *\n", opmult);
    printf("*       Add Constraint:%d       *\n", opadd);

    for (int i = 0; i < end; i++) {
        int j = list[i].num;
        printf("cycle:%d ", list[i].state);

        if (list[i].state < 10) {
            printf(" ");
        }

        printf("r%d=r%d", input[j].result, input[j].op1);

        if (input[j].op == 1) {
            printf("+");
        } else {
            printf("*");
        }

        printf("r%d\n", input[j].op2);
    }
}

int main() {

    int end;
    end = In();
    list_scheduling(end);
    return 0;
}
