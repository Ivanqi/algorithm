#include "CreateIndexFile.h"
#include "FileProcess.h"
#include "HashSearch.h"
#include "StringProcess.h"

#define strcpy_s strcpy
#define strncpy_s strncpy

static char filename[FILE_MAX_NUM][FILENAME_MAX_LEN] = {0};
static char words[WORD_MAX_NUM][WORD_MAX_NUM] = {0};
static char items[ITEM_MAX_NUM][WORD_MAX_LEN] = {0};
static char indexFile[] = "/../data/index.txt";
static char subDir[] = "/../data/12";

// 从行缓冲中得到各项信息，将其写入items数组(通过#####对字符串切割)
void GetItems(char *&move, int &count, int &wordnum) {

    char *front = move;
    bool flag = false;
    int len;

    // strstr()函数用来检索子串在字符串中首次出现的位置
    move = strstr(move, "#####");
    if (*(move + 5) == '#') {
        flag = true;
    }

    if (move) {
        len = move - front;
        // 复制字符串的前n个字符
        strncpy(items[count], front, len);
    }

    items[count][len] = '\0';
    count++;

    if (flag) {
        move = move + 10;
    } else {
        move = move + 5;
    }
}

// 保存关键字相应的文档内容
doc_list SaveItems() {

    char date[9];
    doc_list infolist = (doc_list)malloc(sizeof(DOCNODE));

    strcpy_s(infolist->id, items[0]);
    infolist->classOne = atoi(items[1]);

    strcpy_s(infolist->classTwo, items[2]);
    infolist->classThree = atoi(items[3]);

    strncpy_s(date, items[4], 8);
    date[8] = '\0';

    strncpy_s(infolist->time, date, 9);
    strcpy_s(infolist->md5, items[5]);
    infolist->weight = atoi(items[6]);

    return infolist;
}

void copyStr(char *from, char *to) {

    char *f = from;
    char *t = to;
    
    while ((*t++ = *f++) != '\0');
}

char* returnFullPath(char *currPath, char *otherPath) {

    char *returnPath = (char *)malloc(ABSPATH_MAX_LEN);
    copyStr(currPath, returnPath);
    strcat(returnPath, otherPath);
    return returnPath;
}

int main() {

    key_list keynode;
    char *pbuf, *move;
    FILE *frp;

    char currPath[128];
    getcwd(currPath, sizeof(currPath) - 1);

    char *dirPath = returnFullPath(currPath, subDir);
    char *indexFilePath = returnFullPath(currPath, indexFile);


    int filenum = GetFileName(dirPath, filename);
    pbuf = (char *)malloc(BUF_MAX_LEN);
    memset(pbuf, 0, BUF_MAX_LEN);

    HASHVALUE *hashvalue = (HASHVALUE *)malloc(sizeof(HASHVALUE));

    FILE *fwp = OpenWriteFile(indexFilePath);
    if (fwp == NULL) {
        return 0;
    }

    PrepareCryptTable();

    int wordnum = 0;
    for (int i = 0; i < filenum; i++) {
        frp = OpenReadFile(dirPath, i, filename);
        if (frp == NULL) {
            break;
        }

        // 每次读取一行处理
        while (fgets(pbuf, BUF_MAX_LEN, frp)) {
            int count = 0;
            move = pbuf;

            if (GetRealString(pbuf) <= 1) {
                continue;
            }

            while (move != NULL) {
                // 找到第一个非'#'的字符
                while (*move == '#') {
                    move++;
                }

                if (!strcmp(move, "")) {
                    break;
                }

                GetItems(move, count, wordnum);
            }

            /**
             * items下标0～6的内容包括: 文档ID、订阅源(子频道)、频道分类、网站类ID(大频道)、时间、md5、文档权重
             * items下标为7之后的词条进行处理
             *  如果当前词条不存在
             *      过hash获取hash index并写入key_array
             *      同时保存items下标为0～6的内容
             *  如果当前词条存在
             *      判断词条当前时间，根据时间由早到晚排序
             *      同时保存items下标为0～6的内容
             *      key_array 的 next 是一个当链表，用于处理hash冲突的情况
             */
            for (int i = 7; i < count; i++) {
                // 将关键字对应的文档内容加入文档结点链表中
                InitHashValue(items[i], hashvalue);

                if (keynode = SearchByString(items[i], hashvalue)) {
                    doc_list infonode = SaveItems();
                    doc_list p = keynode->next;

                    // 根据时间由早到晚排序
                    // 词条一致，但是时间无法对应上。那么使用链地法，时间早的在前面，时间晚的在后边
                    if (strcmp(infonode->time, p->time) > 0) {
                        // 考虑infonode插入keynode后的情况
                        infonode->next = p;
                        keynode->next = infonode;
                    } else {
                        // 考虑其他情况. 链表中的当前结点一样，那么找到链表中的下个结点，比较时间，时间早的插入在前边
                        doc_list pre = p;
                        p = p->next;

                        while (p) {
                            if (strcmp(infonode->time, p->time) < 0) {
                                p = p->next;
                                pre = pre->next;
                            } else {
                                break;
                            }
                        }

                        infonode->next = p;
                        pre->next = infonode;
                    }

                    keynode->count++;
                } else {
                    // 如果关键字第一次出现，则将其加入hash表
                    int pos = InsertString(items[i], hashvalue);
                    keynode = key_array[pos];

                    doc_list infolist = SaveItems();

                    infolist->next = NULL;
                    keynode->next = infolist;

                    if (pos != -1) {
                        // 存储词条信息
                        strcpy_s(words[wordnum++], items[i]);
                    }
                }
            }
        }
    }

    // 通过快排对关键字进行排序
    qsort(words, WORD_MAX_NUM, WORD_MAX_LEN, strcoll);

    // 遍历关键字数组，将关键字及其对应的文档内容写入文件中
    int rownum = 1;
    for (int i = 0; i < WORD_MAX_NUM; i++) {
        InitHashValue(words[i], hashvalue);
        keynode = SearchByString(words[i], hashvalue);

        if (keynode != NULL) {
            doc_list infolist = keynode->next;
            char date[9];

            int *count = new int[WORD_MAX_NUM];
            memset(count, 0, WORD_MAX_NUM);
            strcpy_s(date, infolist->time);
            int num = 0;

            // 得到单个日前的文档数目. 当前相同词条的个数
            for (int j = 0; j < keynode->count; j++) {
                if (strcmp(date, infolist->time) == 0) {
                    count[num]++;
                } else {
                    count[++num]++;
                }

                strcpy_s(date, infolist->time);
                infolist = infolist->next;
            }
            
            /**
             * num: 当前相同词条的个数(时间不一致)
             * rownum: 遍历的次数
             */
            fprintf(fwp, "%s %d %d\n", words[i], num + 1, rownum);
            WriteFile(keynode, num, fwp, count);
            rownum++;
        }
    }

    free(pbuf);
    free(hashvalue);
    fclose(fwp);
    free(dirPath);
    free(indexFilePath);

    return 0;
}