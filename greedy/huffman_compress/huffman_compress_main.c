#pragma pack(push)
#pragma pack(1)

#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

    if (argc != 4) {
        printf("正确的命令: huffman_compress_decode <mode[encode|decode]> <源文件名> <目标文件名>\n");
        return 0;
    }

    char sourceFileName[256] = {0};
    char targetFileName[256] = {0};

    strcpy(sourceFileName, argv[2]);
    strcpy(targetFileName, argv[3]);
    
    if (strcmp(argv[1], "encode") == 0) {
        return huffmanEncode(sourceFileName, targetFileName);
    } else if (strcmp(argv[1],  "decode") == 0) {
        return huffmanDecode(sourceFileName, targetFileName);
    } else {
        printf("mode 选择错误");
        return 0;
    }    
}

#pragma pack(pop)