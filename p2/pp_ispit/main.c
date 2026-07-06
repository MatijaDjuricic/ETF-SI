#include <stdio.h>
#include <time.h>
#include "spisci.h"
int main(const int argc, const char *argv[]) {
    clock_t start_tick = clock();
    time_t start_time = time(NULL);
    printf("%s", ctime(&start_time));
    if (argc != 3) {
        printf("ARG_GRESKA\n");
    }
    FILE *f_students = fopen(argv[1], "r");
    if (f_students == NULL) {
        printf("DAT_GRESKA\n");
        fclose(f_students);
        return 1;
    }
    FILE *f_terms = fopen(argv[2], "r");
    if (f_terms == NULL) {
        printf("DAT_GRESKA\n");
        fclose(f_terms);
        return 1;
    }
    Node *students = read_students(f_students);
    Term *terms = read_terms(f_terms);
    fclose(f_students);
    fclose(f_terms);
    assign_students_to_terms(students, terms);
    write_term_files(students, terms);
    write_students(stdout, students);
    free_students(students);
    free_terms(terms);
    clock_t end_tick = clock();
    time_t end_time = time(NULL);
    double execution_time_ms = (double)(end_tick - start_tick) / CLOCKS_PER_SEC * 1000.0;
    printf("%s", ctime(&end_time));
    printf("%f ms", execution_time_ms);
    return 0;
}