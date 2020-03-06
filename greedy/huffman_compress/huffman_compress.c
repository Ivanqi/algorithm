#include "huffman_compress.h"

bool isFileExits(char *fileName) {
    FILE *fp;

    fp = fopen(fileName, "rb");
    if (NULL == fp) return false;

    fclose(fp);
    return true;
}

void showAlphaFreq(ALPHA_FREQ *alphaFreq, int alphaVariety) {
    int i;
    for (i = 0; i < alphaVariety; i++) {
        int ch = alphaFreq[i].alpha;
        // 字符超出了ASCII码表示范围
        if (ch > 127) {
            printf("[%d]: %d\n", ch, alphaFreq[i].freq);
        } else {
            printf("[%c]: %d\n", ch, alphaFreq[i].freq);
        }
    }
}

void showHuffmanTab(HUFFMAN_TAB *huffmanTab, int count) {
	int i;

	printf("%-4s %-4s %-4s %-6s %-6s %-6s %s\n", "下标", "字符", "频度", "左孩子", "右孩子", "visited", "code");
	for (i = 0; i < count; i++) {
		printf("%-5d %-4c %-5d %-6d %-7d %-4d %s\n",
            i,
            huffmanTab[i].alphaFreq.alpha,
            huffmanTab[i].alphaFreq.freq,
            huffmanTab[i].leftChild,
            huffmanTab[i].rightChild,
            huffmanTab[i].visited,
            (huffmanTab[i].code ? huffmanTab[i].code : "无"));
	}
}

void destoryHuffmanTab(HUFFMAN_TAB *huffmanTab, int alphaVariety) {
    int i;

    for (i = 0; i < alphaVariety; i++) {
        free(huffmanTab[i].code);
    }
    free(huffmanTab);
}

/**
 * 从sourceFileName 文件中读取字符串
 * feof()函数不是"文件结束标志"，而是一个“动作标志”函数
 * 表示的是上一次的文件读写操作是否成功
 * 刚刚打开文件，feof()函数的初始值为0，当上一次文件操作失败时，返回非0
 */
ALPHA_FREQ *getAlphaFreq(char *sourceFileName, int *alphaVariety) {
    int freq[256] = {0};
    int i;
    int index;

    ALPHA_FREQ *alphaFreq = NULL;
    FILE *fpIn;
    int ch;

    fpIn = fopen(sourceFileName, "rb");

    // 统计所有字符的频度
    ch = fgetc(fpIn);
    while (!feof(fpIn)) {
        freq[ch]++;
        ch = fgetc(fpIn);
    }
    fclose(fpIn);

    // 统计所有字符的种类
    for (i = 0; i < 256; i++) {
        if (freq[i]) {
            (*alphaVariety)++;
        }
    }

    alphaFreq = (ALPHA_FREQ *)calloc(sizeof(ALPHA_FREQ), *alphaVariety);
    for (i = index = 0; i < 256; i++) {
        if (freq[i]) {
            alphaFreq[index].alpha = i;
            alphaFreq[index].freq = freq[i];
            index++;
        }
    }
    return alphaFreq;
}

HUFFMAN_TAB *initHuffmanTab(ALPHA_FREQ *alphaFreq, int alphaVariety, int *hufIndex) {
    int i;
    HUFFMAN_TAB *huffmanTab = NULL;

    huffmanTab = (HUFFMAN_TAB *)calloc(sizeof(HUFFMAN_TAB), 2 * alphaVariety - 1);
    // huffmanTab 申请了 2 *  alphaVariety - 1大小空间，在这只用了alphaVariety个，还剩alphaVariety - 1个
    for(i = 0; i < alphaVariety; i++) {
        hufIndex[alphaFreq[i].alpha] = i;       //  把哈夫曼表中的字符和其对应的下标形成键值对，存到hufIndex中
        huffmanTab[i].alphaFreq = alphaFreq[i];
        huffmanTab[i].leftChild = huffmanTab[i].rightChild = -1;
        huffmanTab[i].visited = false;
        huffmanTab[i].code = (char *)calloc(sizeof(char), alphaVariety);
    }

    return huffmanTab;
}

