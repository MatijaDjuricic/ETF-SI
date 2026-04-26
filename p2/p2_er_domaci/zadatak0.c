#include <stdio.h>
#include <stdlib.h>
int **read_loot(int *cnt) {
    int **loot, exp_cnt;
    scanf("%d", cnt);
    loot = malloc(*cnt * sizeof(int*));
    for (int i = 0; i < *cnt; i++) {
        scanf("%d", &exp_cnt);
        loot[i] = malloc((exp_cnt + 1) * sizeof(int));
        loot[i][0] = exp_cnt;
        for (int j = 1; j <= exp_cnt; j++) {
            scanf("%d", &loot[i][j]);
        }
    }
    return loot;
}
void print_loot(int **loot, int cnt) {
    for (int i = 0; i < cnt; i++) {
        for (int j = 1; j <= loot[i][0]; j++) {
            printf("%d", loot[i][j]);
            if(j != loot[i][0]){
                putchar(' ');
            }
        }
        if(i != cnt - 1){
            putchar('\n');
        }
    }
}
int *distribute_spoils(int **loot, int cnt, int pirates) {
    int turn = 0;
    int *spoils = calloc(pirates, sizeof(int));
    for (int i = 0; i < cnt; i++) {
        int left = 1;
        int right = loot[i][0];
        while (left <= right) {
            if (loot[i][right] > loot[i][left]) {
                spoils[turn++ % pirates] += loot[i][right--];
            } else {
                spoils[turn++ % pirates] += loot[i][left++];
            }
        }
    }
    return spoils;
}
void free_loot(int **loot, int cnt) {
    for (int i = 0; i < cnt; i++) {
        free(loot[i]);
    }
    free(loot);
}
int main() {
    int cnt, pirates;
    int **loot = read_loot(&cnt);
    scanf("%d", &pirates);
    print_loot(loot, cnt);
    putchar('\n');
    int *spoils = distribute_spoils(loot, cnt, pirates);
    for (int i = 0; i < pirates; i++) {
        printf("%d", spoils[i]);
        if(i != pirates - 1){
            putchar(' ');
        }
    }
    free(spoils);
    free_loot(loot, cnt);
    return 0;
}