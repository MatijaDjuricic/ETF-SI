#ifndef SPISCI_H
#define SPISCI_H
#define T_MAX 10
typedef struct student {
    int index;
    int index_number;
    int index_year;
    char first_name[32];
    char last_name[32];
    char department[3];
} Student;
typedef struct node {
    Student *data;
    struct node *prev;
    struct node *next;
} Node;
typedef struct term {
    char date[12];
    char start[6];
    char end[6];
    struct term *next;
} Term;
Node* read_students(FILE *file);
Term* read_terms(FILE *file);
void write_students(FILE *file, Node *head);
void write_term_files(Node *students, Term *terms);
void assign_students_to_terms(Node *students, Term *terms);
void free_students(Node *head);
void free_terms(Term *head);
#endif