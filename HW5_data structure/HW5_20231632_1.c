#include <stdio.h>
#include <stdlib.h>

typedef struct node *treePointer;
typedef struct node {
    int key;
    treePointer parent;
    treePointer leftChild, rightChild;
} node;

treePointer queue[1000]; 

treePointer root = NULL;

treePointer create(int key) {
    treePointer newNode = (treePointer)malloc(sizeof(node));
    newNode->key = key;
    newNode->parent = NULL;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    return newNode;
}

treePointer find(treePointer node) {
    if (!node) {
        return NULL;
    }

    treePointer last = NULL;
    //treePointer queue[1000];  
    int front = 0;
    int rear = 0;

    queue[rear++] = node;

    while (front < rear) {
        treePointer current = queue[front++];
        last = current;

        if (current->leftChild){
            queue[rear++] = current->leftChild;
        }
        if (current->rightChild) {
            queue[rear++] = current->rightChild;
        }
    }
    return last;
}

void swap(treePointer m, treePointer n) {
    int temp = m->key;
    m->key = n->key;
    n->key = temp;
}

void up(treePointer node) {
    while ((node->parent) && (node->key)>(node->parent->key)) {
        swap(node, node->parent);
        node = node->parent;
    }
}

void down(treePointer node) {
    while (node) {
        treePointer maxNode = node;

        if ((node->leftChild) && (node->leftChild->key > maxNode->key)) {
            maxNode = node->leftChild;
        }
        if ((node->rightChild) && (node->rightChild->key > maxNode->key)) {
            maxNode = node->rightChild;
        }
        if (maxNode == node) {
            break;
        }
        swap(node, maxNode);
        node = maxNode;
    }
}

int exists(int key) {
    //treePointer queue[1000];
    int front = 0;
    int rear = 0;
    if (root) {
        queue[rear++] = root;
    }
    while (front<rear) {
        treePointer current = queue[front++];
        if (current->key == key) {
            return 1;
        }
        if (current->leftChild) {
            queue[rear++] = current->leftChild;
        }
        if (current->rightChild) {
            queue[rear++] = current->rightChild;
        }
    }
    return 0;
}

void insert(int key) {
    if (exists(key)) {
        printf("Exist number\n");
        return;
    }
    treePointer newNode = create(key);
    if (!root) {
        root = newNode;
    } else {
        treePointer last = find(root);
        if (!last->leftChild) {
            last->leftChild = newNode;
            newNode->parent = last;
        } else {
            last->rightChild = newNode;
            newNode->parent = last;
        }
        up(newNode);
    }
    printf("Insert %d\n", key);
}

void delete() {
    if (!root) {
        printf("The heap is empty\n");
        return;
    }

    int max = root->key;

    treePointer last = find(root);
    if (last == root) {
        free(root);
        root = NULL;
    } else {
        root->key = last->key;
        if (last->parent->leftChild == last) {
            last->parent->leftChild = NULL;
        } else {
            last->parent->rightChild = NULL;
        }
        free(last);
        down(root);
    }
    printf("Delete %d\n", max);
}

int main() {
    char command;
    int key;
    FILE *input = fopen("input1.txt", "r");
    FILE *output = freopen("output1.txt", "w", stdout);
    while (fscanf(input, " %c", &command) != EOF) {
        switch (command) {
            case 'i':
                fscanf(input, "%d", &key);
                insert(key);
                break;
            case 'd':
                delete();
                break;
            case 'q':
                fclose(input);
                fclose(output);
                return 0;
            default:
                break;
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}

