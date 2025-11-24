#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD 1000
#define MAX_LEN  1000
#define MAX_LINE 10000

typedef struct {
    char word[100];
    int count;              // số lần xuất hiện
    int lines[500];         // danh sách dòng
    int lineCount;
} IndexItem;

IndexItem table[MAX_WORD];
int tableSize = 0;

char stopwords[500][100];
int stopCount = 0;

int isSentenceEnd(char ch) {
    return (ch == '.' || ch == '!' || ch == '?');
}

int isStopWord(char *w) {
    for (int i = 0; i < stopCount; i++) {
        if (strcmp(stopwords[i], w) == 0)
            return 1;
    }
    return 0;
}

int findWord(char *w) {
    for (int i = 0; i < tableSize; i++) {
        if (strcmp(table[i].word, w) == 0)
            return i;
    }
    return -1;
}

void addIndex(char *w, int line) {
    int idx = findWord(w);

    if (idx == -1) {
        strcpy(table[tableSize].word, w);
        table[tableSize].count = 1;
        table[tableSize].lineCount = 1;
        table[tableSize].lines[0] = line;
        tableSize++;
    } else {
        table[idx].count++;

        // chỉ thêm dòng nếu không trùng dòng cuối
        if (table[idx].lines[table[idx].lineCount - 1] != line) {
            table[idx].lines[table[idx].lineCount++] = line;
        }
    }
}

int cmp(const void *a, const void *b) {
    IndexItem *x = (IndexItem *)a;
    IndexItem *y = (IndexItem *)b;
    return strcmp(x->word, y->word);
}

int main() {
    //Đọc stop words 
    FILE *fs = fopen("stopw.txt", "r");
    if (!fs) {
        printf("Khong mo duoc stopw.txt\n");
        return 1;
    }

    while (fscanf(fs, "%s", stopwords[stopCount]) == 1) {
        // đổi sang lowercase
        for (int i = 0; stopwords[stopCount][i]; i++)
            stopwords[stopCount][i] = tolower(stopwords[stopCount][i]);
        stopCount++;
    }
    fclose(fs);

    //Đọc văn bản 
    FILE *fv = fopen("alice30.txt", "r");
    if (!fv) {
        printf("Khong mo duoc vanban.txt\n");
        return 1;
    }

    char lineStr[MAX_LEN];
    int lineNumber = 0;

    while (fgets(lineStr, sizeof(lineStr), fv)) {
        lineNumber++;

        int len = strlen(lineStr);
        int i = 0;

        char word[100];
        int pos = 0;

        while (i <= len) {
            if (isalpha(lineStr[i])) {
                word[pos++] = lineStr[i];
            } else {
                if (pos > 0) {
                    word[pos] = '\0';

                    // kiểm tra xem từ đứng sau dấu câu
                    int afterEnd = 0;
                    int k = i - pos - 1;

                    while (k >= 0) {
                        if (isspace(lineStr[k])) {
                            k--;
                            continue;
                        }
                        if (isSentenceEnd(lineStr[k]))
                            afterEnd = 1;
                        break;
                    }

                    // loại danh từ riêng
                    if (isupper(word[0]) && !afterEnd) {
                        // do nothing
                    } else {
                        // chuyển lowercase
                        for (int t = 0; word[t]; t++)
                            word[t] = tolower(word[t]);

                        // bỏ stop-word
                        if (!isStopWord(word)) {
                            addIndex(word, lineNumber);
                        }
                    }

                }
                pos = 0;
            }
            i++;
        }
    }

    fclose(fv);

    //Sắp xếp
    qsort(table, tableSize, sizeof(IndexItem), cmp);

    //In kết quả 
    for (int i = 0; i < tableSize; i++) {
        printf("%-12s %d  ", table[i].word, table[i].count);
        for (int j = 0; j < table[i].lineCount; j++) {
            printf("%d", table[i].lines[j]);
            if (j < table[i].lineCount - 1)
                printf(", ");
        }
        printf("\n");
    }

    return 0;
}
