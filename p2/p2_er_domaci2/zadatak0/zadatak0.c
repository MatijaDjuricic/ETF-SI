#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
TNode* read_node(FILE *input) {
    char email[256];
    TNode *node = malloc(sizeof(TNode));
    if (fscanf(input, "%s %s", email, node->data.date) != 2) {
        free(node);
        return NULL;
    }
    node->data.email = strdup(email);
    node->next = NULL;
    return node;
}
TNode* read_list(FILE *input) {
    TNode *head = NULL, *tail = NULL, *new_node;
    while ((new_node = read_node(input)) != NULL) {
        if (head == NULL) head = new_node;
        else tail->next = new_node;
        tail = new_node;
    }
    return head;
}
TNode* log_users(TNode *first, char date_from[11], char date_to[11]) {
    TNode *head = NULL, *tail = NULL;
    for (TNode *current = first; current != NULL; current = current->next) {
        if (strcmp(current->data.date, date_from) >= 0 && strcmp(current->data.date, date_to) <= 0) {
            TNode *existing = NULL;
            for (TNode *search = head; search != NULL; search = search->next) {
                if (strcmp(current->data.email, search->data.email) == 0) {
                    existing = search;
                    break;
                }
            }
            if (existing != NULL) {
                existing->data.num++;
            } else {
                TNode *new_node = malloc(sizeof(TNode));
                new_node->data.email = strdup(current->data.email);
                strcpy(new_node->data.date, current->data.date);
                new_node->data.num = 1;
                new_node->next = NULL;
                if (head == NULL) head = new_node;
                else tail->next = new_node;
                tail = new_node;
            }
        }
    }
    for (TNode *current = head; current != NULL; current = current->next) {
        for (TNode *second = current->next; second != NULL; second = second->next) {
            if (strcmp(current->data.email, second->data.email) > 0) {
                TData temp = current->data;
                current->data = second->data;
                second->data = temp;
            }
        }
    }
    return head;
}
void print_list(TNode *first, char *file_name) {
    FILE *output = fopen(file_name, "w");
    TNode *current = first;
    while (current != NULL) {
        fprintf(output, "%s %d", current->data.email, current->data.num);
        if (current->next != NULL) {
            fprintf(output, "\n");
        }
        current = current->next;
    }
    fclose(output);
}
void free_list(TNode *first) {
    while (first != NULL) {
        TNode *temp = first;
        first = first->next;
        free(temp->data.email);
        free(temp);
    }
}
int main() {
    FILE *dat = fopen("log.txt", "r");
    if (!dat) {
        return 0;
    }
    TNode *first = read_list(dat);
    TNode *second = log_users(first, "2021-01-15", "2021-02-15");
    print_list(second, "statistika.txt");
    free_list(first);
    free_list(second);
    fclose(dat);
    return 0;
}