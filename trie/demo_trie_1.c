#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_NUM 26

typedef struct TrieNode {
    char data;
    int isEndingChar;
    struct TrieNode **children;
}TrieNode;


// 往Trie树中插入一个字符串
TrieNode* create(char data) {
   TrieNode *trie = (TrieNode *)malloc(sizeof(TrieNode));
   assert(trie != NULL);
   trie->data = data;
   trie->isEndingChar = 0;
   trie->children = (TrieNode **)malloc(sizeof(TrieNode *) * MAX_NUM);
   assert(trie->children != NULL);

   return trie;
}

void insert(TrieNode *trie, char *text) {
    assert(trie != NULL);
    int textLen = strlen(text);
    int i;
    TrieNode *p = trie;
    for (i = 0; i < textLen; i++) {
        int index = text[i] - 'a';
        if (p->children[index] == NULL) {
            TrieNode *new = create(text[i]);
            p->children[index] = new;
        }
        p = p->children[index];
    }
    p->isEndingChar = 1;
}

// 在Trie树中查找一个字符串
int find(TrieNode *trie, char *pattren) {
    assert(trie != NULL);
    TrieNode *p = trie;
    int pLen = strlen(pattren);
    int i;

    for (i = 0; i < pLen; i++) {
        int index = pattren[i] - 'a';
        if (p->children[index] == NULL) {
            return 0;
        }
        p = p->children[index];
    }

    if (p->isEndingChar == 0) return 0;
    else return 1;
}

int main() {
    TrieNode *trie = create('/');;
    
    assert(trie != NULL);

    insert(trie, "how");
    insert(trie, "hi");
    insert(trie, "her");
    insert(trie, "hello");
    insert(trie, "so");
    insert(trie, "see");

    int ret = find(trie, "hi");
    printf("ret:%d\n", ret);

    ret = find(trie, "h");
    printf("ret:%d\n", ret);

    ret = find(trie, "he");
    printf("ret:%d\n", ret);

    ret = find(trie, "so");
    printf("ret:%d\n", ret);
    return 0;
}