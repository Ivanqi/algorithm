#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fileManage.h>
#include <priorityQueue.h>
#include <assert.h>
#define FILE_ARR_LEN 4
#define FILE_PATH 11

int min_keycmp(char *a, char *b) {
    if (strcmp(a, b) < 0) return 1;
    else return 0;
}

int remove_callback_func(FILE *output, HeapInfo* heap_info) {
    assert(heap_info != NULL);

    if (heap_info->file_info->pos >= heap_info->file_info->max_buf) {
        if (file_set_buf(heap_info->file_info)) {
            heap_info->file_info->pos = 0;
        } else {
            heap_info->file_info->pos = heap_info->file_info->max_buf;
            return 0;
        }
    }
    file_output(output, heap_info->file_info->buf[heap_info->file_info->pos]);
    heap_info->val = heap_info->file_info->buf[++heap_info->file_info->pos];
    return 1;
}



FileInfo* file_oper() {
    char file_path[FILE_ARR_LEN][FILE_PATH] = {"test_1.txt", "test_2.txt", "test_3.txt", "test_4.txt"};

    FileInfo* file = file_create(FILE_ARR_LEN);
    int i;
    for (i = 0; i < FILE_ARR_LEN; i++) {
        file_insert(file, file_path[i]);
    }

    return file;
}

void heap_oper(MinHeap *heap, FileInfo* file) {
    int i;
    for (i = 0; i < file->count;i++) {
        heap_insert(heap, file->records[i]);
    }
}

FILE* return_output_handler() {
    FILE *output;
    if ((output = fopen("ret.txt", "w+")) == NULL) {
        printf("%s 打开失败!", "ret.txt");
    }
    fclose(output);
    if ((output = fopen("ret.txt", "a+")) == NULL) {
        printf("%s 打开失败!", "ret.txt");
    }


    return output;
}

int main () {
    FILE *output = return_output_handler();
    assert(output != NULL);
    
    FileInfo* file = file_oper();

    MinHeap *heap =  heap_create(FILE_ARR_LEN, output, min_keycmp, remove_callback_func);

    heap_oper(heap, file);

    // int i;
    // for (i = 0; i < 52; i++) {
    //     heap_remove_top(heap);
    // }
    while (1) {
        if (heap->count == 0) break;

        heap_remove_top(heap);
    }

    file_destory(file);
    heap_destory(heap);
    fclose(output);
    return 0;
}