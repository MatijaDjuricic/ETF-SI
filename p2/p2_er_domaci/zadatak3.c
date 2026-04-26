#include <stdio.h>
#include <stdlib.h>
#define N_COL 3
#define SAFE_MALLOC(ptr, n) \
ptr = malloc((n) * sizeof(*ptr)); \
if (ptr == NULL) { \
printf("MEM_GRESKA"); \
}
#define SAFE_REALLOC(data, n) \
void *ptr = realloc(data, (n) * sizeof(*data)); \
if (ptr == NULL) { \
free(data); \
printf("MEM_GRESKA"); \
} \
data = ptr
int **read_classrooms() {
    int **data, n, i = 0, row = 0;
    SAFE_MALLOC(data, 1);
    while (1) {
        scanf("%d", &n);
        if (n == -1) break;
        if (i % N_COL == 0) {
            SAFE_REALLOC(data, row + 1);
            SAFE_MALLOC(data[row], N_COL);
            row++;
        }
        data[row - 1][i % N_COL] = n;
        i++;
    }
    SAFE_REALLOC(data, row + 1);
    data[row] = NULL;
    return data;
}
void print_classrooms(int **data) {
    for (int i = 0; data[i] != 0; i++) {
        for (int j = 0; j < N_COL; j++) {
            printf("%d", data[i][j]);
            if (j != N_COL - 1) {
                putchar(' ');
            }
        }
        if (data[i + 1] != 0) {
            putchar('\n');
        }
    }
}
void process_classrooms(int **data) {
    int i, j;
    for (i = 0; data[i] != 0; i++) {
        for (j = i + 1; data[j] != 0; j++) {
            if (data[i][1] > data[j][1]) {
                int *temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
    for (i = 0; data[i] != 0; i++) {
        for (j = i + 1; data[j] != 0; j++) {
            if (data[i][2] + data[j][2] <= data[j][1]) {
                data[j][2] += data[i][2];
                free(data[i]);
                for (int k = i; data[k] != 0; k++) {
                    data[k] = data[k + 1];
                }
                i--;
                break;
            }
        }
    }
}
void free_classrooms(int **data) {
    for (int i = 0; data[i] != 0; i++) {
        free(data[i]);
    }
    free(data);
}
int main() {
    int **data = read_classrooms();
    process_classrooms(data);
    print_classrooms(data);
    free_classrooms(data);
    return 0;
}