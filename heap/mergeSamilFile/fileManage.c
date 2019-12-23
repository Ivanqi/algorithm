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

int file_check_letter(char ch) {
    if ( ( ch >= 'a' && ch <= 'z' ) || (ch >= 'A' && ch <= 'Z' ) ) return 1;
    else return 0; 
}

int file_check_number(char ch) {
    if (ch >='0' && ch <= '9') return 1;
    else return 0;
}

// int file_check_chinese(char ch) {
//     // 最后检查是否是汉字：（按GB 2312 汉字区检查，不考虑特殊汉字 B0A1-F7FE）
// }

void file_output(FILE *fp, char str) {
    if (file_check_letter(str)) {
        printf("file_output:%c\n", str);
        fwrite(&str, 1, 1, fp);
    }
}

void file_destory(FileInfo *file) {
   if (file != NULL) {
       if (file->records != NULL) {
           int i;
            for (i = 0; i < file->capitiry; i++) {
                FileManage *tmp = file->records[i];
                fclose(tmp->fp);
                free(tmp);
            }
       }
       free(file->records);
       free(file);
   }
}