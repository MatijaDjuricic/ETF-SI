#include <stdio.h>
#include <stdlib.h>
#include "spisci.h"
Node *read_students(FILE *file) {
    Node *head = NULL, *tail = NULL;
    while (1) {
        Node *new_node = malloc(sizeof(Node));
        new_node->data = malloc(sizeof(Student));
        if (new_node == NULL) return NULL;
        if (fscanf(file, "%d %30s %30s %2s", &new_node->data->index, new_node->data->last_name,
            new_node->data->first_name, new_node->data->department) != 4) {
            free(new_node);
            break;
        }
        new_node->data->index_number = new_node->data->index % 10000;
        new_node->data->index_year = new_node->data->index / 10000;
        new_node->prev = NULL;
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
    }
    return head;
}
Term *read_terms(FILE *file) {
    Term *head = NULL, *tail = NULL;
    while (1) {
        Term *new_term = malloc(sizeof(Term));
        if (new_term == NULL) return NULL;
        if (fscanf(file, " %11[^-]-%5[^#]#%5s", new_term->date, new_term->start, new_term->end) != 3) {
            free(new_term);
            break;
        }
        new_term->next = NULL;
        if (head == NULL) {
            head = new_term;
            tail = new_term;
        } else {
            tail->next = new_term;
            tail = new_term;
        }
    }
    return head;
}