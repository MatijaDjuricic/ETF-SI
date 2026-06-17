#include <stdio.h>
#include <stdlib.h>
#define WORKS_LIMIT 10
#define SAFE_MALLOC(ptr, n) \
ptr = malloc((n) * sizeof(*ptr)); \
if (!ptr) { \
printf("MEM_GRESKA"); \
}
typedef struct session {
    char name[3];
    char time_from[6];
    char time_to[6];
    int room_num;
    int works_num;
} Session;
typedef struct node {
    Session data;
    struct node *next;
} Node;
Node *read_node() {
    Session s;
    if (fscanf(stdin, "%5s-%5s %d %2s %d", s.time_from, s.time_to, &s.room_num, s.name, &s.works_num) != 5) {
        return NULL;
    }
    Node *node;
    SAFE_MALLOC(node, 1);
    node->data = s;
    node->next = NULL;
    return node;
}
Node* read_list() {
    Node *head = NULL, *tail = NULL, *new_node;
    while ((new_node = read_node()) != NULL) {
        if (head == NULL) head = new_node;
        else tail->next = new_node;
        tail = new_node;
    }
    return head;
}
int calc_time_min(char time_format[6]) {
    return 60 * (10 * (time_format[0] - '0') + (time_format[1] - '0')) + 10 * (time_format[3] - '0') + (time_format[4] - '0');
}
int calc_duration(char time_from[6], char time_to[6]) {
    return calc_time_min(time_to) - calc_time_min(time_from);
}
void sort_list(Node *head) {
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        for (Node *sec = curr; sec != NULL; sec = sec->next) {
            if (curr->data.room_num > sec->data.room_num ||
                (curr->data.room_num == sec->data.room_num &&
                calc_time_min(curr->data.time_from) > calc_time_min(sec->data.time_from))) {
                Session temp = curr->data;
                curr->data = sec->data;
                sec->data = temp;
            }
        }
    }
}
void filter_list(Node **head) {
    while (*head && (*head)->data.works_num < WORKS_LIMIT) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    Node *curr = (*head)->next, *prev = *head;
    while (curr != NULL) {
        if (curr->data.works_num < WORKS_LIMIT) {
            prev->next = curr->next;
            free(curr);
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}
void print_list(Node *head) {
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        printf("%d %s %d %s %d",
            curr->data.room_num,
            curr->data.time_from,
            calc_duration(curr->data.time_from, curr->data.time_to),
            curr->data.name,
            curr->data.works_num
        );
        if (curr->next != NULL) putchar('\n');
    }
}
void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}
int main() {
    Node *head = read_list();
    sort_list(head);
    filter_list(&head);
    print_list(head);
    free_list(head);
    return 0;
}