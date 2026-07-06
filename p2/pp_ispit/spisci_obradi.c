#include <stdio.h>
#include <stdlib.h>
#include "spisci.h"
void assign_students_to_terms(Node *students, Term *terms) {
    FILE *out = fopen("termini.txt", "w");
    Node *curr_student = students;
    Term *curr_term = terms;
    int student_count = 0, term_index = 0;
    while (curr_student != NULL && curr_term != NULL) {
        if (student_count == 0) {
            fprintf(out, "%d: %s %s-%s\n",
                term_index,
                curr_term->date,
                curr_term->start,
                curr_term->end
            );
        }
        fprintf(out, "  %d. %04d/%02d %s %s\n",
            ++student_count,
            curr_student->data->index_number,
            curr_student->data->index_year,
            curr_student->data->last_name,
            curr_student->data->first_name
        );
        curr_student = curr_student->next;
        if (student_count == T_MAX) {
            student_count = 0;
            curr_term = curr_term->next;
            term_index++;
        }
    }
    fclose(out);
}
void free_students(Node* head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}
void free_terms(Term *head) {
    while (head != NULL) {
        Term *temp = head;
        head = head->next;
        free(temp);
    }
}