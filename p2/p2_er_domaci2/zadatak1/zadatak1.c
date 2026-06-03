#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
data_t* read_line(FILE *input) {
    char language[256];
    data_t *data = malloc(sizeof(data_t));
    if (fscanf(input, "%[^,],%d,%d,%d", language, &data->year, &data->quarter, &data->count) != 4) {
        free(data);
        return NULL;
    }
    data->language = strdup(language);
    return data;
}
node_t* read_lines(char* input_name) {
    FILE *input = fopen(input_name, "r");
    if (input == NULL) return NULL;
    node_t *head = NULL, *tail = NULL;
    data_t *temp_data;
    while ((temp_data = read_line(input)) != NULL) {
        node_t *new_node = malloc(sizeof(node_t));
        new_node->data = temp_data;
        new_node->next = NULL;
        if (head == NULL) head = new_node;
        else tail->next = new_node;
        tail = new_node;
    }
    fclose(input);
    return head;
}
void sort(node_t* head) {
    for (node_t *current = head; current != NULL; current = current->next) {
        for (node_t *second = current->next; second != NULL; second = second->next) {
            if (current->data->year > second->data->year ||
                (current->data->year == second->data->year && current->data->quarter > second->data->quarter) ||
                (current->data->year == second->data->year && current->data->quarter == second->data->quarter &&
                strcmp(current->data->language, second->data->language) > 0)) {
                data_t *temp = current->data;
                current->data = second->data;
                second->data = temp;
            }
        }
    }
}
node_t* compress_data(node_t* head, int year_from, int year_to) {
    node_t *current = head, *prev = NULL;
    while (current != NULL && (current->data->year < year_from || current->data->year >= year_to)) {
        node_t *temp = current;
        current = current->next;
        free(temp->data->language);
        free(temp->data);
        free(temp);
    }
    head = current;
    for (current = head; current != NULL; current = current->next) {
        if (current->data->year < year_from || current->data->year >= year_to) {
            node_t *temp = current;
            if (prev != NULL) prev->next = current->next;
            free(temp->data->language);
            free(temp->data);
            free(temp);
            continue;
        }
        while (current->next != NULL &&
            current->next->data->year == current->data->year &&
            current->next->data->quarter == current->data->quarter) {
            node_t *duplicate = current->next;
            current->data->count += duplicate->data->count;
            int new_len = strlen(current->data->language) + strlen(duplicate->data->language) + 2;
            char *new_str = malloc(new_len * sizeof(char));
            if (new_str != NULL) {
                sprintf(new_str, "%s,%s", current->data->language, duplicate->data->language);
                free(current->data->language);
                current->data->language = new_str;
            }
            current->next = duplicate->next;
            free(duplicate->data->language);
            free(duplicate->data);
            free(duplicate);
        }
        prev = current;
    }
    return head;
}
void free_data(node_t* head) {
    while (head != NULL) {
        node_t *temp = head;
        head = head->next;
        free(temp->data->language);
        free(temp->data);
        free(temp);
    }
}
void print_data(node_t *first) {
    node_t *current = first;
    while (current != NULL) {
        printf("%s %d %d %d",
            current->data->language, current->data->year,
            current->data->quarter, current->data->count
        );
        current = current->next;
    }
}
int main() {
    node_t *head = read_lines("data.txt");
    sort(head);
    head = compress_data(head, 2020, 2021);
    print_data(head);
    free_data(head);
    return 0;
}