#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 128

int operand_stack[MAX_LENGTH]; 
int operand_top = -1;  
char operator_stack[MAX_LENGTH];
int operator_top = -1;  
   
void pushOperand(int item) {
    if (operand_top >= MAX_LENGTH - 1) {
        fprintf(stderr, "overflow\n");
        exit(EXIT_FAILURE);
    }
    operand_stack[++operand_top] = item;
}

int popOperand(void) {
    if (operand_top == -1) {
        fprintf(stderr, "underflow\n");
        exit(EXIT_FAILURE);
    }
    return operand_stack[operand_top--];
}         

void pushOperator(char item) {
    if (operator_top >= MAX_LENGTH - 1) {
        fprintf(stderr, "overflow\n");
        exit(EXIT_FAILURE);
    }
    operator_stack[++operator_top] = item;
}

char popOperator(void) {
    if (operator_top == -1) {
        fprintf(stderr, "underflow\n");
        exit(EXIT_FAILURE);
    }
    return operator_stack[operator_top--];
}

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '#') return 3; 
    return 0;
}

void eval(char *infix, char *postfix) {
    int i, j = 0;
    char ch;

    for (i = 0; infix[i] != '\0'; i++) {
        ch = infix[i];

        if (isspace(ch)) {
            continue;
        } else if (isdigit(ch)) {
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            i--;
        } else if (ch == '(') {
            pushOperator(ch);
        } else if (ch == ')') {
            while (operator_top != -1 && operator_stack[operator_top] != '(') {
                postfix[j++] = popOperator();
                postfix[j++] = ' ';
            }
            popOperator(); 
        } else if (isOperator(ch)) {
            if (ch == '-' && (i == 0 || infix[i-1] == '(' || isOperator(infix[i-1]))) {
                pushOperator('#'); 
            } else {
                while (operator_top != -1 && precedence(operator_stack[operator_top]) >= precedence(ch)) {
                    postfix[j++] = popOperator();
                    postfix[j++] = ' ';
                }
                pushOperator(ch);
            }
        }
    }

    while (operator_top != -1) {
        postfix[j++] = popOperator();
        postfix[j++] = ' ';
    }
    postfix[j - 1] = '\0';
}

int calc(char *postfix) {
    char *token = strtok(postfix, " ");
    int op1, op2;

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushOperand(atoi(token));
        } else {
            switch (token[0]) {
                case '+':
                    op2 = popOperand();
                    op1 = popOperand();
                    pushOperand(op1 + op2);
                    break;
                case '-':
                    op2 = popOperand();
                    op1 = popOperand();
                    pushOperand(op1 - op2);
                    break;
                case '*':
                    op2 = popOperand();
                    op1 = popOperand();
                    pushOperand(op1 * op2);
                    break;
                case '/':
                    op2 = popOperand();
                    op1 = popOperand();
                    pushOperand(op1 / op2);
                    break;
                case '%':
                    op2 = popOperand();
                    op1 = popOperand();
                    pushOperand(op1 % op2);
                    break;
                case '#':
                    op1 = popOperand();
                    pushOperand(-op1);
                    break;
            }
        }
        token = strtok(NULL, " ");
    }
    return popOperand();
}

int main() {
    char infix[MAX_LENGTH + 1];
    char postfix[MAX_LENGTH * 2 + 1];
    int result;
    fgets(infix, MAX_LENGTH, stdin);
    infix[strcspn(infix, "\n")] = '\0'; 
    eval(infix, postfix);
    printf("%s\n", postfix);
    result = calc(postfix);
    printf("%d\n", result);
    return 0;
}

