#ifndef TYPE_H
#define TYPE_H
typedef struct data {
    char *language;
    int year;
    int quarter;
    int count;
} data_t;
typedef struct node {
    data_t *data;
    struct node *next;
} node_t;
#endif