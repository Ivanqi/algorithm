#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
using namespace std;

/**
 * @brief SDBM Hash Function 
 * @detail 本算法是由于在开源项目SDBM（一种简单的数据库引擎）中被应用而得名，它与BKDRHash思想一致，只是种子不同而已
 */
size_t SDBMHash(const char *str) {
    register size_t hash = 0;  
    while (size_t ch = (size_t)*str++)  {  
        hash = 65599 * hash + ch;         
    }  
    return hash;  
}

/**
 * @brief RS Hash Function  
 * @detail 因Robert Sedgwicks在其《Algorithms in C》一书中展示而得名。  
 */
size_t RSHash(const char *str) {
    register size_t hash = 0;  
    size_t magic = 63689;     
    while (size_t ch = (size_t)*str++) {  
        hash = hash * magic + ch;  
        magic *= 378551;  
    }  
    return hash;  
}

/**
 * @brief JS Hash Function  
 * 由Justin Sobel发明的一种hash算法。  
 */
size_t JSHash(const char *str) {
     if(!*str)        // 这是由本人添加，以保证空字符串返回哈希值0  
        return 0;  
    register size_t hash = 1315423911;  
    while (size_t ch = (size_t)*str++) {  
        hash ^= ((hash << 5) + ch + (hash >> 2));  
    }  
    return hash; 
}

/**
 * @brief PJW Hash Function  
 * @detail 本算法是基于AT&T贝尔实验室的Peter J. Weinberger的论文而发明的一种hash算法。
 */
size_t PJWHash(const char *str) {
    if (!*str) return 0;

    unsigned int hash = 1315423911;
    while (unsigned int ch = (unsigned int)*str++) {
        hash ^= ((hash << 5) + ch + (hash >> 2));
    }

    return hash;
}
/**
 * @brief AP Hash Function 
 * @detail 由Arash Partow发明的一种hash算法。  
 */
size_t APHash(const char *str) {
    static const size_t TotalBits       = sizeof(size_t) * 8;  
    static const size_t ThreeQuarters   = (TotalBits  * 3) / 4;  
    static const size_t OneEighth       = TotalBits / 8;  
    static const size_t HighBits        = ((size_t)-1) << (TotalBits - OneEighth);      
          
    register size_t hash = 0;  
    size_t magic = 0;     
    while (size_t ch = (size_t)*str++) {  
        hash = (hash << OneEighth) + ch;  
        if ((magic = hash & HighBits) != 0) {  
            hash = ((hash ^ (magic >> ThreeQuarters)) & (~HighBits));  
        }  
    }  
    return hash;  
}
/**
 * @brief DJB Hash Function  
 * @detail 由Daniel J. Bernstein教授发明的一种hash算法。  
 */
size_t DJBHash(const char *str) {
    if(!*str)   // 这是由本人添加，以保证空字符串返回哈希值0  
        return 0;  
    register size_t hash = 5381;  
    while (size_t ch = (size_t)*str++) {  
        hash += (hash << 5) + ch;  
    }  
    return hash; 
}

/**
 * @brief ELF Hash Function  
 * @detail 由于在Unix的Extended Library Function被附带而得名的一种hash算法，它其实就是PJW Hash的变形。  
 */
size_t ELFHash(const char *str) {

    static const size_t TotalBits       = sizeof(size_t) * 8;  
    static const size_t ThreeQuarters   = (TotalBits  * 3) / 4;  
    static const size_t OneEighth       = TotalBits / 8;  
    static const size_t HighBits        = ((size_t)-1) << (TotalBits - OneEighth);      
    register size_t hash = 0;  
    size_t magic = 0;  
    while (size_t ch = (size_t)*str++)  {  
        hash = (hash << OneEighth) + ch;  
        if ((magic = hash & HighBits) != 0) {  
            hash ^= (magic >> ThreeQuarters);  
            hash &= ~magic;  
        }         
    }  
    return hash;  
}

/**
 * @brief BKDR Hash Function  
 * @detail 本 算法由于在Brian Kernighan与Dennis Ritchie的《The C Programming Language》一书被展示而得名
 * 是一种简单快捷的hash算法，也是Java目前采用的字符串的Hash算法（累乘因子为31）。  
 */
size_t BKDRHash(const char *str) {
    
    register size_t hash = 0;  
    while (size_t ch = (size_t)*str++) {         
        hash = hash * 131 + ch;   // 也可以乘以31、131、1313、13131、131313..  
        // 有人说将乘法分解为位运算及加减法可以提高效率，如将上式表达为：hash = hash << 7 + hash << 1 + hash + ch;  
        // 但其实在Intel平台上，CPU内部对二者的处理效率都是差不多的，  
        // 我分别进行了100亿次的上述两种运算，发现二者时间差距基本为0（如果是Debug版，分解成位运算后的耗时还要高1/3）；  
        // 在ARM这类RISC系统上没有测试过，由于ARM内部使用Booth's Algorithm来模拟32位整数乘法运算，它的效率与乘数有关：  
        // 当乘数8-31位都为1或0时，需要1个时钟周期  
        // 当乘数16-31位都为1或0时，需要2个时钟周期  
        // 当乘数24-31位都为1或0时，需要3个时钟周期  
        // 否则，需要4个时钟周期  
        // 因此，虽然我没有实际测试，但是我依然认为二者效率上差别不大          
    }  
    return hash; 
}

class Bloomfilter
{
    public:
        /**
         * 注意计算得到的哈希函数个数k需要不大于hashtable 的size
         * double err_rate 期望的失误率
         * int num 样本个数
         * char* path 传入样本的文档路径
         */
        Bloomfilter(double err_rate, int num, char* path);
        ~Bloomfilter();

