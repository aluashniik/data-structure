#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 128

typedef struct {
    int top;
    unsigned term;
    char* array;
} Stack;

Stack* createStack(unsigned term) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->term = term;
    stack->top = -1;
    stack->array = (char*) malloc(stack->term * sizeof(char));
    return stack;
}

int isFull(Stack* stack) {
    return stack->top == stack->term - 1;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, char item) {
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

char pop(Stack* stack) {
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top--];
}

char peek(Stack* stack) {
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top];
}

char* removek(char* num, int k) {
    int n = strlen(num);
    if (k >= n) return "0";

    Stack* stack = createStack(n);
    
    for (int i = 0; i < n; i++) {
        while (!isEmpty(stack) && k > 0 && num[i] < peek(stack)) {
            pop(stack);
            k--;
        }
        push(stack, num[i]);
    }
    while (k > 0 && !isEmpty(stack)) {
        pop(stack);
        k--;
    }
    char* result = (char*) malloc((stack->top + 2) * sizeof(char));
    int len = 0;
    while (!isEmpty(stack)) {
        result[len++] = pop(stack);
    }
    result[len] = '\0';
    for (int i = 0; i < len / 2; i++) {
        char temp = result[i];
        result[i] = result[len - 1 - i];
        result[len - 1 - i] = temp;
    }
    int start = 0;
    while (start < len && result[start] == '0') {
        start++;
    }
    
    if (start == len) { 
        free(result);
        free(stack->array);
        free(stack);
        return "0";
    }

    char* finalResult = (char*) malloc((len - start + 1) * sizeof(char));
    for (int i = start; i < len; i++) {
        finalResult[i - start] = result[i];
    }
    finalResult[len - start] = '\0';

    free(result);
    free(stack->array);
    free(stack);

    return finalResult;
}

int main() {
    char num[MAX_LENGTH + 1];
    int k;
    scanf("%s", num);
    scanf("%d", &k);
    char* result = removek(num, k);
    printf("%s\n", result);
    free(result);
    return 0;
}
