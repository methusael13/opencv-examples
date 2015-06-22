#include "stack.h"
#include <stdio.h>

#define SZ 10

int main(int argc, char *argv[]) {
    int i;
    Stack *stack = NULL;
    
    for (i = 0; i < SZ; i++) {
        stack_push(&stack, i + 1);
        printf("Pushed: %d\n", stack_peek(stack));
    }

    printf("\n");
    while (stack->next != NULL)
        printf("Popped: %d\n", stack_pop(&stack));
    printf("Last: %d\n", stack->val);

    destroy_stack(stack);
    return 0;
}
