#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SAFE_MALLOC(ptr, n) \
ptr = malloc((n) * sizeof(*ptr)); \
if (!ptr) { \
printf("MEM_GRESKA"); \
}
typedef struct book {
    char ISBN[14];
    char title[101];
    char author[51];
    char genre[31];
    int year;
    float price;
    float avg_grade;
} Book;
typedef struct book_node {
    Book data;
    struct book_node *next;
} BookNode;
typedef struct author {
    char name[51];
    int book_num;
    float sum_avg_grade;
    float avg_grade;
} Author;
typedef struct author_node {
    Author data;
    struct author_node *next;
} AuthorNode;
typedef struct genre_node {
    char name[31];
    Book best_book;
    struct genre_node *next;
} GenreNode;
BookNode *read_book_node(FILE *input) {
    Book book;
    if (fscanf(input, " %[^,],%[^,],%[^,],%[^,],%d,%f,%f",
        book.ISBN, book.title, book.author, book.genre,
        &book.year, &book.price, &book.avg_grade) != 7) {
        return NULL;
    }
    BookNode *node;
    SAFE_MALLOC(node, 1);
    node->data = book;
    node->next = NULL;
    return node;
}
BookNode *create_books_list(FILE *input) {
    BookNode *head = NULL, *tail = NULL, *curr_node;
    fscanf(input, "%*[^\n]");
    while ((curr_node = read_book_node(input)) != NULL) {
        if (head == NULL) head = curr_node;
        else tail->next = curr_node;
        tail = curr_node;
    }
    return head;
}
AuthorNode *create_authors_list(BookNode *book_head) {
    AuthorNode *head = NULL, *tail = NULL, *curr_node;
    for (BookNode *book = book_head; book != NULL; book = book->next) {
        int found = 0;
        for (AuthorNode *author = head; author != NULL; author = author->next) {
            if (strcmp(book->data.author, author->data.name) == 0) {
                found = 1;
                author->data.book_num++;
                author->data.sum_avg_grade += book->data.avg_grade;
                author->data.avg_grade = author->data.sum_avg_grade / (author->data.book_num * 1.0);
                break;
            }
        }
        if (!found) {
            SAFE_MALLOC(curr_node, 1);
            strcpy(curr_node->data.name, book->data.author);
            curr_node->data.book_num = 1;
            curr_node->data.sum_avg_grade = book->data.avg_grade;
            curr_node->data.avg_grade = book->data.avg_grade;
            curr_node->next = NULL;
            if (head == NULL) head = curr_node;
            else tail->next = curr_node;
            tail = curr_node;
        }
    }
    return head;
}
GenreNode *create_genres_list(BookNode *book_head) {
    GenreNode *head = NULL, *tail = NULL, *curr_node;
    for (BookNode *book = book_head; book != NULL; book = book->next) {
        int found = 0;
        for (GenreNode *genre = head; genre != NULL; genre = genre->next) {
            if (strcmp(book->data.genre, genre->name) == 0) {
                found = 1;
                if (book->data.avg_grade > genre->best_book.avg_grade ||
                    (book->data.avg_grade == genre->best_book.avg_grade &&
                    strcmp(book->data.ISBN, genre->best_book.ISBN) < 0)) {
                    genre->best_book = book->data;
                }
                break;
            }
        }
        if (!found) {
            SAFE_MALLOC(curr_node, 1);
            strcpy(curr_node->name, book->data.genre);
            curr_node->best_book = book->data;
            curr_node->next = NULL;
            if (head == NULL) head = curr_node;
            else tail->next = curr_node;
            tail = curr_node;
        }
    }
    return head;
}
void sort_authors_list(AuthorNode *head) {
    for (AuthorNode *curr = head; curr != NULL; curr = curr->next) {
        for (AuthorNode *sec = head; sec->next != NULL; sec = sec->next) {
            if (strcmp(curr->data.name, sec->data.name) < 0) {
                Author temp = curr->data;
                curr->data = sec->data;
                sec->data = temp;
            }
        }
    }
}
void sort_genres_list(GenreNode *head) {
    for (GenreNode *curr = head; curr != NULL; curr = curr->next) {
        for (GenreNode *sec = curr->next; sec != NULL; sec = sec->next) {
            if (strcmp(curr->name, sec->name) > 0) {
                char temp_name[31];
                strcpy(temp_name, curr->name);
                strcpy(curr->name, sec->name);
                strcpy(sec->name, temp_name);
                Book temp_book = curr->best_book;
                curr->best_book = sec->best_book;
                sec->best_book = temp_book;
            }
        }
    }
}
Author get_most_influential_author(AuthorNode *head) {
    Author author = head->data;
    for (AuthorNode *curr = head->next; curr != NULL; curr = curr->next) {
        if (curr->data.book_num > author.book_num ||
            (curr->data.book_num == author.book_num &&
            curr->data.avg_grade > author.avg_grade)) {
            author = curr->data;
        }
    }
    return author;
}
void write_authors_list(AuthorNode *head, FILE *output) {
    for (AuthorNode *curr = head; curr != NULL; curr = curr->next) {
        fprintf(output, "%s,%d,%.2f", curr->data.name, curr->data.book_num, curr->data.avg_grade);
        if (curr->next != NULL) {
            fprintf(output, "\n");
        }
    }
}
void write_genres_list(GenreNode *head, FILE *output) {
    for (GenreNode *curr = head; curr != NULL; curr = curr->next) {
        fprintf(output, "%s,%s,%s,%.2f", curr->name, curr->best_book.ISBN, curr->best_book.title, curr->best_book.avg_grade);
        if (curr->next != NULL) {
            fprintf(output, "\n");
        }
    }
}
void free_lists(BookNode *books_head, AuthorNode *authors_head, GenreNode *genres_head) {
    while (books_head != NULL) {
        BookNode *temp = books_head;
        books_head = books_head->next;
        free(temp);
    }
    while (authors_head != NULL) {
        AuthorNode *temp = authors_head;
        authors_head = authors_head->next;
        free(temp);
    }
    while (genres_head != NULL) {
        GenreNode *temp = genres_head;
        genres_head = genres_head->next;
        free(temp);
    }
}
int main(const int argc, char *argv[]) {
    if (argc < 4) {
        printf("ARG_GRESKA");
        return 0;
    }
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("DAT_GRESKA");
        return 0;
    }
    FILE *authors_output = fopen(argv[2], "w");
    FILE *genres_output = fopen(argv[3], "w");
    BookNode *books_head = create_books_list(input);
    AuthorNode *authors_head = create_authors_list(books_head);
    sort_authors_list(authors_head);
    write_authors_list(authors_head, authors_output);
    GenreNode *genres_head = create_genres_list(books_head);
    sort_genres_list(genres_head);
    write_genres_list(genres_head, genres_output);
    Author author = get_most_influential_author(authors_head);
    printf("%s %d %.2f", author.name, author.book_num, author.avg_grade);
    free_lists(books_head, authors_head, genres_head);
    fclose(input);
    fclose(authors_output);
    fclose(genres_output);
    return 0;
}