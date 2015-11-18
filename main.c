

#include "common.h"
#include "SP_Stack.h"
#include "SP_Aux.h"

#define VERIFY_STACK_MSG_OK(msg)        \
    if (SP_STACK_SUCCESS != (msg)) {    \
        goto cleanup;                   \
    }
    
#define VERIFY_AUX_MSG_OK(msg)        \
    if (SP_AUX_SUCCESS != (msg)) {    \
        goto cleanup;                   \
    }
    
typedef enum {
    NUM,
    OP,
} READ_TYPE;

char* getErrorMessage(SP_STACK_MSG stack_msg, SP_AUX_MSG aux_msg);

/**
 * Receives a line of input and returns the output for that line
 *
 * @param
 * 		char* line - The line of input to parse and analyze
 * @return
 *		A pointer to the message that needs to be printed to the screen.
 *      This message needs to be freed using the freeLineFromUser function.
 */
char* calculateLineResult(char* line) {
    char* return_value = NULL;
    SP_STACK_MSG stack_msg = SP_STACK_SUCCESS;
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;
    SP_STACK* numbers_stack = NULL;
    SP_STACK* operations_stack = NULL;
    char* current_position = line;
    bool should_continue = true;
    unsigned long number_read = 0;
    SP_STACK_ELEMENT_TYPE operation_read = UNKNOWN;
    bool operation_stack_is_empty = true;
    
    numbers_stack = spStackCreate(&stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
    
    operations_stack = spStackCreate(&stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
    
    current_position = advanceToNextToken(&aux_msg, current_position);
    VERIFY_AUX_MSG_OK(aux_msg);
    
    number_read = parseNumber(&aux_msg, current_position, &current_position);
    VERIFY_AUX_MSG_OK(aux_msg);
    
    pushNumber(&stack_msg, numbers_stack, number_read);
    VERIFY_STACK_MSG_OK(stack_msg);
    
    while(should_continue) {
        current_position = advanceToNextToken(&aux_msg, current_position);
        VERIFY_AUX_MSG_OK(aux_msg);
        
        operation_read = parseOperation(&aux_msg, current_position, &current_position);
        VERIFY_AUX_MSG_OK(aux_msg);
        
        operation_stack_is_empty = spStackIsEmpty(operations_stack, &stack_msg);
        VERIFY_STACK_MSG_OK(stack_msg);
        
        if (operation_stack_is_empty) {
            pushOperation(&stack_msg, operations_stack, operation_read);
            VERIFY_STACK_MSG_OK(stack_msg);
        }
    }
    
cleanup:
    if (NULL == return_value) {
        return_value = getErrorMessage(stack_msg, aux_msg);
    }

    spStackDestroy(operations_stack);
    spStackDestroy(numbers_stack);
    return return_value;
}

int main() {
    int return_value = -1;
    bool should_continue = true;
    
    while (should_continue) {
        SP_AUX_MSG msg = SP_AUX_SUCCESS;
        char* line = NULL;
        char* result = NULL;
        
        line = getLineFromUser(&msg);
        VERIFY_AUX_MSG_OK(msg);
        
        should_continue = isEndMessage(&msg, line);
        VERIFY_AUX_MSG_OK(msg);
        VERIFY_CONDITION(should_continue);
        
        result = calculateLineResult(line);
        printf("%s", result);
        
    cleanup:
        freeLineFromUser(line);
        freeLineFromUser(result);
    }
    
    return return_value;
}