#include <stdio.h>
#include <stdlib.h>
#include "spisci.h"
void write_students(FILE *file, Node *head) {
    int student_count = 0;
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        fprintf(file,"%d. %04d/%02d %s %s\n",
            ++student_count,
            curr->data->index_number,
            curr->data->index_year,
            curr->data->last_name,
            curr->data->first_name
        );
    }
}
void write_term_files(Node *students, Term *terms) {
    Node *curr_student = students;
    Term *curr_term = terms;
    int term_index = 0;
    while (curr_term != NULL) {
        char filename[32];
        sprintf(filename, "term_%d.txt", term_index);
        FILE *file = fopen(filename, "w");
        if (file == NULL) return;
        fprintf(file,"%s %s-%s\n",
            curr_term->date,
            curr_term->start,
            curr_term->end
        );
        int student_count = 0;
        while (curr_student != NULL && student_count < T_MAX) {
            fprintf(file,"%d. %04d/%02d %s %s\n",
                ++student_count,
                curr_student->data->index_number,
                curr_student->data->index_year,
                curr_student->data->last_name,
                curr_student->data->first_name
            );
            curr_student = curr_student->next;
        }
        fclose(file);
        curr_term = curr_term->next;
        term_index++;
    }
}