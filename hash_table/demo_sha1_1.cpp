#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

/**
 * 参考资料
 * 1. https://blog.sbw.so/u/cpp-implements-class-for-sha1-encryption-algorithm.html
 * 2. https://www.codetd.com/article/8884694
 */
using namespace std;

// 调整移位
unsigned circleShift(const unsigned& word, const int& bits) {
    return (word << bits) | ((word) >> (32 - bits));
}

unsigned sha1Fun(const unsigned& B, const unsigned& C, const unsigned& D, const unsigned& t) {
    switch (t / 20) {
        case 0: return (B & C) | ((~B) & D);
        case 2: return (B & C) | (B & D) | (C & D);
        case 1:
        case 3: return B ^ C ^ D;
    }

    return t;
}

string sha1(const string& strRaw) {
    string str(strRaw);
    /**
     * 1. 补位
     *  先补一个1，然后再补0，直到长度满足对512取模后余数是448
     *  补位是至少补一位，最多补512位
     */
    // 因为C++没有byte类型，这里直接添加'0x80'即'10000000'
    str += (unsigned char)(0x80);
    // 每个字节8位，所以要乘8，左移3位
    while (str.size() << 3 % 512 != 448) {
        str += (char)0;
    }

    /**
     * 2. 补长度
     *   所谓的补长度是将原始数据的长度补到已经进行补位操作消息后面
     *   通常用一个64位的数据来表示原始消息的长度
     *   如果原始的消息长度超过了512，我们需要将它补成512的倍数
     *   然后我们把整个消息分成一个一个512位的数据块，分别处理每个数据块(即SHA一次最多加密64个字节，512位)，从而得到消息摘要(密文)
     */
    // 写入原始数据长度,每次写入8位，即一个字节
    for (int i(56); i >= 0; i-=8) {
        str += (unsigned char)((strRaw.size() << 3) >> i);
    }

    // 暂时不清楚这些常量的定义
    const unsigned K[4] = {0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};

    // 计算需要两个缓冲区，每个都由5个32位字节，还需要一个80个32位字的缓存区
    // 第一个6个字的缓冲区被标识为A, B, C, D, E, T
    unsigned A(0x67452301), B(0xefcdab89), C(0x98badcfe), D(0x10325476), E(0xc3d2e1f0), T(0);
    unsigned W[80] = {0};

    // 每次处理64字节，共512位
    for (unsigned i(0); i != str.size(); i+= 64) {
        // 以每32位分块，分成16位，放入字缓存W[0] ~ W[16]中
        for (unsigned t(0); t != 16; ++t) {
            // 将4个8位数据放入一个32位变量中
            W[t] = ((unsigned)str[i + 4 * t] & 0xff) << 24 |
                ((unsigned)str[i + 4 * t + 1] & 0xff) << 16 |
                ((unsigned)str[i + 4 * t + 2] & 0xff) << 8 |
                ((unsigned)str[i + 4 * t + 3] & 0xff);
        }

        // 填充
        // 将80字缓存w后64字填充
        for (unsigned t(16); t != 80; ++t) {
            W[t] = circleShift(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
        }

        /**
         * 填充完毕，从W[0]开始，进行80轮的加密计算
         */
        for (unsigned t(0); t != 80; ++t) {
            T = circleShift(A, 5) + sha1Fun(B, C, D, t) + E + W[t] + K[t / 20];
            E = D;
            D = C;
            C = circleShift(B, 30);
            B = A;
            A = T;
        }
        // 为5个缓冲区A,B,C,D,E加上它们的原始数据
        A += 0x67452301;
        B += 0xefcdab89;
        C += 0x98badcfe;
        D += 0x10325476;
        E += 0xc3d2e1f0;
    }

    stringstream ss;
    ss << setw(8) << setfill('0') << hex << A << B << C << D << E;
    ss >> str;
    return str;
}

int main (int argc, char *argv[]) {

    string str("");
 
    cout<<"in put a string :"<<endl;
    getline(cin,str);

    cout <<"raw  string: " << str << endl << "sha1 encode: " << sha1(str) << endl;
    return 0;
}