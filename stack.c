#include <stdio.h>
#include <stdlib.h>

typedef enum{
    STACK_OK = 0,
    STACK_OVERFLOW = 1 << 0,
    STACK_NULL = 2 << 1,    
    // TODO: add more error types
 } error_types;

 typedef struct{
    int *data;
    int *top;
    size_t size;
    size_t capacity;
 } Stack;

Stack *stack_build(void);
void *stack_destroy(Stack *stack);
int stack_pop(Stack *stack);
void stack_push(Stack *stack, int value);
void auto_resize(Stack *stack);

int main(void){
    Stack *stack = stack_build();

    // TODO: main:)


    stack_destroy(stack);
    return 0;   
}

Stack *stack_build(void){
    Stack *stack = calloc(1, sizeof(Stack));
    stack->data = calloc(1, sizeof(int));
    stack->top = stack->data;
    stack->size = 0;
    stack->capacity = 1;
}

void *stack_destroy(Stack *stack){
    if (!stack){
        return;
    } 

    free(stack->data);
    free(stack->top);
    free(stack);
}

int stack_pop(Stack *stack){
    if (!(stack)){
        /* error: stack pointer is NULL" */;
        return 1;
    }

    if (stack->size == 0){
        /* error: pop from empry_stack */
        return 1;
    }

    int value = *stack->top;
    stack->size --;

    if (stack->size == 0){
        stack->top = stack->data - 1;                 // stack is empty
    } else{
        stack->top = stack->data + (stack->size - 1);
    }
}

void stack_push(Stack *stack, int value){
    if (!stack) {
        /* error: stack pointer is NULL */
        return;
    }

    auto_resize(stack);                               // if we need to increase capacity  

    stack->size++;

    if (stack->size == 1)
        stack->top = stack->data;                
    else
        stack->top = stack->data + stack->size - 1; 

    *stack->top = value;
}

void auto_resize(Stack *stack){
    if (!stack)
        return;

    if (stack->size < stack->capacity)
        return;

    size_t new_capacity;

    if (stack->capacity == 0)
        new_capacity = 1;
    else
        new_capacity = stack->capacity * 2;


    int *new_data = realloc(stack->data, new_capacity * sizeof(int));

    if (new_data == NULL) {
        /* error: realloc failed */ 
        return;
    }


    stack->data = new_data;
    stack->capacity = new_capacity;
}

// TODO: void dump
void stack_dump(Stack *stack){
    if (stack == 0){
        /* error: stack pointer is NULL */
        return;
    }

}
