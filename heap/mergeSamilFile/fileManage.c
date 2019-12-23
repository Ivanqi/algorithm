#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fileManage.h>

FileInfo* file_create(int num) {
    assert(num > 0);
    FileInfo *file;
    file = (FileInfo *)malloc(sizeof(FileInfo));
    file->capitiry = num;
    int count = 0;

    file->records = (FileManage **)malloc(sizeof(FileManage*) * num);
    assert(file->records != NULL);

    int i;
    for (i = 0; i < num; i++) {
        file->records[i] = NULL;
    }

    return file;
}

void file_insert(FileInfo *file, char *file_path) {
    assert(file != NULL);
    if (file->count >= file->capitiry) {
        printf("容量已满，无法再插入");
        return;
    }
    FILE *fp;

    if ((fp = fopen(file_path, "r+")) == NULL) {
        printf("%s 打开失败!", file_path);
        return;
    }

    FileManage *fm = (FileManage *)malloc(sizeof(FileManage));
    fm->pos = 0;
    fm->fp = fp;
    fm->fileName = file_path;
    fm->max_buf = buf_num;

    file_set_buf(fm);

    file->records[file->count] = fm;
    file->count++;
}

int file_set_buf(FileManage *fm) {
    if (feof(fm->fp) == 0) {
        int i, ch;
        for (i = 0; i < buf_num; i++) {
            ch = fgetc(fm->fp);
            if (ch == EOF) break;
            if (ch == ' ' || ch == '\n') {
                i--;
                continue;
            }
            fm->buf[i] = ch;
        }
        return 1;
    } else {
        return 0;
    }
}

void file_output(FILE *fp, char *str) {
    // char *tmpStr = (char *)&str[0];
    printf("file_output:%c\n", str[0]);
    // fwrite(tmpStr, strlen(tmpStr), 1, fp);
}

void file_destory(FileInfo *file) {
   if (file != NULL) {
       if (file->records != NULL) {
           int i;
            for (i = 0; i < file->capitiry; i++) {
               FileManage *tmp = file->records[i];
               free(tmp);
            }
       }
       free(file->records);
       free(file);
   }
}