#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static const uint64_t CANARY = 0xDEADBEEF;
typedef enum{
    STACK_OK = 0,
    STACK_OVERFLOW = 1 << 0,
    STACK_NULL = 2 << 1,
    // TODO: add more error types
 } error_types;

typedef struct{
    uint64_t left_canary;

    int *data;
    int *top;
    size_t size;
    size_t capacity;

    uint64_t right_canary;
} Stack;

Stack *stack_build(void);
void stack_destroy(Stack *stack);
int stack_pop(Stack *stack);
int stack_push(Stack *stack, int value);
void auto_resize(Stack *stack);
void stack_dump(Stack *stack);

int main(void){
    Stack *stack = stack_build();

    stack_destroy(stack);
    return 0;
}

Stack *stack_build(void){
    Stack *stack = calloc(1, sizeof(Stack));
    if (stack == NULL) return NULL;

    stack->data = calloc(1, sizeof(int));
    if (stack->data == NULL) {
        free(stack);
        return NULL;
    }

    stack->top = stack->data;
    stack->size = 0;
    stack->capacity = 1;

    return stack;
}

void stack_destroy(Stack *stack){
    if (!stack){
        return;
    }

    free(stack->data);
    free(stack);
}

int stack_pop(Stack *stack){
    if (!(stack)){
        printf("ERROR: STACK POINTER IS NULL\n");
        return 1;
    }

    if (stack->size == 0){
        printf("ERROR: POP FROM EMRPTY STACK\n");
        return 1;
    }

    int value = *stack->top;
    stack->size--;

    if (stack->size == 0){
        stack->top = stack->data - 1;                 // stack is empty
    } else{
        stack->top = stack->data + (stack->size - 1);
    }

    return value;
}

int stack_push(Stack *stack, int value){
    if (!stack) {
        printf("ERROR: STACK POINTER IS NULL\n");
        return 1;
    }

    auto_resize(stack);                               // if we need to increase capacity

    if (stack->size >= stack->capacity) {
        printf("ERROR: REALLOC FAILED / NO SPACE\n");
        return 1;
    }

    stack->size++;

    if (stack->size == 1)
        stack->top = stack->data;
    else
        stack->top = stack->data + stack->size - 1;

    *stack->top = value;

    return value;
}

void auto_resize(Stack *stack){
    if (!stack)
        return;

    if (stack->size < stack->capacity)
        return;

    size_t new_capacity = 0;

    if (stack->capacity == 0)
        new_capacity = 1;
    else
        new_capacity = stack->capacity * 2;

    int *new_data = realloc(stack->data, new_capacity * sizeof(int));

    if (new_data == NULL) {
        printf("ERROR: REALLOC FAILED\n");
        return;
    }

    stack->data = new_data;
    stack->capacity = new_capacity;

    if (stack->size == 0){
        stack->top = stack->data - 1;
    } else{
        stack->top = stack->data + (stack->size - 1);
    }
}

void stack_dump(Stack *stack){
    printf("STACK DUMP\n{\n");

    if (stack == NULL){
        printf("\tERROR: STACK POINTER IS NULL\n}\n");
        return;
    }

    printf("\tdata = %p\n\ttop = %p\n\tsize = %zu\n\tcapacity = %zu\n",
           (void*)stack->data, (void*)stack->top, stack->size, stack->capacity);

    printf("\tData:\n\t{\n");

    if (stack->data == NULL){
        printf("\t\tERROR: DATA IS NULL\n");
    } else {
        for (size_t idx = 0; idx < stack->size; idx++){
            printf("\t\t[%zu] = %d;\n", idx, stack->data[idx]);
        }
    }

    printf("\t}\n}\n");
}

uint64_t *left_data_canary(Stack *stack){
    if (stack == NULL){
        return NULL;
    }

    if (stack->data == NULL){
        return NULL;
    }

    return((uint64_t*)stack->data) - 1;
}

uint64_t *right_data_canary(Stack *stack){
    if (stack == NULL){
        return NULL;
    }

    if (stack->data == NULL){
        return NULL;
    }

    return(uint64_t*)stack->data + stack->capacity;
}
