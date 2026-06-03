#ifndef TYPE_H
#define TYPE_H
typedef struct task_info {
    char name[51];
    int priority;
    int day;
    int month;
    int year;
} TData;
typedef struct task {
    TData data;
    struct task *subtasks;
    struct task *next;
} TNode;
#endif