void creatHuffmanTree(HUFFMAN_TAB *huffmanTab, int alphaVariety) {
    int i;
    int leftChild;
    int rightChild;

    // huffmanTab使用剩下的 alphaVariety - 1 个空洞
    for (i = 0; i < alphaVariety - 1; i++) {
        leftChild = getMinFreq(huffmanTab, alphaVariety + i);
        rightChild = getMinFreq(huffmanTab, alphaVariety + i);
        huffmanTab[alphaVariety + i].alphaFreq.alpha = '#';
        huffmanTab[alphaVariety + i].alphaFreq.freq = huffmanTab[leftChild].alphaFreq.freq + huffmanTab[rightChild].alphaFreq.freq;

        huffmanTab[alphaVariety + i].leftChild = leftChild;
        huffmanTab[alphaVariety + i].rightChild = rightChild;
        huffmanTab[alphaVariety + i].visited = false;
    }
}

// 在哈夫曼中找没有放过访问过的最小频度下标
int getMinFreq(HUFFMAN_TAB *huffmanTab, int count) {
    int index;
    int minIndex = NOT_INIT;

    for (index = 0; index < count; index++) {
        if (false == huffmanTab[index].visited) {
            if (NOT_INIT == minIndex || huffmanTab[index].alphaFreq.freq < huffmanTab[minIndex].alphaFreq.freq) {
                minIndex = index;
            }
        }
    }

    huffmanTab[minIndex].visited = true;
    return minIndex;
}

void makeHuffmanCode(HUFFMAN_TAB *huffmanTab, int root, int index, char *code) {
    if (huffmanTab[root].leftChild != -1 && huffmanTab[root].rightChild != -1) {
        code[index] = '1';
        makeHuffmanCode(huffmanTab, huffmanTab[root].leftChild, index + 1, code);
        code[index] = '0';
        makeHuffmanCode(huffmanTab, huffmanTab[root].rightChild, index + 1, code);
    } else {
        code[index] = 0;
        strcpy(huffmanTab[root].code , code);
    }
}

// 取最后一个字节的有效位数
int getlastValidBit(HUFFMAN_TAB *huffmanTab, int alphaVariety) {
    int sum = 0;
    int i;

    for (i = 0; i < alphaVariety; i++) {
        sum += strlen(huffmanTab[i].code) * huffmanTab[i].alphaFreq.freq;
        // 如果不执行这一步，当数据长度超过int的表示范围，就会出错
        sum &= 0xFF;        // 0xFF化为二进制位1111 1111 , 这样做sum始终是最后一个字节，8位
    }
    // 举例：若最后生成7位二进制，划分一个字节，那么，这一字节只有7位有效位，其余都是垃圾位
    // 我们只需要取出这个字节的那7个有效位，所以sum 和 8 取模就可以了
    // sum = sum % 8 <=> sum = sum & 0x7
    sum &= 0x7;
    return sum == 0 ? 8 : sum;
}

