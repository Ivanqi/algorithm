#include <stack>
#include <iostream>
using namespace std;

stack<int> stack_push(stack<int> stack) {
    for (int i = 0; i < 5; i++) {
        stack.push(i);
    }
    return stack;
}

stack<int> stack_pop(stack<int> stack) {
    cout << "Pop: ";
    for (int i = 0; i < 5; i++) {
        int y = (int)stack.top();
        stack.pop();
        cout << (y) << endl;
    }
    return stack;
}

void stack_peek(stack<int> stack) {
    int element = (int)stack.top();
    cout << "Element on stack top : " << element << endl;
}

void stack_search(stack<int> stack, int element) {
    int pos = -1, co = 0;
    while (stack.size() > 0) {
        co++;
        if (stack.top() == element) {
            pos = co;
            break;
        }
        stack.pop();
    }

    if (pos == -1) {
        cout << "Element not found"  << endl;
    } else {
        cout << "Element is found at position: " << pos << endl;
    }
}


int main() {

    stack<int> stack;
    stack = stack_push(stack);
    stack = stack_pop(stack);
    stack = stack_push(stack);
    stack_peek(stack);
    stack_search(stack, 2);
    stack_search(stack, 6);

    return 0;
}
