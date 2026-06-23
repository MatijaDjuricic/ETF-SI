#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define INF 1000000
#define MAX_NODES 100
#define MAX_LINES 256
#define DICTS_DIR "./dicts/"
#define END_WORD "kaladont"
typedef enum {
    UTILITY,
    EFFICIENCY,
    DANGER
} CALC_ENUM;
typedef struct stack {
    int *nodes;
    int top;
    int capacity;
} Stack;
typedef struct graph {
    int **matrix;
    int curr_size;
    int max_capacity;
} Graph;
typedef struct {
    Graph *graph;
    char words[MAX_NODES][MAX_LINES];
    int num_words;
} Kaladont;
Stack create_stack(int capacity) {
    Stack s;
    s.nodes = malloc(capacity * sizeof(int));
    s.top = -1;
    s.capacity = capacity;
    return s;
}
void push(Stack *s, int value) {
    if (s->top == s->capacity - 1) return;
    s->nodes[++(s->top)] = value;
}
int pop(Stack *s) {
    return s->nodes[(s->top)--];
}
int is_stack_empty(Stack *s) {
    return s->top == -1;
}
Graph *create_graph(int n) {
    Graph *graph = malloc(sizeof(Graph));
    graph->curr_size = n;
    graph->max_capacity = MAX_NODES;
    graph->matrix = malloc(MAX_NODES * sizeof(int*));
    for (int i = 0; i < MAX_NODES; i++) {
        graph->matrix[i] = calloc(MAX_NODES,sizeof(int));
    }
    return graph;
}
void add_graph_node(Graph *graph) {
    if (!graph) {
        printf("Graf nije kreiran (opcija 1)\n");
        return;
    }
    if (graph->curr_size >= graph->max_capacity) return;
    int new_node = graph->curr_size;
    for (int i = 0; i <= new_node; i++) {
        graph->matrix[new_node][i] = 0;
        graph->matrix[i][new_node] = 0;
    }
    graph->curr_size++;
    printf("Uspesno dodat cvor %d u graf\n", new_node);
}
void add_graph_edge(Graph *graph, int u, int v) {
    if (!graph) {
        printf("Graf nije kreiran (opcija 1)\n");
        return;
    }
    if (u < 0 || u >= graph->curr_size || v < 0 || v >= graph->curr_size) {
        printf("Indeksi moraju biti izmedju 0 i %d\n", graph->curr_size - 1);
        return;
    }
    if (graph->matrix[u][v] == 1) {
        printf("Grana %d -> %d vec postoji.\n", u, v);
        return;
    }
    graph->matrix[u][v] = 1;
    printf("Uspesno dodata grana: %d -> %d (u -> v)\n", u, v);
}
void remove_graph_node(Graph *graph, int node) {
    if (!graph) {
        printf("Graf nije kreiran (opcija 1)\n");
        return;
    }
    if (node < 0 || node >= graph->curr_size) {
        printf("Nepostojeci cvorovi\n");
        return;
    }
    int last_node = graph->curr_size - 1;
    if (node != last_node) {
        for (int i = 0; i < graph->curr_size; i++) {
            graph->matrix[node][i] = graph->matrix[last_node][i];
            graph->matrix[i][node] = graph->matrix[i][last_node];
        }
        graph->matrix[node][node] = graph->matrix[last_node][last_node];
    }
    graph->curr_size--;
    printf("Uspesno uklonjen cvor %d iz grafa\n", node);
}
void remove_graph_edge(Graph *graph, int u, int v) {
    if (!graph) {
        printf("Graf nije kreiran (opcija 1)\n");
        return;
    }
    if (u < 0 || u >= graph->curr_size || v < 0 || v >= graph->curr_size) {
        printf("Indeksi moraju biti izmedju 0 i %d\n", graph->curr_size - 1);
        return;
    }
    if (graph->matrix[u][v] == 0) {
        printf("Grana %d -> %d ne postoji\n", u, v);
        return;
    }
    graph->matrix[u][v] = 0;
    printf("Uspesno uklonjena grana: %d -> %d (u -> v)\n", u, v);
}
void print_graph(Graph *graph) {
    if (!graph) {
        printf("Graf nije kreiran (opcija 1)\n");
        return;
    }
    printf("Matrica susednosti (aktivna velicina: %d x %d):\n", graph->curr_size, graph->curr_size);
    printf("    ");
    for (int i = 0; i < graph->curr_size; i++) printf("%d ", i);
    printf("\n");
    for (int i = 0; i < graph->curr_size; i++) {
        printf("%d | ", i);
        for (int j = 0; j < graph->curr_size; j++) {
            printf("%d ", graph->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void free_graph(Graph *graph) {
    if (!graph) return;
    for (int i = 0; i < graph->max_capacity; i++) {
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    free(graph);
}
void create_kaladont_graph(Kaladont *kaladont) {
    kaladont->graph = create_graph(kaladont->num_words);
    for (int i = 0; i < kaladont->num_words; i++) {
        for (int j = 0; j < kaladont->num_words; j++) {
            if (i == j) continue;
            if (kaladont->words[i][strlen(kaladont->words[i]) - 2] == kaladont->words[j][0] &&
                kaladont->words[i][strlen(kaladont->words[i]) - 1] == kaladont->words[j][1]) {
                add_graph_edge(kaladont->graph, i, j);
            }
        }
    }
}
int import_kaladont_dictionary(const char *filename, Kaladont *kaladont) {
    char filepath[MAX_LINES];
    strcpy(filepath, DICTS_DIR);
    strcat(filepath, filename);
    strcat(filepath, ".txt");
    FILE *f = fopen(filepath, "r");
    if (!f) return 0;
    if (fscanf(f, "%d", &kaladont->num_words) != 1) {
        fclose(f);
        return 0;
    }
    printf("Broj reci u recniku: %d\n", kaladont->num_words);
    for (int i = 0; i < kaladont->num_words && i < MAX_NODES; i++) {
        if (fscanf(f, "%s", kaladont->words[i]) == 1) {
            printf("%d: %s\n", i, kaladont->words[i]);
        }
    }
    if (kaladont->graph) {
        free_graph(kaladont->graph);
        kaladont->graph = NULL;
    }
    create_kaladont_graph(kaladont);
    print_graph(kaladont->graph);
    fclose(f);
    return 1;
}
int calc_utility(Kaladont *kaladont, int start_node) {
    if (!kaladont || !kaladont->graph) return 0;
    if (start_node < 0 || start_node >= kaladont->graph->curr_size) return 0;
    int n = kaladont->graph->curr_size;
    int *visited = calloc(n, sizeof(int));
    Stack s = create_stack(n);
    int visited_cnt = 0;
    visited[start_node] = 1;
    push(&s, start_node);
    while (!is_stack_empty(&s)) {
        int curr = pop(&s);
        visited_cnt++;
        for (int i = 0; i < n; i++) {
            if (kaladont->graph->matrix[curr][i] == 1 && !visited[i]) {
                visited[i] = 1;
                push(&s, i);
            }
        }
    }
    free(visited);
    free(s.nodes);
    return visited_cnt - 1;
}
int calc_efficiency(Kaladont *kaladont, int start_node) {
    if (!kaladont || !kaladont->graph) return 0;
    if (start_node < 0 || start_node >= kaladont->graph->curr_size) return 0;
    int n = kaladont->graph->curr_size;
    int *distance = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) distance[i] = INF;
    distance[start_node] = 0;
    Stack s = create_stack(n);
    push(&s, start_node);
    while (!is_stack_empty(&s)) {
        int curr = pop(&s);
        for (int i = 0; i < n; i++) {
            if (kaladont->graph->matrix[curr][i] == 1 && distance[curr] + 1 < distance[i]) {
                distance[i] = distance[curr] + 1;
                push(&s, i);
            }
        }
    }
    int max_shortest_path = 0;
    for (int i = 0; i < n; i++) {
        if (distance[i] != INF && distance[i] > max_shortest_path) {
            max_shortest_path = distance[i];
        }
    }
    free(s.nodes);
    free(distance);
    return max_shortest_path;
}
int calc_danger(Kaladont *kaladont, int start_node) {
    if (!kaladont || !kaladont->graph) return 0;
    if (start_node < 0 || start_node >= kaladont->graph->curr_size) return 0;
    int n = kaladont->graph->curr_size;
    int have_exit = 0;
    for (int i = 0; i < n; i++) {
        if (kaladont->graph->matrix[start_node][i] == 1) {
            have_exit = 1;
            break;
        }
    }
    if (!have_exit) return 1;
    int *distance = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) distance[i] = INF;
    distance[start_node] = 0;
    Stack s = create_stack(n);
    push(&s, start_node);
    while (!is_stack_empty(&s)) {
        int curr = pop(&s);
        for (int i = 0; i < n; i++) {
            if (kaladont->graph->matrix[curr][i] == 1 && distance[curr] + 1 < distance[i]) {
                distance[i] = distance[curr] + 1;
                push(&s, i);
            }
        }
    }
    int min_danger_path = INF;
    for (int i = 0; i < n; i++) {
        int is_exit = 0;
        for (int j = 0; j < n; j++) {
            if (kaladont->graph->matrix[i][j] == 1) {
                is_exit = 1;
                break;
            }
        }
        if (distance[i] != INF && !is_exit && distance[i] < min_danger_path) {
            min_danger_path = distance[i];
        }
    }
    free(s.nodes);
    free(distance);
    if (min_danger_path != INF) return min_danger_path + 1;
    return 0;
}
int find_word_index(Kaladont *kaladont, const char *word) {
    for (int i = 0; i < kaladont->num_words; i++) {
        if (strcmp(kaladont->words[i], word) == 0) {
            return i;
        }
    }
    return -1;
}
int is_kaladont_valid(const char *curr_word, const char *next_word) {
    int len = strlen(curr_word);
    if (len < 2 || strlen(next_word) < 2) return 0;
    if (curr_word[len - 2] == next_word[0] && curr_word[len - 1] == next_word[1]) {
        return 1;
    }
    return 0;
}
void calc_wrapper(Kaladont *kaladont, CALC_ENUM calc_type) {
    if (!kaladont->graph) {
        printf("Recnik nije unet (opcija 1)\n");
        return;
    }
    char target_word[MAX_LINES], calc_word[MAX_LINES];
    int node, calc;
    while (1) {
        printf("Unesi rec: ");
        scanf("%s", target_word);
        node = find_word_index(kaladont, target_word);
        if (node != -1) break;
        printf("Rec '%s' ne postoji u recniku\n", target_word);
    }
    switch (calc_type) {
        case UTILITY:
            calc = calc_utility(kaladont, node);
            strcpy(calc_word, "KORISNOST");
            break;
        case EFFICIENCY:
            calc = calc_efficiency(kaladont, node);
            strcpy(calc_word, "EFIKASNOST");
            break;
        case DANGER:
            calc = calc_danger(kaladont, node);
            strcpy(calc_word, "OPASNOST");
            break;
    }
    printf("%s reci '%s': %d\n", calc_word, target_word, calc);
}
void kaladont_game(Kaladont *kaladont) {
    if (!kaladont->graph || kaladont->num_words == 0) {
        printf("Nema ucitanog recnika\n");
        return;
    }
    char input[MAX_LINES];
    int used[MAX_NODES] = {0};
    int curr = rand() % kaladont->graph->curr_size;
    used[curr] = 1;
    printf("Pocetna rec: %s\n", kaladont->words[curr]);
    while (1) {
        printf("\nUnesi rec ili komandu ('?' = pomoc, '0' = izlaz): ");
        scanf("%s", input);
        if (strcmp(input, "0") == 0) break;
        if (strcmp(input, "?") == 0) {
            printf("\n==?== POMOC ZA REC '%s' ==?==\n", kaladont->words[curr]);
            printf("KORISNOST: %d\n", calc_utility(kaladont, curr));
            printf("EFIKASNOST: %d\n", calc_efficiency(kaladont, curr));
            printf("OPASNOST: %d\n", calc_danger(kaladont, curr));
            continue;
        }
        int next = find_word_index(kaladont, input);
        if (next == -1) {
            printf("Rec ne postoji u unetom recniku\n");
            printf("Pobednik: AI (%s)\n", END_WORD);
            break;
        }
        if (used[next]) {
            printf("Rec je vec iskoriscena\n");
            printf("Pobednik: AI (%s)\n", END_WORD);
            break;
        }
        if (!is_kaladont_valid(kaladont->words[curr], kaladont->words[next])) {
            printf("Pobednik: AI (%s)\n", END_WORD);
            break;
        }
        curr = next;
        used[curr] = 1;
        printf("Sledeca rec: %s\n", kaladont->words[curr]);
        int candidates[MAX_NODES], cnt = 0;
        for (int i = 0; i < kaladont->graph->curr_size; i++) {
            if (!used[i] && kaladont->graph->matrix[curr][i] == 1) {
                candidates[cnt++] = i;
            }
        }
        if (cnt == 0) {
            printf("Pobednik: Igrac\n");
            break;
        }
        int ai = candidates[rand() % cnt];
        used[ai] = 1;
        curr = ai;
        printf("AI (rec): '%s'\n", kaladont->words[curr]);
        if (strcmp(kaladont->words[curr], END_WORD) == 0) {
            printf("Pobednik: AI (%s)\n", END_WORD);
            break;
        }
    }
}
void print_main_manu() {
    printf("===== GLAVNI MENI =====\n");
    printf("1. Zadatak 1: Graf - osnovni algoritmi\n");
    printf("2. Zadatak 2: Igra Kaladont\n");
    printf("0. Izlaz\n");
}
void print_graph_manu() {
    printf("===== MENI GRAF - OSNOVNI ALGORITMI =====\n");
    printf("1. Kreiranje prazne strukture (dimenzije grafa)\n");
    printf("2. Dodavanje cvorova u graf\n");
    printf("3. Uklanjanje cvorova iz grafa\n");
    printf("4. Dodavanje grane izmedju dva cvora u grafu\n");
    printf("5. Uklanjanje grane izmedju dva cvora u grafu\n");
    printf("6. Ispis reprezentacije grafa\n");
    printf("7. Brisanje grafa iz memorije\n");
    printf("0. Izlaz\n");
}
void print_kaladont_manu() {
    printf("===== MENI IGRE KALADONT =====\n");
    printf("1. Kreiranje grafa iz ulazne tekstualne datoteke (recnik - uvezi fajl)\n");
    printf("2. Korisnost zadate reci\n");
    printf("3. Efikasnost zadate reci\n");
    printf("4. Opasnost zadate reci\n");
    printf("5. Simulacija igre - kaladont\n");
    printf("0. Izlaz\n");
}
void task1(Graph **graph) {
    int option;
    print_graph_manu();
    while (1) {
        printf("Unesi opciju(1,2,3,4,5,6,7,0): ");
        scanf("%d", &option);
        if (option == 0) {
            print_main_manu();
            break;
        }
        switch (option) {
            case 1: {
                if (*graph) {
                    free_graph(*graph);
                    *graph = NULL;
                }
                int n;
                printf("Unesi velicinu grafa (n): ");
                scanf("%d", &n);
                *graph = create_graph(n);
                break;
            }
            case 2: {
                add_graph_node(*graph);
                break;
            }
            case 3: {
                int node;
                printf("Unesi index cvora grafa: ");
                scanf("%d", &node);
                remove_graph_node(*graph, node);
                break;
            }
            case 4: {
                int u, v;
                printf("Unesi index prvog cvora grafa (u): ");
                scanf("%d", &u);
                printf("Unesi index drugog cvora grafa (v): ");
                scanf("%d", &v);
                add_graph_edge(*graph, u, v);
                break;
            }
            case 5: {
                int u, v;
                printf("Unesi index prvog cvora grafa (u): ");
                scanf("%d", &u);
                printf("Unesi index drugog cvora grafa (v): ");
                scanf("%d", &v);
                remove_graph_edge(*graph, u, v);
                break;
            }
            case 6: {
                print_graph(*graph);
                break;
            }
            case 7: {
                free_graph(*graph);
                *graph = NULL;
                break;
            }
            default: {
                printf("Nepostojeca opcija\n");
                break;
            }
        }
    }
}
void task2(Kaladont **kaladont) {
    if (!*kaladont) {
        *kaladont = malloc(sizeof(Kaladont));
        (*kaladont)->graph = NULL;
        (*kaladont)->num_words = 0;
    }
    int option;
    print_kaladont_manu();
    while (1) {
        printf("Unesi opciju(1,2,3,4,5,0): ");
        scanf("%d", &option);
        if (option == 0) {
            print_main_manu();
            break;
        }
        switch (option) {
            case 1: {
                char filename[MAX_LINES];
                while (1) {
                    printf("Unesi naziv tekstualnog fajla (<naziv_fajla>.txt): ");
                    scanf("%s", filename);
                    if (!import_kaladont_dictionary(filename, *kaladont)) {
                        printf("Pogrsan naziv tekstualnog fajla\n");
                        continue;
                    }
                    break;
                }
                break;
            }
            case 2: {
                calc_wrapper(*kaladont, UTILITY);
                break;
            }
            case 3: {
                calc_wrapper(*kaladont, EFFICIENCY);
                break;
            }
            case 4: {
                calc_wrapper(*kaladont, DANGER);
                break;
            }
            case 5: {
                kaladont_game(*kaladont);
                break;
            }
            default: {
                printf("Nepostojeca opcija\n");
                break;
            }
        }
    }
}
int main() {
    Graph *graph = NULL;
    Kaladont *kaladont = NULL;
    int option;
    srand(time(NULL));
    print_main_manu();
    while (1) {
        printf("Unesi opciju(1,2,0): ");
        scanf("%d", &option);
        if (option == 0) break;
        if (option == 1) task1(&graph);
        else if (option == 2) task2(&kaladont);
        else printf("Nepostojeca opcija\n");
    }
    if (graph) {
        free_graph(graph);
        graph = NULL;
    }
    if (kaladont) {
        if (kaladont->graph) {
            free_graph(kaladont->graph);
            kaladont->graph = NULL;
        }
        free(kaladont);
        kaladont = NULL;
    }
    return 0;
}