// hufIndex下标为字符的ASCII码,其值为该字符在哈夫曼表中的下表，形成键值。再把字符串转换为哈夫曼编码时可以实现快速查找
void huffmanEncoding(HUFFMAN_TAB *huffmanTab, char *sourceFileName, char *targetFileName, int *hufIndex, int alphaVariety, ALPHA_FREQ *alphaFreq) {

    FILE *fpIn;
    FILE *fpOut;
    int ch;
    unsigned char value;
    int bitIndex = 0;
    int i;
    char *hufCode = NULL;
    HUF_FILE_HEAD fileHead = {'h', 'f', 'm'};

    fpIn = fopen(sourceFileName, "rb");
    fpOut = fopen(targetFileName, "w+");
    

    fileHead.alphaVariety = (unsigned char) alphaVariety;
    fileHead.lastValidBit = getlastValidBit(huffmanTab, alphaVariety);

    /**
     * size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
     *  ptr -- 这是指向要被写入的元素数组的指针。
     *  size -- 这是要被写入的每个元素的大小，以字节为单位。
     *  nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
     *  stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。
     */

    // 给文件头部写入元数据
    fwrite(&fileHead, sizeof(HUF_FILE_HEAD), 1, fpOut);
    // 给元数据后写字符种类和频度，解压缩时需要用这些生成一模一样的的哈夫曼树
    fwrite(alphaFreq, sizeof(ALPHA_FREQ), alphaVariety, fpOut);

    ch = fgetc(fpIn);
    while (!feof(fpIn)) {
        hufCode = huffmanTab[hufIndex[ch]].code;
        // 把每个字符的哈夫曼编码一个一个过
        // 如果是字符串'0', 就转换成二进制的0
        // 如果是字符串'1', 就转换为二进制的1
        for (i = 0; hufCode[i]; i++) {
            if ('0' == hufCode[i]) {
                // value 为一个字节
                // 从第1位依次赋值，若大于八位(一个字节)了，就写入文件中
                CLR_BYTE(value, bitIndex);
            } else {
                SET_BYTE(value, bitIndex);
            }
            bitIndex++;
            if (bitIndex >= 8) {
                bitIndex = 0;
                fwrite(&value, sizeof(unsigned char), 1, fpOut);
            }
        }
        ch = fgetc(fpIn);
    }

    // 如果最后一次不满一个字节，依然需要写到文件中，注意：写入最后一个字节可能存在垃圾位
    if (bitIndex) {
        fwrite(&value, sizeof(unsigned char), 1, fpOut);
    }

    fclose(fpIn);
    fclose(fpOut);
}

void huffmanDecoding(HUFFMAN_TAB *huffmanTab, char *sourceFileName, char *targetFileName, int alphaVariety, HUF_FILE_HEAD fileHead) {
    int root = 2 * alphaVariety - 2;
    FILE *fpIn;
    FILE *fpOut;

    bool finished = false;
    unsigned char value;
    unsigned char outValue;

    int index = 0;
    long fileSize;
    long curLocation;

    fpIn = fopen(sourceFileName, "rb");
    fpOut = fopen(targetFileName, "wb");

    fseek(fpIn, 0L, SEEK_END);
    fileSize = ftell(fpIn);                                                            // 文件总长度 fileSize
    fseek(fpIn, sizeof(HUF_FILE_HEAD) + sizeof(ALPHA_FREQ) * alphaVariety, SEEK_SET);  // 略过前面的元数据，字符种类和频度
    curLocation = ftell(fpIn);

    // 从根出发, '1'向左子树走，'0'向右子树走，若到达叶子节点，输出叶子节点下标对应的字符。再回到根节点继续
    fread(&value, sizeof(unsigned char), 1, fpIn);

    while (!finished) {
        if (huffmanTab[root].leftChild == -1 && huffmanTab[root].rightChild == -1) {
            outValue = huffmanTab[root].alphaFreq.alpha;
            fwrite(&outValue, sizeof(unsigned char), 1, fpOut);
            if (curLocation >= fileSize && index >= fileHead.lastValidBit) break;

            root = 2 * alphaVariety - 2;
        }

        // 取出的一个字节从第一位开始看，'1'向左子树，'0'向右子树走
        // 若超过一个字节，8位，则需要读取下一个字节
        if (GET_BYTE(value, index)) {
            root = huffmanTab[root].leftChild;
        } else {
            root = huffmanTab[root].rightChild;
        }

        if (++index >= 8) {
            index = 0;
            fread(&value, sizeof(unsigned char), 1, fpIn);
            curLocation = ftell(fpIn);
        }
    }

    fclose(fpIn);
    fclose(fpOut);
}

HUF_FILE_HEAD readFileHead(char *sourceFileName) {
    HUF_FILE_HEAD fileHead;
    FILE *fp;

    fp = fopen(sourceFileName, "rb");
    // 读取压缩文件的头部元数据
    fread(&fileHead, sizeof(HUF_FILE_HEAD), 1, fp);
    fclose(fp);

    return fileHead;
}

