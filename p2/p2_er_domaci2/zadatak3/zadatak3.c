#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
TNode* read_list(char *dat_name);
TNode* read_node(FILE *input);
void print_list(TNode *head, FILE *output);
void free_list(TNode *head);
int main(int argc, char *argv[]) {
    FILE *out = fopen("view.txt", "w");
    TNode *head = read_list(argv[1]);
    print_list(head, out);
    free_list(head);
    fclose(out);
    return 0;
}
TNode* read_node(FILE *input) {
    char subtasks_file[51];
    TNode *new_node = malloc(sizeof(TNode));
    if (fscanf(input, "%s %d %d.%d.%d. %s",  new_node->data.name, &new_node->data.priority,
        &new_node->data.day, &new_node->data.month, &new_node->data.year, subtasks_file) != 6) {
        free(new_node);
        return NULL;
    }
    new_node->next = NULL;
    if (strcmp(subtasks_file, "X") == 0 || strcmp(subtasks_file, "") == 0) {
        new_node->subtasks = NULL;
    } else {
        new_node->subtasks = read_list(subtasks_file);
    }
    return new_node;
}
TNode* read_list(char *dat_name) {
    FILE *input = fopen(dat_name, "r");
    if (input == NULL) return NULL;
    TNode *head = NULL, *tail = NULL, *new_node;
    while ((new_node = read_node(input)) != NULL) {
        if (head == NULL) head = new_node;
        else tail->next = new_node;
        tail = new_node;
    }
    fclose(input);
    return head;
}
void print_list(TNode *head, FILE *output) {
    TNode *current = head;
    static int depth = 0;
    while (current != NULL) {
        for (int i = 0; i < depth; i++) {
            fprintf(output, "    ");
        }
        fprintf(output, "%d %s (%d.%d.%d.)\n",
            current->data.priority, current->data.name,
            current->data.day, current->data.month, current->data.year
        );
        if (current->subtasks != NULL) {
            depth++;
            print_list(current->subtasks, output);
            depth--;
        }
        current = current->next;
    }
}
void free_list(TNode *head) {
    while (head != NULL) {
        TNode *temp = head;
        head = head->next;
        free_list(temp->subtasks);
        free(temp);
    }
}