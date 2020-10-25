#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

typedef struct {
    int arr[1000];
    int top;
}stack;

bool empty(stack* st) {
    return st->top < 0;
}

void push(stack* st, int elem){
    st->top++;
    st->arr[st->top] = elem;
}

int pop(stack* st){
    if(empty(st)){
        printf("syntax error");
        exit(0);
    }
    return st->arr[st->top--];
}

int peek(stack* st){
    if (empty(st)) {
        printf("syntax error");
        exit(0);
    }
    return st->arr[st->top];
}

int priority (char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

void calculation(stack *nums, char operation){
    int b = pop(nums);
    int a = pop(nums);
    switch (operation){
        case '+':
            push(nums, a + b);
            break;
        case '-':
            push(nums, a - b);
            break;
        case '*':
            push(nums, a * b);
            break;
        case '/':
            if (!b) {
                printf("division by zero");
                exit(0);
            }
            push(nums, a / b);
            break;
    }
}

bool isOperation (char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}
bool isDigit (char c) {
    return c >= '0' && c <= '9';
}
bool okSym (char c) {
    return isOperation(c) || isDigit(c);
}

int result(char *inputStr, int strSize){
    stack nums;
    nums.top = -1;
    stack operations;
    operations.top = -1;
    for(int i = 0; i < strSize; i++){
        if (inputStr[i] == '(') {
            push(&operations, (int)inputStr[i]);
        }
        else if (inputStr[i] == ')'){
            if (i == 0 || inputStr[i - 1] == '('){
                printf("syntax error");
                exit(0);
            }
            char prev;
            while((prev = (char)pop(&operations)) != '(')
                calculation(&nums, prev);
        }
        else if (isOperation(inputStr[i])){
            while(!empty(&operations) && priority((char)peek(&operations)) >= priority(inputStr[i]))
                calculation(&nums, (char)pop(&operations));
            push(&operations, (int)inputStr[i]);
        }
        else{
            int num = inputStr[i] - '0';
            while(i < strSize && isDigit(inputStr[++i]))
                num = num * 10 + (inputStr[i] - '0');
            i--;
            push(&nums, num);
        }
    }
    while (!empty(&operations))
        calculation(&nums, (char)pop(&operations));
    return pop(&nums);
}

int main(){
    char inputStr[1005];
    int strSize = 0;
    for(int i = 0; i < 1000; ++i){
        char a;
        if(!scanf("%c", &a))
            return 0;
        if (a == '\n')
            break;
        if(!okSym(a)){
            printf("syntax error");
            return 0;
        }
        inputStr[strSize++] = a;
    }
    printf("%d", result(inputStr, strSize));
}