ALPHA_FREQ *readAlphaFreq(char *sourceFileName, int *alphaVariety, HUF_FILE_HEAD fileHead) {
    
    ALPHA_FREQ *alphaFreq = NULL;
    FILE *fp;
    *alphaVariety = fileHead.alphaVariety;
    alphaFreq = (ALPHA_FREQ*) calloc(sizeof(ALPHA_FREQ), *alphaVariety);
   

    fp = fopen(sourceFileName, "rb");
    // 略过前头数据的字节
    fseek(fp, sizeof(HUF_FILE_HEAD), SEEK_SET);
    fread(alphaFreq, sizeof(ALPHA_FREQ), *alphaVariety, fp);
    
    fclose(fp);
    return alphaFreq;
}

int huffmanEncode(char *sourceFileName, char *targetFileName) {

    ALPHA_FREQ *alphaFreq = NULL;               // 统计字符及频度的数组
    int alphaVariety = 0;                       // 字符种类
    HUFFMAN_TAB *huffmanTab = NULL;             // 哈夫曼表
    char *code = NULL;                          // 存储字符的哈夫曼编码
    int hufIndex[256] = {0};                    // 下标为字符的ASCII码，其值为该字符在哈夫曼表中的下标

    // 第二个参数为源文件名
    if (!isFileExits(sourceFileName)) {
        printf("源文件(%s)不存在!\n", sourceFileName);
        return 0;
    }

    alphaFreq = getAlphaFreq(sourceFileName, &alphaVariety);
    // showAlphaFreq(alphaFreq, alphaVariety);

    huffmanTab = initHuffmanTab(alphaFreq, alphaVariety, hufIndex);
    creatHuffmanTree(huffmanTab, alphaVariety);
    code = (char *)calloc(sizeof(char), alphaVariety);
    makeHuffmanCode(huffmanTab, 2 * alphaVariety - 2, 0, code);
    // showHuffmanTab(huffmanTab, 2 * alphaVariety - 1);
    huffmanEncoding(huffmanTab, sourceFileName, targetFileName, hufIndex, alphaVariety, alphaFreq);

    destoryHuffmanTab(huffmanTab, alphaVariety);
    free(alphaFreq);
    free(code);

    return 0;
}

int huffmanDecode(char *sourceFileName, char *targetFileName) {

    ALPHA_FREQ *alphaFreq = NULL;               // 统计字符及频度的数组
    int alphaVariety = 0;                       // 字符种类
    HUFFMAN_TAB *huffmanTab = NULL;             // 哈夫曼表
    char *code = NULL;                          // 存储字符的哈夫曼编码
    int hufIndex[256] = {0};                    // 下标为字符的ASCII码，其值为该字符在哈夫曼表中的下标
    HUF_FILE_HEAD fileHead;

    if (!isFileExits(sourceFileName)) {
        printf("源文件(%s)不存在!\n", sourceFileName);
        return 0;
    }

    fileHead = readFileHead(sourceFileName);
    if (!(fileHead.flag[0] == 'h' && fileHead.flag[1] == 'f' && fileHead.flag[2] == 'm')) {
        printf("不可识别的文件格式\n");
    }

    alphaFreq = readAlphaFreq(sourceFileName, &alphaVariety, fileHead);
    // showAlphaFreq(alphaFreq, alphaVariety);

    huffmanTab = initHuffmanTab(alphaFreq, alphaVariety, hufIndex);
    creatHuffmanTree(huffmanTab, alphaVariety);
    code = (char *)calloc(sizeof(char), alphaVariety);
    makeHuffmanCode(huffmanTab, 2 * alphaVariety - 2, 0, code);
    // showHuffmanTab(huffmanTab, 2 * alphaVariety - 1);

    huffmanDecoding(huffmanTab, sourceFileName, targetFileName, alphaVariety, fileHead);

    destoryHuffmanTab(huffmanTab, alphaVariety);
    free(alphaFreq);
    free(code);

    return 0;
}