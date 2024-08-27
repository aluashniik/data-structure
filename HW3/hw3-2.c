#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 20

char stack[MAX_SIZE];
int top = -1;

int isEmpty() {
    return top == -1;
}

int isFull() {
    return top == MAX_SIZE - 1;
}

void push(char value) {
    if (isFull()) {
        printf("Stack is full\n");
        return;
    }
    stack[++top] = value;
}

char pop() {
    if (isEmpty()) {
        printf("Stack is empty\n");
        return '\0';
    }
    return stack[top--];
}

char peek() {
    if (isEmpty()) {
        printf("Stack is empty\n");
        return '\0';
    }
    return stack[top];
}

int precedence(char op) {
    if (op == '*' || op == '/'){
        return 2;
    }
    else if (op == '+' || op == '-'){
        return 1;
    }
    else{
        return 0;
    }
}

void conv(char *infix, char *prefix) {
    int length = strlen(infix);
    char temp[length + 1];

    for (int i = 0; i < length; i++) {
        temp[i] = infix[length - 1 - i];
    }
    temp[length] = '\0';

    int j = 0;

    for (int i = 0; i < length; i++) {
        char ch = temp[i];
        if (ch >= '0' && ch <= '9') {
            prefix[j++] = ch;
        } 
        else if (ch == ')') {
            push(ch);
        }
        else if (ch == '(') {
            while (!isEmpty() && peek() != ')') {
                prefix[j++] = pop();
            }
            pop();
        } 
        else {
            while (!isEmpty() && precedence(peek()) >= precedence(ch)) {
                prefix[j++] = pop();
            }
            push(ch);
        }
    }

    while (!isEmpty()) {
        prefix[j++] = pop();
    }
    
    prefix[j] = '\0';

    length = strlen(prefix);
    for (int i = 0; i < length / 2; i++) {
        char temp = prefix[i];
        prefix[i] = prefix[length - 1 - i];
        prefix[length - 1 - i] = temp;
    }
}

int main() {
    char infix[MAX_SIZE], prefix[MAX_SIZE];
    fgets(infix, MAX_SIZE, stdin);
    infix[strcspn(infix, "\n")] = 0;
    conv(infix, prefix);
    printf("%s\n", prefix);

    return 0;
}
