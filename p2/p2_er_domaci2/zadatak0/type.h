#ifndef TYPE_H
#define TYPE_H
typedef struct data {
    char *email;
    char date[11];
    int num;
} TData;
typedef struct node {
    TData data;
    struct node *next;
} TNode;
#endif