#include "MD5HashFunc.h"
unsigned int MD5HashFunc::GetKey(std::string str) {
    const unsigned char* digest = MD5(str).getDigest(); // 16位 16字节
    unsigned int hash = 0;

    /**
     * 每四个字节构成一个32位整数
     * 将四个32位整数相加得到hash值
     */
    for (int i = 0; i < 4; i++) {
        hash += ((unsigned int)(digest[i * 4 + 3] & 0xFF) << 24)
			| ((unsigned int)(digest[i * 4 + 2] & 0xFF) << 16)
			| ((unsigned int)(digest[i * 4 + 1] & 0xFF) << 8)
			| ((unsigned int)(digest[i * 4 + 0] & 0xFF));
    }
    return hash;
}