#include <stdio.h>
#include <stdlib.h>
#define MAX_GRADE 10
#define MIN_GRADE 5
#define SAFE_MALLOC(ptr, n) \
ptr = malloc((n) * sizeof(*ptr)); \
if (!ptr) { \
    printf("MEM_GRESKA"); \
}
int **input_data(int *rows, int *cols) {
    int **matrix;
    scanf("%d %d", rows, cols);
    SAFE_MALLOC(matrix, *rows);
    for (int i = 0; i < *rows; i++) {
        SAFE_MALLOC(matrix[i], *cols);
        for (int j = 0; j < *cols; j++) {
            scanf("%d ", &matrix[i][j]);
        }
    }
    return matrix;
}
void remove_students(int **matrix, int *rows, int *cols) {
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (matrix[i][j] == MIN_GRADE) {
                free(matrix[i]);
                for (int k = i; k < *rows - 1; k++) {
                    matrix[k] = matrix[k + 1];
                }
                (*rows)--;
                i--;
                break;
            }
        }
    }
}
void remove_subjects(int **matrix, int *rows, int *cols) {
    for (int j = 0; j < *cols; j++) {
        int found = 0;
        for (int i = 0; i < *rows; i++) {
            if (matrix[i][j] == MAX_GRADE) {
                found = 1;
                break;
            }
        }
        if (!found) {
            for (int i = 0; i < *rows; i++) {
                for (int k = j; k < *cols - 1; k++) {
                    matrix[i][k] = matrix[i][k + 1];
                }
            }
            (*cols)--;
            j--;
        }
    }
}
void remove_subjects_2(int **matrix, int *rows, int *cols) {
    int new_col = 0;
    for (int j = 0; j < *cols; j++) {
        int found = 0;
        for (int i = 0; i < *rows; i++) {
            if (matrix[i][j] == MAX_GRADE) {
                found = 1;
                break;
            }
        }
        if (found) {
            if (new_col != j) {
                for (int i = 0; i < *rows; i++) {
                    matrix[i][new_col] = matrix[i][j];
                }
            }
            new_col++;
        }
    }
    *cols = new_col;
}
void print_data(int **matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        if (matrix[i]) {
            putchar('\n');
        }
    }
}
void free_data(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
int main() {
    int rows, cols;
    int **matrix = input_data(&rows, &cols);
    print_data(matrix, rows, cols);
    remove_students(matrix, &rows, &cols);
    print_data(matrix, rows, cols);
    remove_subjects(matrix, &rows, &cols);
    print_data(matrix, rows, cols);
    free_data(matrix, rows);
    return 0;
}