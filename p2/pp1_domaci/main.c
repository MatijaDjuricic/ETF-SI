#include <stdio.h>
#define MAX_N 100
int main() {
    int arr[MAX_N], n, i;
    scanf("%d", &n);
    if (n <= 0 || n > MAX_N) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i != n - 1) {
            putchar(' ');
        }
    }
    int min = arr[0], max = arr[0];
    for (i = 0; i < n; i++) {
        if (min > arr[i]) {
            min = arr[i];
        }
        if (max < arr[i]) {
            max = arr[i];
        }
    }
    printf("\n%d %d\n", min, max);
    int start_i = 0, curr_len = 1, max_len = 1;
    int max_start_i = 0, max_end_i = 0;
    for (i = 1; i < n + 1; i++) {
        if (arr[i] > arr[i - 1]) {
            curr_len++;
        } else {
            if (curr_len > max_len) {
                max_len = curr_len;
                max_start_i = start_i;
                max_end_i = i - 1;
            }
            start_i = i;
            curr_len = 1;
        }
    }
    for (i = max_start_i; i < max_end_i + 1; i++) {
        printf("%d", arr[i]);
        if (i != max_end_i) {
            putchar(' ');
        }
    }
    return 0;
}