        bool is_contain(const char* str);   // 查看字符串是否在样本中存在
        int hashnum();                      // 返回k
        int sizeofpool();                   // 返回len
        void filter_init();                 // 初始化布隆过滤器
    private:
        void listinit();                    // 打开path路径的文档，计算每一行样本到内存bitpool中
        int hashtable_init();               // 把几个哈希函数加入到vector<unsigned(*) (const char*)> hashtable容齐中，必须大于k
        int len;
        char* mypath;                       // 文件路径，通过构造函数传入路径

        Bloomfilter() = delete;
        double precision;
        int *bitpool;                       // 需要内存的长度，在构造函数中申请
        int bitpoollen;                     // 需要的二进制位数m
        int hashfuncnum;                    // 需要的哈希函数的个数k， k <= hashtable.size()
        int samplenum;                      // 样本个数，构造函数传入
        vector<size_t (*) (const char*)> hashtable;       // 存放计算字符串哈希值的哈希函数
};

double lg2(double n) {
    return log(n) / log(2);
}

int Bloomfilter::hashtable_init()
{
    hashtable.push_back(*BKDRHash);
    hashtable.push_back(*SDBMHash);
    hashtable.push_back(*RSHash);
    hashtable.push_back(*APHash);
    hashtable.push_back(*JSHash);
    hashtable.push_back(*PJWHash);
    hashtable.push_back(*DJBHash);
    hashtable.push_back(*ELFHash);
    return hashtable.size();
}

Bloomfilter::Bloomfilter(double err_table, int num, char *path)
{
    mypath = path;
    samplenum = num;
    bitpoollen =- ((samplenum * log(err_table)) / (log(2) * log(2)));
    hashfuncnum = 0.7 * (bitpoollen / samplenum);
    len = bitpoollen / 32 + 1;
    // cout << "bitpoollen:" << bitpoollen << " | samplenum: " << samplenum  << " | hashfuncnum:" << hashfuncnum  << " | len:" << len << endl;
    bitpool = new int[len];
}

int Bloomfilter::hashnum()
{
    return hashfuncnum;
}

int Bloomfilter::sizeofpool()
{
    return len;
}

void Bloomfilter::filter_init()
{
    hashtable_init();
    if (hashfuncnum > hashtable.size()) {
        cout << "哈希表中函数不足，请添加" << endl;
        exit(0);
    }
    listinit();
}

bool Bloomfilter::is_contain(const char* str)
{
    int hashval;
    for (int i = 0; i != hashfuncnum; i++) {
        hashval = hashtable[i](str);
        if (hashval < 0) continue;
        hashval = hashval % (len * 32);
        if (bitpool[hashval / 32] & (0x1 << (hashval % 32))) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

void Bloomfilter::listinit()
{
    FILE* fp;
    char* buf;
    size_t length = 0;
    fp = fopen(mypath, "r+");

    int hashval;
    char *p;

    while (getline(&buf, &length, fp) != EOF) {
        p = buf;
        while (*p != '\n') {
            p++;
        }
        *p = '\0';

        for (int i = 0; i != hashfuncnum; i++) {
            hashval = hashtable[i](buf);
            // cout << "hsahval:" << hashval << " | buf:" << buf << endl;
            if (hashval < 0) continue;
            hashval = hashval % (len * 32);
            bitpool[hashval / 32] |= (0x1 << (hashval % 32));
        }
    }
    fclose(fp);
}

Bloomfilter::~Bloomfilter() {
    // delete [] bitpool;   // 暂时注释， bl 在函数调用时候会出现错误，原因暂时不明
}

void showTestDetail(Bloomfilter mybloom, char* str) {
    FILE* fp;
    char* buf;
    size_t length = 0;
    fp = fopen(str, "r+");

    char *p;
    int errorNum = 0;
    int successNum = 0;
    int totalNum = 0;

    while (getline(&buf, &length, fp) != EOF) {
        p = buf;
        while (*p != '\n') {
            p++;
        }
        *p = '\0';
        if (mybloom.is_contain(buf)) {
            successNum++;
        } else {
            errorNum++;
        }
        totalNum++;
    }
    fclose(fp);

    printf("测试个数: %d, 成功数:%d, 失败数: %d\n", totalNum, successNum, errorNum);
    printf("成功率: %.2lf, 失败率: %.2lf\n", (double) successNum / totalNum, (double)errorNum / totalNum);
}


void test_case_1(double err_table) {

    char str[] = "./test_data/redlist.txt";
    Bloomfilter mybloom(err_table, 14064, str);
    mybloom.filter_init();

    cout << "需要的哈希函数的个数: " << mybloom.hashnum() << endl;
    cout << "需要申请多少个int: " << mybloom.sizeofpool() << endl;

    showTestDetail(mybloom, str);
}

void test_case_2(double err_table) {

    char str[] = "./test_data/word-list-large.txt";
    char str2[] = "./test_data/word_list.txt";
    Bloomfilter mybloom(err_table, 64131, str);
    mybloom.filter_init();

    cout << "需要的哈希函数的个数: " << mybloom.hashnum() << endl;
    cout << "需要申请多少个int: " << mybloom.sizeofpool() << endl;

    showTestDetail(mybloom, str2);
}

void test_case_3(double err_table) {
    char str[] = "./test_data/word-list-extra-large.txt";
    char str2[] = "./test_data/word-list-large.txt";
    Bloomfilter mybloom(err_table, 192425, str2);
    mybloom.filter_init();

    cout << "需要的哈希函数的个数: " << mybloom.hashnum() << endl;
    cout << "需要申请多少个int: " << mybloom.sizeofpool() << endl;

    showTestDetail(mybloom, str);
}

int main() {
    
    double err_table = 0.01;
    test_case_3(err_table);
   
    return 0;;
}