#ifndef _STACK_
#define _STACK_

typedef struct stack {
    int val;
    struct stack *next;
} Stack;

Stack * create_stack(void);
void destroy_stack(Stack *stack);

int stack_peek(Stack *stack);
int stack_pop(Stack **stack);
void stack_push(Stack **stack, int val);

#endif
