#include "huffencode.h"
// #include "huffdecode.h"
#include <string.h>
// #include <stdio.h>

using namespace huffman;
using namespace std;

void huffmanEncode(const char *sourceFileName, const char *targetFileName) {
    huffEncode he;
    he.encode(sourceFileName, targetFileName);
}

// void testHuffDecode(const std::string& filename) {
//     std::string destFilename = "de" + filename;
//     huffDecode hd;
//     hd.decode(filename.data(), destFilename.data());
// }


int main(int argc, char const *argv[]) {

    if (argc != 4) {
        cout << "正确的命令: huffman_compress_decode <mode[encode|decode]> <源文件名> <目标文件名>" << endl;
        return 0;
    }
    const char *sourceFileName;
    const char *targetFileName;

    sourceFileName = argv[2];
    targetFileName = argv[3];
    
    if (strcmp(argv[1], "encode") == 0) {
        huffmanEncode(sourceFileName, targetFileName);
    } else if (strcmp(argv[1],  "decode") == 0) {
        // return huffmanDecode(sourceFileName, targetFileName);
    } else {
        cout << "mode 选择错误" << endl;
        
    } 
    return 0;
}