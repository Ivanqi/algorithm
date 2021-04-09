#ifndef OS_LINUX
#pragma once
#endif

#include "HashSearch.h"

#define OS_LINUX

// 得到目录下所有文件名. 这个函数 应该不处理子目录
int GetFileName(char filename[][FILE_MAX_NUM]) {

    int filenum = 0;
    char *dir = "./12";

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if ((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return -1;
    }

    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        if (!S_ISDIR(statbuf.st_mode)) {
            printf("%s\n", entry->d_name);
            strcpy(filename[filenum++], entry->d_name);
        }
    }

    closedir(dp);
    return filenum;
}