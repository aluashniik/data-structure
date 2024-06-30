#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct TreeNode {
    int key;
    struct TreeNode *left, *right;
} TreeNode;

TreeNode* create(int key) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

TreeNode* construct(int preorder[], int* indx, int key, int min, int max, int size) {
    if (*indx >= size) {
        return NULL;
    }

    TreeNode* root = NULL;

    if (key>min && key<max) {
        root = create(key);
        *indx += 1;

        if (*indx<size) {
            root->left = construct(preorder, indx, preorder[*indx], min, key, size);
        }

        if (*indx<size) {
            root->right = construct(preorder, indx, preorder[*indx], key, max, size);
        }
    }
    return root;
}

void inorder(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

void postorder(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->key);
}

void freeTree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int ifhas(int arr[], int size) {
    for (int i=0; i<size-1; i++) {
        for (int j=i+1; j<size; j++) {
            if (arr[i] == arr[j]) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    FILE *input = fopen("input2.txt", "r");
    FILE *output = freopen("output2.txt", "w", stdout);
    if (input == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }
    int n;
    fscanf(input, "%d", &n);
    int preorder[n];
    for (int i = 0; i < n; i++) {
        fscanf(input, "%d", &preorder[i]);
    }
    fclose(input);
    if (ifhas(preorder, n)) {
        printf("cannot construct BST\n");
        fclose(output);
        return 0;
    }
    int indx = 0;
    TreeNode* root = construct(preorder, &indx, preorder[0], INT_MIN, INT_MAX, n);

    if (root == NULL) {
        printf("cannot construct BST\n");
        fclose(output);
        return 0;
    }
    printf("Inorder: ");
    inorder(root);
    printf("\n");

    printf("Postorder: ");
    postorder(root);
    printf("\n");

    freeTree(root);
    fclose(output);
    return 0;
}
