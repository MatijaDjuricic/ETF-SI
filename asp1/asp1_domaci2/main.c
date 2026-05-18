#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N_MAT 4
#define MAX_LINE 256
#define STATES_DIR "./states/"
typedef enum {
    EMPTY = '.',
    PLAYER = 'X',
    AI = 'O',
} SYMBOL;
typedef enum {
    INF = 1000000,
    AI_WON = 1000,
    PLAYER_WON = -1000,
    DRAW = 0,
} MINIMAX;
typedef struct TreeNode {
    SYMBOL curr_player;
    char table[N_MAT][N_MAT];
    int score;
    int depth;
    int children_count;
    int move_col;
    struct TreeNode *children[N_MAT * N_MAT];
} TreeNode;
typedef struct StackNode {
    TreeNode *tree_node;
    int visited;
    struct StackNode *next;
} StackNode;
typedef struct {
    StackNode *top;
} Stack;
typedef struct QueueNode {
    TreeNode *tree_node;
    struct QueueNode *next;
} QueueNode;
typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;
TreeNode *create_tree_node(char table[N_MAT][N_MAT], SYMBOL curr_player) {
    TreeNode *node = malloc(sizeof(TreeNode));
    memcpy(node->table, table, sizeof(char) * N_MAT * N_MAT);
    node->curr_player = curr_player;
    node->depth = 0;
    node->score = 0;
    node->children_count = 0;
    node->move_col = 0;
    return node;
}
void add_child(TreeNode *parent, TreeNode *child) {
    parent->children[parent->children_count] = child;
    parent->children_count++;
}
StackNode *create_stack_node(TreeNode *node) {
    StackNode *s = malloc(sizeof(StackNode));
    s->tree_node = node;
    s->visited = 0;
    s->next = NULL;
    return s;
}
void push(Stack *s, TreeNode *node, int visited) {
    StackNode *new_node = create_stack_node(node);
    new_node->tree_node = node;
    new_node->visited = visited;
    new_node->next = s->top;
    s->top = new_node;
}
TreeNode *pop(Stack *s) {
    if (!s->top) return NULL;
    StackNode *temp = s->top;
    TreeNode *node = temp->tree_node;
    s->top = s->top->next;
    free(temp);
    return node;
}
QueueNode *create_queue_node(TreeNode *node) {
    QueueNode *q = malloc(sizeof(QueueNode));
    q->tree_node = node;
    q->next = NULL;
    return q;
}
void enqueue(Queue *q, TreeNode *node) {
    QueueNode *new_node = create_queue_node(node);
    if (!q->rear) {
        q->front = new_node;
        q->rear = new_node;
        return;
    }
    q->rear->next = new_node;
    q->rear = new_node;
}
TreeNode *dequeue(Queue *q) {
    if (!q->front) return NULL;
    QueueNode *temp = q->front;
    TreeNode *node = temp->tree_node;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free(temp);
    return node;
}
char check_winner(char table[N_MAT][N_MAT]) {
    for (int i = 0; i < N_MAT; i++) {
        for (int j = 0; j < N_MAT - 2; j++) {
            char curr = table[i][j];
            if (curr != EMPTY && curr == table[i][j + 1] && curr == table[i][j + 2]) {
                return curr;
            }
        }
    }
    for (int i = 0; i < N_MAT - 2; i++) {
        for (int j = 0; j < N_MAT; j++) {
            char curr = table[i][j];
            if (curr != EMPTY && curr == table[i + 1][j] && curr == table[i + 2][j]) {
                return curr;
            }
        }
    }
    for (int i = 0; i < N_MAT - 2; i++) {
        for (int j = 0; j < N_MAT - 2; j++) {
            char curr = table[i][j];
            if (curr != EMPTY && curr == table[i + 1][j + 1] && curr == table[i + 2][j + 2]) {
                return curr;
            }
        }
    }
    for (int i = 0; i < N_MAT - 2; i++) {
        for (int j = 2; j < N_MAT; j++) {
            char curr = table[i][j];
            if (curr != EMPTY && curr == table[i + 1][j - 1] && curr == table[i + 2][j - 2]) {
                return curr;
            }
        }
    }
    return EMPTY;
}
int check_draw(char table[N_MAT][N_MAT]) {
    if (check_winner(table) != EMPTY) return 0;
    for (int i = 0; i < N_MAT; i++) {
        for (int j = 0; j < N_MAT; j++) {
            if (table[i][j] == EMPTY) return 0;
        }
    }
    return 1;
}
void init_empty_table(char table[N_MAT][N_MAT]) {
    for (int i = 0; i < N_MAT; i++) {
        for (int j = 0; j < N_MAT; j++) {
            table[i][j] = EMPTY;
        }
    }
}
void input_table_state(char table[N_MAT][N_MAT]) {
    for (int i = 0; i < N_MAT; i++) {
        for (int j = 0; j < N_MAT; j++) {
            scanf(" %c", &table[i][j]);
        }
    }
}
int import_table_state(const char *filename, char table[N_MAT][N_MAT]) {
    char filepath[MAX_LINE], line[MAX_LINE];
    strcpy(filepath, STATES_DIR);
    strcat(filepath, filename);
    strcat(filepath, ".txt");
    FILE *f = fopen(filepath, "r");
    if (!f) return 0;
    for (int i = 0; i < N_MAT; i++) {
        fgets(line, sizeof(line), f);
        char *token = strtok(line, " ");
        int j = 0;
        while (token) {
            table[i][j] = *token;
            token = strtok(NULL, " ");
            j++;
        }
    }
    fclose(f);
    return 1;
}
int is_valid_table(char table[N_MAT][N_MAT]) {
    for (int j = 0; j < N_MAT; j++) {
        int found = 0;
        for (int i = 0; i < N_MAT; i++) {
            if (table[i][j] == PLAYER || table[i][j] == AI) {
                found = 1;
            }
            else if (table[i][j] == EMPTY) {
                if (found) {
                    return 0;
                }
            }
        }
    }
    return 1;
}
int is_terminal_table_state(char table[N_MAT][N_MAT]) {
    char winner = check_winner(table);
    if (winner == PLAYER || winner == AI) return 1;
    if (check_draw(table)) return 1;
    return 0;
}
void generate_tree(TreeNode *root, int h) {
    Queue q;
    q.front = NULL;
    q.rear = NULL;
    enqueue(&q, root);
    while (q.front) {
        TreeNode *node = dequeue(&q);
        if (node->depth >= h) continue;
        if (check_winner(node->table) != EMPTY) continue;
        for (int j = 0; j < N_MAT; j++) {
            int row = -1;
            for (int i = N_MAT - 1; i >= 0; i--) {
                if (node->table[i][j] == EMPTY) {
                    row = i;
                    break;
                }
            }
            if (row != -1) {
                char new_table[N_MAT][N_MAT];
                memcpy(new_table, node->table, N_MAT * N_MAT * sizeof(char));
                new_table[row][j] = node->curr_player;
                SYMBOL next_player = (node->curr_player == AI) ? PLAYER : AI;
                TreeNode *child = create_tree_node(new_table, next_player);
                child->depth = node->depth + 1;
                child->move_col = j;
                add_child(node, child);
                enqueue(&q, child);
            }
        }
    }
}
void free_tree(TreeNode **root_ptr) {
    if (!(*root_ptr)) return;
    Stack s;
    s.top = NULL;
    push(&s, *root_ptr, 0);
    while (s.top) {
        TreeNode *node = pop(&s);
        for (int i = 0; i < node->children_count; i++) {
            if (node->children[i]) {
                push(&s, node->children[i], 0);
            }
        }
        free(node);
    }
    *root_ptr = NULL;
}
int heuristic_position(char table[N_MAT][N_MAT]) {
    int j, vc = 0, vp = 0;
    for (int i = 0; i < N_MAT; i++) {
        for (j = 1; j <= 2; j++) {
            if (table[i][j] == AI) vc++;
            else if (table[i][j] == PLAYER) vp++;
        }
    }
    for (j = 0; j < N_MAT; j++) {
        if (table[N_MAT - 1][j] == AI) vc++;
        else if (table[N_MAT - 1][j] == PLAYER) vp++;
    }
    return 5 * (vc - vp);
}
void make_move(char table[N_MAT][N_MAT], int col, SYMBOL curr_player) {
    for (int i = N_MAT - 1; i >= 0; i--) {
        if (table[i][col] == EMPTY) {
            table[i][col] = curr_player;
            break;
        }
    }
}
void make_ai_move(char table[N_MAT][N_MAT], TreeNode **root_ptr) {
    TreeNode *root = *root_ptr;
    if (!root || root->children_count == 0) {
        int col;
        do {
            col = rand() % N_MAT;
        } while (table[0][col] != EMPTY);
        make_move(table, col, AI);
        return;
    }
    int best_score = -INF;
    int best_idx[N_MAT * N_MAT];
    int best_count = 0;
    for (int i = 0; i < root->children_count; i++) {
        int score = root->children[i]->score;
        if (score > best_score) {
            best_score = score;
            best_idx[0] = i;
            best_count = 1;
        }
        else if (score == best_score) {
            best_idx[best_count] = i;
            best_count++;
        }
    }
    int chosen = best_idx[rand() % best_count];
    TreeNode *best_move = root->children[chosen];
    memcpy(table, best_move->table, sizeof(char) * N_MAT * N_MAT);
    *root_ptr = best_move;
}
SYMBOL next_player_from_table(char table[N_MAT][N_MAT]) {
    int player_count = 0, ai_count = 0;
    for (int i = 0; i < N_MAT; i++) {
        for (int j = 0; j < N_MAT; j++) {
            if (table[i][j] == PLAYER) player_count++;
            else if (table[i][j] == AI) ai_count++;
        }
    }
    return (player_count <= ai_count) ? PLAYER : AI;
}
void minimax(TreeNode *root) {
    if (!root) return;
    Stack s;
    s.top = NULL;
    push(&s, root, 0);
    while (s.top) {
        StackNode *stack_node = s.top;
        TreeNode *node = stack_node->tree_node;
        char winner = check_winner(node->table);
        if (winner == PLAYER) {
            node->score = PLAYER_WON;
            pop(&s);
            continue;
        }
        if (winner == AI) {
            node->score = AI_WON;
            pop(&s);
            continue;
        }
        if (check_draw(node->table)) {
            node->score = DRAW;
            pop(&s);
            continue;
        }
        if (node->children_count == 0) {
            node->score = heuristic_position(node->table);
            pop(&s);
            continue;
        }
        if (!stack_node->visited) {
            for (int i = node->children_count - 1; i >= 0; i--) {
                push(&s, node->children[i], 0);
            }
            stack_node->visited = 1;
        } else {
            if (node->curr_player == AI) {
                int best = -INF;
                for (int i = 0; i < node->children_count; i++) {
                    if (node->children[i]->score > best) {
                        best = node->children[i]->score;
                    }
                }
                node->score = best;
            } else {
                int best = INF;
                for (int i = 0; i < node->children_count; i++) {
                    if (node->children[i]->score < best) {
                        best = node->children[i]->score;
                    }
                }
                node->score = best;
            }
            pop(&s);
        }
    }
}
void print_table_state(char table[N_MAT][N_MAT]) {
    for (int i = 0; i < N_MAT; i++) {
        putchar('|');
        for (int j = 0; j < N_MAT; j++) {
            printf(" %c ", table[i][j]);
        }
        printf("|\n");
    }
    printf("--------------\n");
}
void print_main_manu() {
    printf("===== MENI IGRE POVEZI 3 =====\n");
    printf("1. Igraj protiv AI\n");
    printf("2. Unesi trenutno stanje (rucno - matricno)\n");
    printf("3. Unesi trenutno stanje (uvezi fajl - import file)\n");
    printf("4. Prikazi trenutno stanje\n");
    printf("5. Prikazi stablo (level-order, broj-cvorova)\n");
    printf("6. Resetuj stanje table i stabla (kreni ispocetka)\n");
    printf("0. Izlaz\n");
}
void print_play_manu() {
    printf("=== SIMULACIJA IGRE POVEZI 3 ===\n");
    printf("=== TRENUTNO STANJE TABLE ===\n");
    printf("=== Igrac (%c) - AI (%c) ===\n", PLAYER, AI);
    printf("1. Postavi %c u prvu kolonu\n", PLAYER);
    printf("2. Postavi %c u drugu kolonu\n", PLAYER);
    printf("3. Postavi %c u trecu kolonu\n", PLAYER);
    printf("4. Postavi %c u cetvrtu kolonu\n", PLAYER);
    printf("0. Izlaz (prikazi glavi meni)\n");
}
void print_tree_level_order_info(TreeNode *root) {
    if (!root) return;
    Queue q;
    q.front = NULL;
    q.rear = NULL;
    enqueue(&q, root);
    int curr_depth = -1, node_num = 0;
    while (q.front) {
        TreeNode *node = dequeue(&q);
        if (!node) continue;
        if (node->depth != curr_depth) {
            curr_depth = node->depth;
            printf("\n=== NIVO %d ===\n", curr_depth);
        }
        print_table_state(node->table);
        node_num++;
        for (int i = 0; i < node->children_count; i++) {
            if (node->children[i]) {
                enqueue(&q, node->children[i]);
            }
        }
    }
    printf("Broj cvorova stabla: %d\n", node_num);
}
TreeNode *find_next_root(TreeNode *root, char table[N_MAT][N_MAT]) {
    if (!root || root->children_count == 0) return NULL;
    for (int i = 0; i < root->children_count; i++) {
        if (memcmp(root->children[i]->table, table, sizeof(char) * N_MAT * N_MAT) == 0) {
            return root->children[i];
        }
    }
    return NULL;
}
void play_game(char table[N_MAT][N_MAT], TreeNode *root) {
    print_play_manu();
    print_table_state(table);
    TreeNode *curr_root = root;
    int move;
    while (1) {
        printf("Unesi potez(1,2,3,4,0): ");
        scanf("%d", &move);
        if (move == 0) break;
        if (move < 1 || move > N_MAT) {
            printf("Nevalidan potez\n");
            continue;
        }
        if (table[0][move - 1] != EMPTY) {
            printf("Kolona je puna\n");
            continue;
        }
        make_move(table, move - 1, PLAYER);
        printf("Igrac(%c):\n", PLAYER);
        print_table_state(table);
        if (check_winner(table) == PLAYER) {
            printf("Pobednik: Igrac(%c)\n", PLAYER);
            break;
        }
        if (check_draw(table)) {
            printf("Nereseno\n");
            break;
        }
        curr_root = find_next_root(curr_root, table);
        make_ai_move(table, &curr_root);
        printf("AI(%c):\n", AI);
        print_table_state(table);
        if (check_winner(table) == AI) {
            printf("Pobednik: AI(%c)\n", AI);
            break;
        }
        if (check_draw(table)) {
            printf("Nereseno\n");
            break;
        }
    }
}
int main() {
    TreeNode *root = NULL;
    char table[N_MAT][N_MAT];
    int option, h = 0;
    srand(time(NULL));
    init_empty_table(table);
    print_main_manu();
    while (1) {
        printf("Unesi opciju(1,2,3,4,5,6,0): ");
        scanf("%d", &option);
        if (option == 0) break;
        switch (option) {
            case 1: {
                if (!h) {
                    while (1) {
                        printf("Unesi maksimalnu visinu stabla (h): ");
                        scanf("%d", &h);
                        if (h < 1 || h > N_MAT * N_MAT) {
                            printf("Parametar (h) mora biti u opsegu [1, %d]\n", N_MAT * N_MAT);
                            continue;
                        }
                        break;
                    }
                }
                free_tree(&root);
                SYMBOL next_player = next_player_from_table(table);
                root = create_tree_node(table, next_player);
                generate_tree(root, h);
                minimax(root);
                play_game(table, root);
                break;
            }
            case 2: {
                free_tree(&root);
                init_empty_table(table);
                while (1) {
                    printf("Unesi matricu stanja (.,X,O) - %dx%d:\n", N_MAT, N_MAT);
                    input_table_state(table);
                    if (!is_valid_table(table)) {
                        printf("Nevalidno stanje table (krsenje gravitacije)\n");
                        continue;
                    }
                    if (is_terminal_table_state(table)) {
                        printf("Nevalidno stanje table za pocetak igre (terminalno stanje table)\n");
                        continue;
                    }
                    break;
                }
                print_table_state(table);
                while (1) {
                    printf("Unesi maksimalnu visinu stabla (h): ");
                    scanf("%d", &h);
                    if (h < 1 || h > N_MAT * N_MAT) {
                        printf("Parametar (h) mora biti u opsegu [1, %d]\n", N_MAT * N_MAT);
                        continue;
                    }
                    break;
                }
                SYMBOL next_player = next_player_from_table(table);
                root = create_tree_node(table, next_player);
                generate_tree(root, h);
                minimax(root);
                break;
            }
            case 3: {
                char filename[MAX_LINE];
                if (root) {
                    free_tree(&root);
                    init_empty_table(table);
                }
                while (1) {
                    printf("Unesi naziv tekstualnog fajla (<naziv_fajla>.txt): ");
                    scanf("%s", filename);
                    if (!import_table_state(filename, table)) {
                        printf("Pogresan naziv tekstualnog fajla\n");
                        continue;
                    }
                    if (!is_valid_table(table)) {
                        printf("Nevalidno stanje table (krsenje gravitacije)\n");
                        continue;
                    }
                    if (is_terminal_table_state(table)) {
                        printf("Nevalidno stanje table za pocetak igre (terminalno stanje table)\n");
                        continue;
                    }
                    break;
                }
                print_table_state(table);
                while (1) {
                    printf("Unesi maksimalnu visinu stabla (h): ");
                    scanf("%d", &h);
                    if (h < 1 || h > N_MAT * N_MAT) {
                        printf("Parametar (h) mora biti u opsegu [1, %d]\n", N_MAT * N_MAT);
                        continue;
                    }
                    break;
                }
                SYMBOL next_player = next_player_from_table(table);
                root = create_tree_node(table, next_player);
                generate_tree(root, h);
                minimax(root);
                break;
            }
            case 4: {
                print_table_state(table);
                break;
            }
            case 5: {
                print_tree_level_order_info(root);
                break;
            }
            case 6: {
                free_tree(&root);
                init_empty_table(table);
                h = 0;
                break;
            }
            default: {
                printf("Nepostojeca opcija\n");
                break;
            }
        }
    }
    free_tree(&root);
    return 0;
}