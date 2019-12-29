#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fileManage.h>

FileManage* file_create() {
    FileManage *file;
    file = (FileManage *)malloc(sizeof(FileManage));
    int count = 0;

    file->filePathHandler = (FilePathHandler **)malloc(sizeof(FilePathHandler*) * file_path_num);
    assert(file->filePathHandler != NULL);
    int i;
    for (i = 0; i < file_path_num; i++) {
        file->filePathHandler[i] = NULL;
    }

    return file;
}

void file_output_insert(FileManage *file, int index) {
    assert(file != NULL);
    if (file->filePathHandler[index] != NULL) {
        return;
    } else {
        FILE *output;
        file->filePathHandler[index] = (FilePathHandler*) malloc(sizeof(FilePathHandler));
        assert(file->filePathHandler != NULL);

        if ((output = fopen(filePath[index], "w+")) == NULL) {
            printf("%s 打开失败!", filePath[index]);
        }
        fclose(output);
        if ((output = fopen(filePath[index], "a+")) == NULL) {
            printf("%s 打开失败!", filePath[index]);
        }
        file->filePathHandler[index]->output = output;
        file->filePathHandler[index]->fileName = filePath[index];
        file->filePathHandler[index]->lineFeedTag = 0;
    }
}

void file_set_buf(FILE *input, FileManage *file) {
    char tmp[20];
    int i, l;
    int j = 0;
    int tmp_len = 20;
    while (!feof(input)) {
        char buf[buf_num];
        fgets(buf, buf_num, input);
        
        for (i = 0; i < strlen(buf); i++) { 
            if (file_check_letter(buf[i])) {
                tmp[j] = buf[i];
                j++;
            } else if (buf[i] == ' ' || buf[i] == '\n') {
                int hashCode = file_hash_code(j, tmp);
                if (file->filePathHandler[hashCode] != NULL) {
                    for(l = 0; l < j; l++){
                        // if (file->filePathHandler[hashCode]->lineFeedTag >= buf_num / 2) {
                        //     file_output(file->filePathHandler[hashCode]->output, '\n');
                        //     file->filePathHandler[hashCode]->lineFeedTag = 0;
                        // }
                        // ++file->filePathHandler[hashCode]->lineFeedTag;
                        file_output(file->filePathHandler[hashCode]->output, tmp[l]);
                    }
                    if ( buf[i] == '\n') {
                        file_output(file->filePathHandler[hashCode]->output, ' ');
                    } else {
                        file_output(file->filePathHandler[hashCode]->output, buf[i]);
                    }
                }
                j  = 0;
            }
        }
    }
}

int file_hash_code(int len, char *buf) {
    int i;
    int hashVal = 0;
    for (i = 0; i < len; i++) {
        hashVal = hashVal + buf[i];
    }
    if (hashVal <= 0) return 0;
    return hashVal % file_path_num;
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
    if (fp != NULL) {
        fwrite(&str, 1, 1, fp);
    }
}

void file_destory(FileManage *file) {
//    if (file != NULL) {
//        if (file->records != NULL) {
//            int i;
//             for (i = 0; i < file->capitiry; i++) {
//                 FileManage *tmp = file->records[i];
//                 fclose(tmp->fp);
//                 free(tmp);
//             }
//        }
//        free(file->records);
//        free(file);
//    }
}

FILE* file_input_hander(char *inputFile) {
    FILE *fp;

    if ((fp = fopen(inputFile, "r+")) == NULL) {
        printf("%s 打开失败!", inputFile);
        return fp;
    }
    return fp;
}

int main() {
    
    int i;
    char input[] = "Azkaban.txt";
    FileManage *file = file_create();
    for (i = 0; i < file_path_num;i++) {
        file_output_insert(file, i);
    }
    FILE *inputHandler = file_input_hander(input);
    file_set_buf(inputHandler, file);
}