#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    int key;
    struct TreeNode *left, *right;
} TreeNode;

TreeNode* createNode(int key) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

TreeNode* insert(TreeNode* root, int key, int* success) {
    if (root == NULL) {
        *success = 1;
        return createNode(key);
    }
    if (key<root->key) {
        root->left = insert(root->left, key, success);
    } else if (key>root->key) {
        root->right = insert(root->right, key, success);
    } else {
        *success = 0; 
    }
    return root;
}

TreeNode* find(TreeNode* root) {
    while (root && root->right != NULL) {
        root = root->right;
    }
    return root;
}

TreeNode* delete(TreeNode* root, int* max) {
    if (root == NULL) {
        return NULL;
    }
    if (root->right == NULL) {
        *max = root->key;
        TreeNode* leftChild = root->left;
        free(root);
        return leftChild;
    }
    root->right = delete(root->right, max);
    return root;
}

void freeTree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    TreeNode* root = NULL;
    char buffer[100];
    char command[10];
    int key;
    FILE *input = fopen("input3.txt", "r");
    FILE *output = freopen("output3.txt", "w", stdout);
    if (input == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), input)) {
        if (sscanf(buffer, "%s %d", command, &key) == 2) {
            if (strcmp(command, "push") == 0) {
                int success;
                root = insert(root, key, &success);
                if (success) {
                    printf("Push %d\n", key);
                } else {
                    printf("Exist number\n");
                }
            }
        } 
        else if (sscanf(buffer, "%s", command) == 1) {
            if (strcmp(command, "top") == 0) {
                TreeNode* maxNode = find(root);
                if (maxNode) {
                    printf("The top is %d\n", maxNode->key);
                } else {
                    printf("The queue is empty\n");
                }
            } 
            else if (strcmp(command, "pop") == 0) {
                if (root) {
                    int max;
                    root = delete(root, &max);
                    printf("Pop %d\n", max);
                } else {
                    printf("The queue is empty\n");
                }
            } 
            else if (strcmp(command, "q") == 0) {
                fclose(input);
                fclose(output);
                freeTree(root);
                return 0;
            }
        }
    }

    fclose(input);
    fclose(output);
    freeTree(root);
    return 0;
}

