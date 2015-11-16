

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
    SP_STACK_MSG msg = SP_STACK_SUCCESS;
    SP_STACK* numbers_stack = NULL;
    SP_STACK* operations_stack = NULL;
    
    numbers_stack = spStackCreate(&msg);
    VERIFY_STACK_MSG_OK(msg);
    
    operations_stack = spStackCreate(&msg);
    VERIFY_STACK_MSG_OK(msg);
    
    // TODO - Implement the algorithm they want here
    
cleanup:
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