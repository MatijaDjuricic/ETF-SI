#include "type.h"
#include <stdio.h>
#include <stdlib.h>
Transaction* read_transaction(FILE *input) {
    Transaction *data = malloc(sizeof(Transaction));
    if (fscanf(input,"%[^,],%[^,],%d,%d\n", data->from_address, data->to_address, &data->value, &data->fee) != 4) {
        free(data);
        return NULL;
    }
    return data;
}
TransactionNode* read_list(char *filename) {
    FILE *input = fopen(filename, "r");
    if (input == NULL) {
        printf("DAT_GRESKA");
        return NULL;
    }
    TransactionNode *head = NULL, *tail = NULL;
    Transaction *temp_data;
    while ((temp_data = read_transaction(input)) != NULL) {
        TransactionNode *new_node = malloc(sizeof(TransactionNode));
        new_node->transaction = temp_data;
        new_node->next = NULL;
        if (head == NULL) head = new_node;
        else tail->next = new_node;
        tail = new_node;
    }
    fclose(input);
    return head;
}
void remove_transactions(TransactionNode **head, int max_fee) {
    TransactionNode *current = *head, *prev = NULL;
    while (current != NULL) {
        if (current->transaction->fee > max_fee) {
            TransactionNode *temp = current;
            current = current->next;
            if (prev == NULL) *head = current;
            else prev->next = current;
            free(temp->transaction);
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}
void print_extreme_values(TransactionNode *head, char *filename) {
    FILE *f = fopen(filename, "w");
    TransactionNode *max[3] = {NULL, NULL, NULL};
    TransactionNode *min[3] = {NULL, NULL, NULL};
    for (TransactionNode *current = head; current != NULL; current = current->next) {
        if (max[0] == NULL || current->transaction->value > max[0]->transaction->value) {
            max[2] = max[1]; max[1] = max[0]; max[0] = current;
        } else if (max[1] == NULL || current->transaction->value > max[1]->transaction->value) {
            max[2] = max[1]; max[1] = current;
        } else if (max[2] == NULL || current->transaction->value > max[2]->transaction->value) {
            max[2] = current;
        }
        if (min[0] == NULL || current->transaction->value < min[0]->transaction->value) {
            min[2] = min[1]; min[1] = min[0]; min[0] = current;
        } else if (min[1] == NULL || current->transaction->value < min[1]->transaction->value) {
            min[2] = min[1]; min[1] = current;
        } else if (min[2] == NULL || current->transaction->value < min[2]->transaction->value) {
            min[2] = current;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (max[i] != NULL) {
            fprintf(f, "%s->%s (%d) %d\n",
                max[i]->transaction->from_address,
                max[i]->transaction->to_address,
                max[i]->transaction->value,
                max[i]->transaction->fee
            );
        }
    }
    for (int i = 0; i < 3; i++) {
        if (min[i] != NULL) {
            fprintf(f, "%s->%s (%d) %d\n",
                min[i]->transaction->from_address,
                min[i]->transaction->to_address,
                min[i]->transaction->value,
                min[i]->transaction->fee
            );
        }
    }
    fclose(f);
}
void free_transaction_list(TransactionNode *head) {
    while (head != NULL) {
        TransactionNode *temp = head;
        head = head->next;
        free(temp->transaction);
        free(temp);
    }
}
int main() {
    int max_fee;
    TransactionNode *head = read_list("transactions.csv");
    scanf("%d", &max_fee);
    remove_transactions(&head, max_fee);
    print_extreme_values(head, "stats.txt");
    free_transaction_list(head);
    return 0;
}