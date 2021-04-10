#ifndef OS_LINUX
#pragma once
#endif

#include "HashSearch.h"
#include <unistd.h>

// 得到目录下所有文件名. 这个函数 应该不处理子目录
int GetFileName(char *dirPath, char filename[][FILENAME_MAX_LEN]) {

    int filenum = 0;

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if ((dp = opendir(dirPath)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dirPath);
        return -1;
    }

    chdir(dirPath);
    while ((entry = readdir(dp)) != NULL) {
        if (!S_ISDIR(statbuf.st_mode)) {
            if( strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 || strcmp("gmon.out", entry->d_name) == 0) {
                continue;     
            }
            strcpy(filename[filenum++], entry->d_name);
        }
    }

    closedir(dp);
    return filenum;
}

// 以读方式打开文件，如果成功返回文件指针
FILE* OpenReadFile(char *dirPath, int index, char filename[][FILENAME_MAX_LEN]) {

    char *abspath = (char *)malloc(ABSPATH_MAX_LEN);

    chdir(dirPath); // 进入某个目录

    // char *strncat(char *dest, const char *src, size_t n) 把 src 所指向的字符串追加到 dest 所指向的字符串的结尾，直到 n 字符长度为止
    strncat(abspath, filename[index], FILENAME_MAX_LEN);
    FILE *fp = fopen(abspath, "r");

    if (fp == NULL) {
        printf("OpenReadFile open read file error!\n");
        return NULL;
    } else {
        return fp;
    }
}

// 以写方式打开，如果成功返回文件指针
FILE* OpenWriteFile(const char *in_file_path) {

    if (in_file_path == NULL) {
        printf("output file path error!\n");
        return NULL;
    }

    FILE *fp = fopen(in_file_path, "w+");
    if (fp == NULL) {
        printf("open write file error!\n");
    }

    return fp;
}

// 将索引信息写入文档
void WriteFile(key_list keylist, int num, FILE *fwp, int *count) {

    doc_list infolist = keylist->next;

    for (int i = 0; i <= num; i++) {
        fprintf(fwp, "%s %d %s %d %s %d %s %d#####", infolist->time, count[i], infolist->id, 
        infolist->classOne, infolist->classTwo, infolist->classThree, infolist->md5, infolist->weight);

        infolist = infolist->next;

        for (int k = 0; k < count[i] - 1; k++) {
            fprintf(fwp, "%s %d %s %d %s %d#####", infolist->id, infolist->classOne,
                    infolist->classTwo, infolist->classThree, infolist->md5, infolist->weight);

            infolist = infolist->next;
        }

        fprintf(fwp, "\n", NULL);
    }
}