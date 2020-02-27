#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <./dmoe_sha256_1.h>

int main() {
    
    char text[] = "blackkitty";
    char sha256[65];

    StrSHA256(text, sizeof(text) - 1, sha256);  // sizeof()计算的结果包含了末尾'\0'应减1
    puts(sha256);
    puts(StrSHA256(text, sizeof(text) - 1, sha256));    // 函数返回值即sha256, 直接输出也可以
}