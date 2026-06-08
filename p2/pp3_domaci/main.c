#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK 50
#define MAX_LINE 256
#define SAFE_MALLOC(ptr, n) \
ptr = malloc((n) * sizeof(*ptr)); \
if (!ptr) { \
printf("MEM_GRESKA"); \
}
#define SAFE_REALLOC(data, n) \
void *ptr = realloc(data, (n) * sizeof(*data)); \
if (ptr == NULL) { \
free(data); \
printf("MEM_GRESKA"); \
} \
data = ptr
char** read_lines() {
    char **lines, *line, c;
    int line_len = 0;
    SAFE_MALLOC(lines, CHUNK);
    while (1) {
        SAFE_MALLOC(line, MAX_LINE);
        int c_cnt = 0;
        while ((c = getchar()) != '\n' && c != EOF) {
            line[c_cnt++] = c;
        }
        line[c_cnt] = '\0';
        if (c_cnt == 0) {
            free(line);
            break;
        }
        SAFE_REALLOC(line, c_cnt + 1);
        lines[line_len++] = line;
    }
    if (line_len == 0) {
        free(lines);
        printf("GRESKA");
        return NULL;
    }
    SAFE_REALLOC(lines, line_len + 1);
    lines[line_len] = NULL;
    return lines;
}
char *convert_markdown_links(char *str) {
    char *out;
    SAFE_MALLOC(out, MAX_LINE);
    out[0] = '\0';
    char *curr = str;
    while (1) {
        char *ob = strchr(curr, '[');
        char *cb = strchr(curr, ']');
        char *op = strchr(curr, '(');
        char *cp = strchr(curr, ')');
        if (ob && cb && op && cp && ob < cb && cb + 1 == op && op < cp) {
            strncat(out, curr, ob - curr);
            int text_len = cb - (ob + 1);
            int url_len  = cp - (op + 1);
            char text[MAX_LINE];
            char url[MAX_LINE];
            memcpy(text, ob + 1, text_len);
            text[text_len] = '\0';
            memcpy(url, op + 1, url_len);
            url[url_len] = '\0';
            char link[MAX_LINE];
            snprintf(link, sizeof(link),"<a href=\"%s\">%s</a>", url, text);
            strcat(out, link);
            curr = cp + 1;
        } else {
            strcat(out, curr);
            break;
        }
    }
    return out;
}
char** convert_text_links(char** lines) {
    char **new_lines;
    int len = 0;
    SAFE_MALLOC(new_lines, CHUNK);
    while (lines[len] != NULL) {
        new_lines[len] = convert_markdown_links(lines[len]);
        len++;
    }
    SAFE_REALLOC(new_lines, len + 1);
    new_lines[len] = NULL;
    return new_lines;
}
void print_lines(char** lines) {
    for (int i = 0; lines[i] != NULL; i++) {
        printf("%s", lines[i]);
        if (lines[i + 1] != 0) {
            putchar('\n');
        }
    }
}
void free_lines(char** lines) {
    for (int i = 0; lines[i] != NULL; i++) {
        free(lines[i]);
    }
    free(lines);
}
int main() {
    char **lines = read_lines();
    if (!lines) return 0;
    print_lines(lines);
    putchar('\n');
    char **new_lines = convert_text_links(lines);
    print_lines(new_lines);
    free_lines(lines);
    free_lines(new_lines);
    return 0;
}