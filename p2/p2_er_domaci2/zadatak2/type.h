#ifndef TYPE_H
#define TYPE_H
typedef struct transaction {
    char from_address[17];
    char to_address[17];
    int value;
    int fee;
} Transaction;
typedef struct transaction_node {
    Transaction *transaction;
    struct transaction_node *next;
} TransactionNode;
#endif