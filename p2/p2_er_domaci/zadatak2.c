#include <stdio.h>
#include <stdlib.h>
#define MIN_COINS 5
int **read_coins(int *n) {
    int **data, n_move;
    scanf("%d", n);
    data = malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++) {
        scanf("%d", &n_move);
        data[i] = malloc((n_move + 1) * sizeof(int));
        data[i][0] = n_move;
        for (int j = 1; j <= n_move; j++) {
            scanf("%d", &data[i][j]);
        }
    }
    return data;
}
void print_coins(int **data, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= data[i][0]; j++) {
            printf("%d", data[i][j]);
            if(j != data[i][0]){
                putchar(' ');
            }
        }
        putchar('\n');
    }
}
int **filter_coins(int **data, int *n) {
    int new_n = *n;
    for (int i = 0; i < new_n; i++) {
        int player1 = 0, player2 = 0;
        for (int j = 1; j <= data[i][0]; j++) {
            if (j % 2 == 1 && data[i][j] >= MIN_COINS) {
                player1 = 1;
            }
            if (j % 2 == 0 && data[i][j] >= MIN_COINS) {
                player2 = 1;
            }
        }
        if (player1 && player2) {
            free(data[i]);
            for (int k = i; k < new_n - 1; k++) {
                data[k] = data[k + 1];
            }
            new_n--;
            i--;
        }
    }
    *n = new_n;
    return data;
}
void free_coins(int **data, int n) {
    for (int i = 0; i < n; i++) {
        free(data[i]);
    }
    free(data);
}
int main () {
    int n;
    int **data = read_coins(&n);
    data = filter_coins(data, &n);
    print_coins(data, n);
    free_coins(data, n);
    return 0;
}