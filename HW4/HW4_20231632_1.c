#include <stdio.h>
#include <stdlib.h>

/*강의자료 코드 참고 */
#define MAX_SIZE 50 /* size of largest matrix */

typedef enum { head, entry } tagfield;
typedef struct matrix_node *matrix_pointer;

typedef struct entry_node {
    int row;
    int col;
    int value;
} entry_node;

typedef struct matrix_node {
    matrix_pointer down;
    matrix_pointer right;
    tagfield tag;
    union {
        matrix_pointer next;
        entry_node entry;
    } u;
} matrix_node;

matrix_pointer hdnode[MAX_SIZE];
matrix_pointer hdnode_t[MAX_SIZE]; // Transposed matrix header nodes
//////////////////////////////////////////////
matrix_pointer new_node() {
    return (matrix_pointer)malloc(sizeof(matrix_node));
}

matrix_pointer mread() {
    /*readinng input file*/
    FILE *input = fopen("input.txt", "r");

    int num_rows, num_cols, num_terms, num_heads, i;
    int row, col, value, current_row;
    matrix_pointer temp, last, node;

    fscanf(input, "%d %d %d", &num_rows, &num_cols, &num_terms);

    num_heads = (num_cols > num_rows) ? num_cols : num_rows;

    node = new_node();
    node->tag = entry;
    node->u.entry.row = num_rows;
    node->u.entry.col = num_cols;

    if (!num_heads) {
        node->right = node;
    } else {
        for (i = 0; i < num_heads; i++) {
            temp = new_node();
            hdnode[i] = temp;
            hdnode[i]->tag = head;
            hdnode[i]->right = temp;
            hdnode[i]->u.next = temp;
        }

        current_row = 0;
        last = hdnode[0];

        for (i = 0; i < num_terms; i++) {
            fscanf(input, "%d %d %d", &row, &col, &value);
            if (row > current_row) {
                last->right = hdnode[current_row];
                current_row = row;
                last = hdnode[row];
            }
            temp = new_node();
            temp->tag = entry;
            temp->u.entry.row = row;
            temp->u.entry.col = col;
            temp->u.entry.value = value;
            last->right = temp;
            last = temp;

            hdnode[col]->u.next->down = temp;
            hdnode[col]->u.next = temp;
        }

        last->right = hdnode[current_row];
        for (i = 0; i < num_cols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        for (i = 0; i < num_heads - 1; i++)
            hdnode[i]->u.next = hdnode[i + 1];
        hdnode[num_heads - 1]->u.next = node;
        node->right = hdnode[0];
    }

    fclose(input);
    return node;
}

void mwrite(matrix_pointer node) {
    FILE *output = fopen("output.txt", "w");

    int i;
    matrix_pointer temp, head = node->right;

    fprintf(output, "%d %d %d\n", node->u.entry.row, node->u.entry.col, node->u.entry.value);
    
    for (i = 0; i < node->u.entry.row; i++) {
        for (temp = head->right; temp != head; temp = temp->right)
            fprintf(output, "%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
        head = head->u.next;
    }

    fclose(output);
}
////////////////////////////////////////////////////////////
int compare_entries(const void *a, const void *b) {
    matrix_pointer e1 = *(matrix_pointer *)a;
    matrix_pointer e2 = *(matrix_pointer *)b;
    if (e1->u.entry.row == e2->u.entry.row)
        return e1->u.entry.col - e2->u.entry.col;
    return e1->u.entry.row - e2->u.entry.row;
}

matrix_pointer mtranspose(matrix_pointer node) {
    int num_rows = node->u.entry.row;
    int num_cols = node->u.entry.col;
    //number of nodes needed
    int num_heads = (num_cols > num_rows) ? num_cols : num_rows;
    matrix_pointer temp, transpose_node;
    int i, current_col = 0;
    int total_terms = 0;
    matrix_pointer *entries = malloc(num_heads * num_heads * sizeof(matrix_pointer));
    //노드가 0이면 원래꺼 변환
    if (!num_heads){
        return node;
    }
    //새로운 노드 생성
    transpose_node = new_node();
    transpose_node->tag = entry;
    //교환
    transpose_node->u.entry.row = num_cols;
    transpose_node->u.entry.col = num_rows;
    //해더 노드를 초기화
    for (i = 0; i < num_heads; i++) {
        temp = new_node();
        hdnode_t[i] = temp;
        hdnode_t[i]->tag = head;
        hdnode_t[i]->right = temp;
        hdnode_t[i]->u.next = temp;
    }
    matrix_pointer head = node->right;
    for (i = 0; i < num_rows; i++) {
        for (temp = head->right; temp != head; temp = temp->right) {
            //각 항목에서 행, 열, 값 추출
            int row = temp->u.entry.row;
            int col = temp->u.entry.col;
            int value = temp->u.entry.value;

            matrix_pointer new_entry = new_node();
            new_entry->tag = entry;
            new_entry->u.entry.row = col;
            new_entry->u.entry.col = row;
            new_entry->u.entry.value = value;
            //저강하고 총 항목 수를 업데이트
            entries[total_terms++] = new_entry;
        }
        head = head->u.next;
    }
    //배열을 정렬
    qsort(entries, total_terms, sizeof(matrix_pointer), compare_entries);
    current_col = -1; 
    matrix_pointer last = NULL;
    for (i = 0; i < total_terms; i++) {
        matrix_pointer entry = entries[i];
        if (entry->u.entry.row != current_col) {
            //마기막 항모그이 링크를 업데이트
            if (last) {
                last->right = hdnode_t[current_col];
            }
            //마지막 포인터를 업데이트
            current_col = entry->u.entry.row;
            last = hdnode_t[current_col];
        }
        last->right = entry;//연결하고 업데이트
        last = entry;
        //아래 링크와 업데이트
        hdnode_t[entry->u.entry.col]->u.next->down = entry;
        hdnode_t[entry->u.entry.col]->u.next = entry;
    }
    if (last) {
        last->right = hdnode_t[current_col];
    }

    for (i = 0; i < num_rows; i++)
        hdnode_t[i]->u.next->down = hdnode_t[i];
    for (i = 0; i < num_heads - 1; i++)
        hdnode_t[i]->u.next = hdnode_t[i + 1];
    hdnode_t[num_heads - 1]->u.next = transpose_node;
    transpose_node->right = hdnode_t[0];

    transpose_node->u.entry.row = num_cols;
    transpose_node->u.entry.col = num_rows;
    transpose_node->u.entry.value = total_terms;

    free(entries);

    return transpose_node;
}
/////////////////////////////////////////////////
int main() {
    matrix_pointer node = mread();
    matrix_pointer transposed_node = mtranspose(node);
    mwrite(transposed_node);
    return 0;
}
