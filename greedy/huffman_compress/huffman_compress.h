#ifndef __HUFFMAN_COMPRESS__
#define __HUFFMAN_COMPRESS__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// 得到index位的值，若index位为0，则GET_BYTE值为假，否则为真
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)

// index位置1
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))

// index位置0
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))

// typedef unsigned char boolean;

// #define TRUE 1
// #define FALSE 0
#define NOT_INIT -1

typedef struct HUF_FILE_HEAD {
    unsigned char flag[3];              // 压缩二进制文件头部标识 ycy
    unsigned char alphaVariety;         // 字符种类
    unsigned char lastValidBit;         // 最后一个字节的有效空间
    unsigned char unused[1];            // 待用空间
} HUF_FILE_HEAD;                        // 这个结构体总共占用16个字节的空间 

typedef struct ALPHA_FREQ {
    unsigned char alpha;                // 字符，考虑到文件中有汉字，所以定义成unsigned char
    int freq;                           // 字符出现的频率
} ALPHA_FREQ;

typedef struct HUFFMAN_TAB {
    ALPHA_FREQ alphaFreq;
    int leftChild;
    int rightChild;
    bool visited;
    char *code;
} HUFFMAN_TAB;

bool isFileExist(char *fileName);
ALPHA_FREQ *getAlphaFreq(char *sourceFileName, int *alphaVariety);
void showAlphaFreq(ALPHA_FREQ *alphaFreq, int alphaVariety);
HUFFMAN_TAB *initHuffmanTab(ALPHA_FREQ *alphaFreq, int alphaVariety, int *hufIndex);
void destoryHuffmanTab(HUFFMAN_TAB *huffmanTab, int count);
int getMinFreq(HUFFMAN_TAB *huffmanTab, int count);
void creatHuffmanTree(HUFFMAN_TAB *huffmanTab, int alphaVariety);
void makeHuffmanCode(HUFFMAN_TAB *huffmanTab, int root, int index, char *code);
void huffmanEncodindg(HUFFMAN_TAB *huffmanTab, char *sourceFileName, char *targetFileName, int *hufIndex, int alphaVariety, ALPHA_FREQ *alphaFreq);
int getlastValidBit(HUFFMAN_TAB *huffmanTab, int alphaVariety);
void showHuffmanTab(HUFFMAN_TAB *huffmanTab, int count);

#endif