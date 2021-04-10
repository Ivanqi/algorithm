// 去除多余符号
int GetRealString(char *pbuf) {

    int len = strlen(pbuf) - 1;
    // 去除 退格、垂直制表符、空格，换行符
    while (len > 0 && (pbuf[len] == (char)0x0d || pbuf[len] == (char)0x0a || pbuf[len] == ' ' || pbuf[len] == '\t')) {
        len--;
    }

    if (len < 0) {
        *pbuf = '\0';
        return len;
    }

    pbuf[len + 1] = '\0';
    return len + 1;
}

// 若字符串 str1 和 str2 相同则返回0。若 str1 大于 str2 则返回大于 0 的值，否则返回小于 0 的值。
int strcoll(const void *s1, const void *s2) {

    char *c_s1 = (char *)s1;
    char *c_s2 = (char *)s2;
    
    while (*c_s1 == *c_s2++) {
        if (*c_s1++ == '\0') {
            return 0;
        }
    }

    return *c_s1 - *--c_s2;
}