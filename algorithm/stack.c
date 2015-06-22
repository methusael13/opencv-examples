#include "stack.h"
#include <stdlib.h>

Stack * create_stack() {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    if (stack) {
        stack->val = 0;
        stack->next = NULL;

        return stack;
    }

    return NULL;
}

void destroy_stack(Stack *stack) {
    if (!stack) return;

    while (stack->next != NULL) {
        Stack *s = stack;
        stack = stack->next;
        free(s);
    }

    free(stack);
}

int stack_peek(Stack *stack) {
    if (stack) return stack->val;
    return -1;
}

int stack_pop(Stack **stack) {
    if (*stack == NULL) return -1;

    Stack *s = *stack;
    int val = s->val;

    *stack = (*stack)->next;
    free(s);

    return val;
}

void stack_push(Stack **stack, int val) {
    Stack *s = create_stack();
    s->val = val;
    s->next = *stack;

    *stack = s;
}
