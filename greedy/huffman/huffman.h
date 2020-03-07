#ifndef _HUFFMAN_H
#define _HUFFMAN_H

//得到index位的值，若index位为0，则GET_BYTE值为假，否则为真
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)
//index位置1
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))
//index位置0
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))

using uchar = unsigned char;
// #define uchar unsigned char

struct fileHead {
    char flag[4];                   // 压缩二进制文件头部标记
    uchar alphaVariety;             // 字符种类
    uchar lastValidBit;             // 最后一个字节的有效位数
    char unused[10];                // 待用空间
};

struct alpahFreq {
    uchar alpha;                    // 字符，考虑文件中有汉字，所以定义成uchar
    int freq;                       // 字符出现的频率
    alpahFreq() {}
    alpahFreq(const std::pair<char, int>& x): alpha(x.first), freq(x.second) {}
};

#endif