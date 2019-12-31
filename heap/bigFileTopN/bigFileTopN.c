#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fileManage.h>
#include <hashMap.h>
#include <priorityQueue.h>

void file_get_buf(FILE *fp, HashMap *h) {
    int i,l;
    int j = 0;
    int tmp_len = 20;
    char tmp[20];
    while (!feof(fp)) {
        char buf[buf_num];
        fgets(buf, buf_num, fp);
        for (i = 0; i < strlen(buf); i++) {
            if (file_check_letter(buf[i])) {
                tmp[j] = buf[i];
                j++;
            } else if (j > 0 && (buf[i] == ' ' || buf[i] == '\n')) {
                char *buf_tmp = (char *)malloc(j);
                for (l = 0; l < j; l++) {
                    buf_tmp[l] = tmp[l];
                }
                hash_map_insert(h, buf_tmp);
                j = 0;
            }
        }
    }
}

int main () {
    int i;
    int maxSize = 500;
    int capacity = 10;
    for (i = 0; i < file_path_num;i++) {
        char *fileName = filePath[i];
        printf("--- %s / START ---- \n", fileName);
        HashMap *h = hash_map_create(maxSize, hash_code, hash_keycmp);
        FILE *fp = file_input_hander(filePath[i]);
        Heap *min_heap = heap_create(capacity, min_cmp);

        file_get_buf(fp, h);

        HashMapNode *pos;
        list_for_each(pos, h->header) {
            heap_insert(min_heap, pos->key, pos->num);
        }

        int i;

        for(i = min_heap->count; i > 1; i--) {
            HeapInfo *pos = min_heap->array[i];
            printf("  %s, %d\n", pos->key, pos->num);
        }
        printf("\n--- %s / END ---- \n", fileName);
        fclose(fp);
        hash_map_destory(h);        
    }

    return 0;
}