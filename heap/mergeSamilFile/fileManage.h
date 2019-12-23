#ifndef _FILE_MANAGE_
#define _FILE_MANAGE_
#define buf_num 225

typedef struct file_manage {
    int pos;
    FILE *fp;
    char *fileName;
    char buf[buf_num];
    int max_buf;
} FileManage;

typedef struct file_info {
    int capitiry;
    int count;
    FileManage **records;
} FileInfo;

FileInfo* file_create(int num);
void file_insert(FileInfo *file, char *file_path);
int file_check_letter(char ch);
int file_check_number(char ch);
int file_set_buf(FileManage *fm);
void file_output(FILE *fp, char str);
void file_destory(FileInfo *file);
#endif