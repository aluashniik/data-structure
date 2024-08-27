#include <stdio.h>
#include <stdlib.h>

#define ROWS 10
#define COLS 10

typedef struct Node {
    int x, y; //coordinates
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct DoublyLinkedList {
    Node *head; 
    Node *tail;
} DoublyLinkedList;

int maze[ROWS][COLS];

void read (const char *filename) {
    FILE *file = fopen(filename, "r");
    /*미로 입력 받기*/
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, "%d", &maze[i][j]);
        }
    }
    fclose(file);
}

int find(int x, int y, int mark[ROWS][COLS], int path[ROWS * COLS][2], int *pathlen) {
    if (x == ROWS - 2 && y == COLS - 2) { //if the current position is the dest
        path[*pathlen][0] = x;
        path[*pathlen][1] = y;
        (*pathlen)++;
        return 1;
    }

    if (x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0 && !mark[x][y]) {//check if the curr pos is valid and not visited
        mark[x][y] = 1;//if curr pos has visited
        path[*pathlen][0] = x;
        path[*pathlen][1] = y;
        (*pathlen)++;

        //8개 방향으로 탐색
        int dir[8][2] = {
            {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, //N, NE, E, SE
            {1, 0}, {1, -1}, {0, -1}, {-1, -1}//S, SW, W, NW
        };

        for (int i = 0; i < 8; i++) {
            int new_x = x + dir[i][0];
            int new_y = y + dir[i][1];
            if (find(new_x, new_y, mark, path, pathlen)) {//call the find func for the pos
                return 1;//if found return 1
            }
        }
        (*pathlen)--;
        mark[x][y] = 0;
    }
    return 0;
}

DoublyLinkedList *create_list() {
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void append(DoublyLinkedList *list, int x, int y) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->x = x;
    new_node->y = y;
    new_node->prev = list->tail;
    new_node->next = NULL;
    if (list->tail) {//if list is not empty
        list->tail->next = new_node;//last node to new
    } else {
        list->head = new_node;//if list is empty head pointer the new
    }
    list->tail = new_node;//update
}

void free_list(DoublyLinkedList *list) {
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void write(const char *filename, DoublyLinkedList *list) {
    FILE *file = fopen(filename, "w");
    Node *current = list->head;//starting from the head
    while (current) {
        fprintf(file, "%d %d\n", current->x, current->y); 
        current = current->next; //write coord to the file moving to the nexxt node
    }
    fclose(file);
}

int main() {
    read("maze.txt");

    int mark[ROWS][COLS] = {0};
    int path[ROWS * COLS][2];
    int path_index = 0;

    if (find(1, 1, mark, path, &path_index)) { // (1, 1)에서 시작해서 길 찾아
        DoublyLinkedList *list = create_list();//새러운 리스트 만들기
        for (int i = 0; i < path_index; i++) {
            append(list, path[i][0], path[i][1]);//append pos to the list
        }
        write("path.txt", list);
        free_list(list);
    } else {
        printf("“The maze does not have a path.\n");//길이 없다면
    }

    return 0;
}

