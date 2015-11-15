

#include "common.h"

#include "SP_Stack.h"

/**
 * The notes said that at no time there were going to be more thatn 1024 elements in the
 * stack, so we have the stack preallocate all the needed space. If we want to support 
 * a dynamic amount of elements in the stack, it would be easier to modify SP_STACK_ELEMENT
 * to have a "previous" variable and implement a linked-list using them.
 */
#define MAX_NUMBER_IN_STACK (1024)
/**
 * This is a declaration of struct which represent a stack.
 * You will need to define the stack in SP_Stack.c
 * You can do so by copying the following code to SP_Stack.c:
 */
struct sp_stack_struct {
	SP_STACK_ELEMENT 	elements[MAX_NUMBER_IN_STACK];
	unsigned int 		current_top;
};

#define CLEAR_MSG(msg) *(msg) = SP_STACK_SUCCESS;

SP_STACK* spStackCreate(SP_STACK_MSG* msg) {
    SP_STACK* created_stack = malloc(sizeof(*created_stack));
    VERIFY_CONDITION_AND_SET_ERROR(NULL != created_stack, msg, SP_STACK_ERROR_ALLOCATION_FAILED);
    
    created_stack->current_top = 0;
    CLEAR_MSG(msg);
    
cleanup:
    return created_stack;
}

void spStackDestroy(SP_STACK* stack) {
    VERIFY_CONDITION(NULL != stack);
    free(stack);
cleanup:
    return;
}

SP_STACK_ELEMENT* spStackTop (SP_STACK* stack, SP_STACK_MSG* msg) {
    SP_STACK_ELEMENT* return_value = NULL;
    VERIFY_CONDITION_AND_SET_ERROR(NULL != stack, msg, SP_STACK_ERROR_NULL_ARGUMENT);
    VERIFY_CONDITION_AND_SET_ERROR(stack->current_top != 0, msg, SP_STACK_ERROR_IS_EMPTY);
    
    return_value = &stack->elements[stack->current_top-1];
    CLEAR_MSG(msg);

cleanup:
    return return_value;
}

SP_STACK* spStackPop(SP_STACK* stack, SP_STACK_MSG* msg) {
    VERIFY_CONDITION_AND_SET_ERROR(NULL != stack, msg, SP_STACK_ERROR_NULL_ARGUMENT);
    VERIFY_CONDITION_AND_SET_ERROR(stack->current_top != 0, msg, SP_STACK_ERROR_IS_EMPTY);
    
    stack->current_top--;
    CLEAR_MSG(msg);
    
cleanup:
    return stack;
}

SP_STACK* spStackPush(SP_STACK* stack, SP_STACK_ELEMENT newElement,SP_STACK_MSG* msg) {
    VERIFY_CONDITION_AND_SET_ERROR(NULL != stack, msg, SP_STACK_ERROR_NULL_ARGUMENT);
    VERIFY_CONDITION_AND_SET_ERROR(stack->current_top != MAX_NUMBER_IN_STACK, msg, SP_STACK_ERROR_ALLOCATION_FAILED);
    
    stack->elements[stack->current_top] = newElement;
    stack->current_top++;
    CLEAR_MSG(msg);
    
cleanup:
    return stack;
}

bool spStackIsEmpty(SP_STACK* stack, SP_STACK_MSG* msg) {
    bool return_value = false;
    VERIFY_CONDITION_AND_SET_ERROR(NULL != stack, msg, SP_STACK_ERROR_NULL_ARGUMENT);

    CLEAR_MSG(msg);
    return_value = (stack->current_top == 0);
    
cleanup:
    return return_value;
}