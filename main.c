/*
 * Main Module
 */

#include <assert.h>
#include "common.h"
#include "SP_Stack.h"
#include "SP_Aux.h"

/*
 * Utility Macros
 */

#define VERIFY_STACK_MSG_OK(msg)        \
    if (SP_STACK_SUCCESS != (msg)) {    \
        goto cleanup;                   \
    }
    
#define VERIFY_AUX_MSG_OK(msg)        \
    if (SP_AUX_SUCCESS != (msg)) {    \
        goto cleanup;                   \
    }

/*
 * Constants
 */

typedef enum {
    NUM,
    OP,
} READ_TYPE;

/*
 * Function Declarations
 */

char* getErrorMessage(SP_STACK_MSG stack_msg, SP_AUX_MSG aux_msg);
bool interact(SP_AUX_MSG* msg);
char* calculateLineResult(char* line);

/*
 * Functions
 */

int main() {
    int return_value = EXIT_FAILURE;
    bool should_exit = false;

    while (!should_exit) {
        SP_AUX_MSG msg = SP_AUX_SUCCESS;
        should_exit = interact(&msg);
        VERIFY_AUX_MSG_OK(msg);
    }

    return_value = EXIT_SUCCESS;

cleanup:
    /* TODO: return msg instead? */
    return return_value;
}

/* TODO: document */
bool interact(SP_AUX_MSG* msg)
{
    bool should_exit = false;
    char* calculatedLine = NULL;

    assert(msg != NULL);

    char line[MAX_LINE_INPUT_LENGTH + 1];
    getLineFromUser(line, sizeof(line), msg);
    VERIFY_AUX_MSG_OK(*msg);

    should_exit = isEndMessage(msg, line);
    VERIFY_AUX_MSG_OK(*msg);

    char* result;
    if (should_exit) {
        result = "Exiting...\n";
    } else {
        calculatedLine = calculateLineResult(line);
        result = calculatedLine;
    }

    int printf_retval = printf(result);
    VERIFY_CONDITION_AND_SET_ERROR(printf_retval >= 0, msg, SP_AUX_PRINT_ERROR);
    VERIFY_CONDITION_AND_SET_ERROR(ferror(stdout) == 0, msg, SP_AUX_PRINT_ERROR);

    SET_MESSAGE(msg, SP_AUX_SUCCESS);

cleanup:
    free(calculatedLine);
    return should_exit;
}

/**
 * Receives a line of input and returns the output for that line
 *
 * @param
 * 		char* line - The line of input to parse and analyze
 * @return
 *		A pointer to the message that needs to be printed to the screen.
 *      This message needs to be freed using free().
 */
char* calculateLineResult(char* line)
{
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
