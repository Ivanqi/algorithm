#ifndef _FILE_MANAGE_
#define _FILE_MANAGE_
#define buf_num 1024
#define file_path_num 10
char static filePath[file_path_num][12] = {"test_0.txt", "test_1.txt", "test_2.txt", "test_3.txt", "test_4.txt", "test_5.txt", "test_6.txt", "test_7.txt", "test_8.txt", "test_9.txt"};

typedef struct file_path_handler {
    FILE *output;
    char *fileName;
    int lineFeedTag;
} FilePathHandler;

typedef struct file_manage {
    int count;
    FilePathHandler** filePathHandler;
} FileManage;

FileManage* file_create();
FILE* file_input_hander(char *inputFile);
void file_output_insert(FileManage *file, int index);
int file_check_letter(char ch);
int file_check_number(char ch);
void file_set_buf(FILE *input, FileManage *file);
void file_output(FILE *fp, char str);
void file_destory(FileManage *file);
int file_hash_code(int len, char *buf);


FILE* file_input_hander(char *inputFile) {
    FILE *fp;

    if ((fp = fopen(inputFile, "r+")) == NULL) {
        printf("%s 打开失败!", inputFile);
        return fp;
    }
    return fp;
}

int file_check_letter(char ch) {
    if ( ( ch >= 'a' && ch <= 'z' ) || (ch >= 'A' && ch <= 'Z' ) ) return 1;
    else return 0; 
}
#endif