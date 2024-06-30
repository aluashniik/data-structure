#include <stdio.h>
#include <stdlib.h>

typedef struct poly_node *poly_pointer;
typedef struct poly_node {
    int coef;
    int expon;
    poly_pointer link;
} poly_node;

poly_pointer get_node(void);
void attach(int coef, int expon, poly_pointer *ptr);
void erase(poly_pointer *ptr);
poly_pointer padd(poly_pointer a, poly_pointer b);
int compare(int a, int b);
poly_pointer pread(const char *filename);
void pwrite(poly_pointer p, const char *filename);
poly_pointer pmult(poly_pointer a, poly_pointer b);

poly_pointer get_node(void) {
    poly_pointer node = (poly_pointer)malloc(sizeof(poly_node));
    if (!node) {
        fprintf(stderr, "The memory is full\n");
        exit(1);
    }
    node->link = NULL;
    return node;
}

void attach(int coef, int expon, poly_pointer *ptr) {
    poly_pointer temp = get_node();
    temp->coef = coef;
    temp->expon = expon;
    temp->link = NULL;
    (*ptr)->link = temp;
    *ptr = temp;
}

void erase(poly_pointer *ptr) {
    poly_pointer temp;
    while (*ptr) {
        temp = *ptr;
        *ptr = (*ptr)->link;
        free(temp);
    }
}

int compare(int a, int b) {
    if (a > b) return 1;
    else if (a == b) return 0;
    else return -1;
}

poly_pointer pread(const char *filename) {
    FILE *file = fopen(filename, "r");
    int num_terms, coef, expon;
    fscanf(file, "%d", &num_terms);

    poly_pointer head = get_node();
    poly_pointer rear = head;

    for (int i = 0; i < num_terms; i++) {
        fscanf(file, "%d %d", &coef, &expon);
        attach(coef, expon, &rear);
    }

    poly_pointer temp = head;
    head = head->link;
    free(temp);
    fclose(file);
    return head;
}
////////////////////////////////////////////////////////////////////
void pwrite(poly_pointer p, const char *filename) {
    FILE *file = fopen(filename, "w");
    int num_terms = 0;
    poly_pointer temp = p;
    //the number of terms
    while (temp) {
        num_terms++;
        temp = temp->link;
    }
    fprintf(file, "%d\n", num_terms);
    //writing each terms to the file
    while (p) {
        fprintf(file, "%d %d\n", p->coef, p->expon);
        p = p->link;
    }

    fclose(file);
}
/////////////////////////////////////////////////////////////////////////////
poly_pointer padd(poly_pointer a, poly_pointer b) {
    poly_pointer c, rear, temp;
    int sum;
    rear = get_node();
    c = rear;
    while (a && b) {
        switch (compare(a->expon, b->expon)) {
            case -1: /* a->expon < b->expon */
                attach(b->coef, b->expon, &rear);
                b = b->link; break;
            case 0: /* a->expon == b->expon */
                sum = a->coef + b->coef;
                if (sum) attach(sum, a->expon, &rear);
                a = a->link;
                b = b->link; break;
            case 1: /* a->expon > b->expon */
                attach(a->coef, a->expon, &rear);
                a = a->link;
        }
    }
    for (; a; a = a->link) attach(a->coef, a->expon, &rear);
    for (; b; b = b->link) attach(b->coef, b->expon, &rear);
    rear->link = NULL;
    temp = c;
    c = c->link;
    free(temp);
    return c;
}
///////////////////////////////////////////////////////////////////////////////
poly_pointer pmult(poly_pointer a, poly_pointer b) {
    if (!a || !b) {
        return NULL;
    }
    poly_pointer c = NULL;
    poly_pointer temp_a, temp_b;
    poly_pointer last_c;
    //through each term of a
    for (temp_a = a; temp_a; temp_a = temp_a->link) {
        poly_pointer product = NULL, last_product;
        //through each term of b
        for (temp_b = b; temp_b; temp_b = temp_b->link) {
            poly_pointer temp_node = get_node();
            //multiply coef and add expon
            temp_node->coef = temp_a->coef * temp_b->coef;
            temp_node->expon = temp_a->expon + temp_b->expon;
            temp_node->link = NULL;
            if (!product) {
                product = temp_node;
                last_product = product;
            } else {
                last_product->link = temp_node;
                last_product = temp_node;
            }
        }
        if (!c) {
            c = product;
        } else { //add result to c
            c = padd(c, product);
            erase(&product);
        }
    }
    return c;
}
//////////////////////////////////////////////////////////////////
int main() {
    poly_pointer a = pread("a.txt");
    poly_pointer b = pread("b.txt");
    poly_pointer d = pmult(a, b);
    pwrite(d, "d.txt");

    erase(&a);
    erase(&b);
    erase(&d);

    return 0;
}
