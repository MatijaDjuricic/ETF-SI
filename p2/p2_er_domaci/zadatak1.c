#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_MAX 100
#define SAFE_REALLOC(data, n) \
void *ptr = realloc(data, (n) * sizeof(*data)); \
if (ptr == NULL) { \
free(data); \
printf("MEM_GRESKA"); \
} \
data = ptr
int **read_friends() {
    int **friends = NULL, rows = 0;
    char line[LINE_MAX];
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "-1") == 0) break;
        int *row = NULL;
        int count = 0;
        char *num_str = strtok(line, " ");
        row = malloc(2 * sizeof(int));
        row[0] = atoi(num_str);
        num_str = strtok(NULL, " ");
        while (num_str != NULL) {
            SAFE_REALLOC(row, count + 3);
            row[2 + count] = atoi(num_str);
            count++;
            num_str = strtok(NULL, " ");
        }
        row[1] = count;
        SAFE_REALLOC(friends, rows + 1);
        friends[rows++] = row;
    }
    SAFE_REALLOC(friends, rows + 1);
    friends[rows] = NULL;
    return friends;
}
void print_friends(int **friends) {
    for (int i = 0; friends[i] != 0; i++) {
        for (int j = 0; j < friends[i][1] + 2; j++) {
            if (j == 1) continue;
            printf("%d", friends[i][j]);
            if (j == 0) {
                printf(" | ");
            } else if (j != friends[i][1] + 1) {
                putchar(' ');
            }
        }
        if (friends[i + 1] != 0) {
            putchar('\n');
        }
    }
}
void remove_false_friends(int **friends) {
    for (int i = 0; friends[i] != 0; i++) {
        for (int j = 2; j < friends[i][1] + 2; j++) {
            int found = 0;
            for (int k = 0; friends[k] != 0; k++) {
                if (i == k) continue;
                if (friends[i][j] == friends[k][0]) {
                    for (int l = 2; l < friends[k][1] + 2; l++) {
                        if (friends[k][l] == friends[i][0]) {
                            found = 1;
                            break;
                        }
                    }
                    break;
                }
            }
            if (!found) {
                for (int m = j; m < friends[i][1] + 1; m++) {
                    friends[i][m] = friends[i][m + 1];
                }
                friends[i][1]--;
                j--;
            }
        }
    }
}
void free_friends(int **friends) {
    for (int i = 0; friends[i] != 0; i++) {
        free(friends[i]);
    }
    free(friends);
}
int main() {
    int **friends = read_friends();
    remove_false_friends(friends);
    print_friends(friends);
    free_friends(friends);
    return 0;
}