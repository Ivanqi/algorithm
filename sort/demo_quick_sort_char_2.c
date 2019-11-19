#include <stdio.h>
#include <string.h>

#define MAXLINES 5000
#define MAXLEN 1000
#define ALLOCSIZE 10000

static char allocbuf[ALLOCSIZE];
static char *alloccp = allocbuf;

void writelines(char *lineptr[], int nlines);
char *alloc(int);
void qsort(char *lineptr[], int left, int right);


void writelines(char *lineptr[], int nlines) {
    int i;
    for (i = 0; i < nlines; i++) {
        printf("%s\n", lineptr[i]);
    }
}

char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - alloccp >= n) {
        alloccp += n;
        return alloccp - n;
    } else {
        return 0;
    }
}

void qsort(char *v[], int left, int right) {
    int i, last;
    void swap(char *v[], int i, int j);

    if (left >= right) return;

    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (strcmp(v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }

    swap(v, left, last);
    qsort(v, left, last - 1);
    qsort(v, last + 1, right);
}

void swap(char *v[], int i, int j) {
    char *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}


int main() {
    int nlines = 10;
    char *lineptr[] = {"C", "C++", "Java", "PHP", "Python", "Perl", "Ruby", "R", "JavaScript", "PHP"};
    qsort(lineptr, 0, nlines - 1);
    writelines(lineptr, nlines);
}
