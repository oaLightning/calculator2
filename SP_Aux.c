/*
 * Auxiliary Functions Module
 */

// TODO - Think if we want to skip tabs as well in advanceToNextToken

#include "SP_Aux.h"
#include "common.h"

#define CLEAR_MSG(msg) SET_MESSAGE(msg, SP_AUX_SUCCESS)

/*
 * Function Implementation
 */

void getLineFromUser(char* buffer, int size, SP_AUX_MSG* msg)
{
    char* fgets_result = NULL;

    fgets_result = fgets(buffer, size, stdin);
    VERIFY_CONDITION_AND_SET_ERROR(0 == ferror(stdin), msg, SP_AUX_INPUT_ERROR);
    VERIFY_CONDITION_AND_SET_ERROR(fgets_result != NULL, msg, SP_AUX_INPUT_ERROR);
    
    CLEAR_MSG(msg);
    
cleanup:
    return;
}

char* copyConstMessageToPool(SP_AUX_MSG* msg, char* message) {
    char* return_value = NULL;
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != message, msg, SP_AUX_NULL_PARAMETER);
    // Note - Here we take into account the null byte at the end of the line
    size_t size_to_alloc = strlen(message) + sizeof(*message);
    
    return_value = malloc(size_to_alloc);
    VERIFY_CONDITION_AND_SET_ERROR(NULL != return_value, msg, SP_AUX_ALLOCATION_ERROR);
    
    // Note - Here we want to make sure that there is enough space for strncpy to copy
    // Note - the null byte as well
    strncpy(return_value, message, size_to_alloc-1);
    CLEAR_MSG(msg);
    
cleanup:
    return return_value;
}

char* advanceToNextToken(SP_AUX_MSG* msg, char* position) {
    char* return_value = NULL;
    const char* DELIMITERS = " \t\r\n";
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != position, msg, SP_AUX_NULL_PARAMETER);
    
    return_value = strtok(position, DELIMITERS);
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != return_value, msg, SP_AUX_LINE_END);
    CLEAR_MSG(msg);

cleanup:
    return return_value;
}

long parseNumber(SP_AUX_MSG* msg, char* position, char** next_position) {
    long return_value = 0;
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != position, msg, SP_AUX_NULL_PARAMETER);
    VERIFY_CONDITION_AND_SET_ERROR(NULL != next_position, msg, SP_AUX_NULL_PARAMETER);
    
    errno = 0;

    /* TODO: make sure that the string doesn't start with +/- */
    return_value = strtol(position, next_position, 10);
    if (0 == return_value) {
        VERIFY_CONDITION_AND_SET_ERROR(*next_position != position, msg, SP_AUX_NOT_NUMBER);
    }
    else if ((LONG_MIN == return_value) || (LONG_MAX == return_value)) {
        VERIFY_CONDITION_AND_SET_ERROR(errno != ERANGE, msg, SP_AUX_INTERNAL_PARSE_NUMBER_ERROR);
    }
    
    CLEAR_MSG(msg);
    
cleanup:
    return return_value;
}

SP_STACK_ELEMENT_TYPE parseOperation(SP_AUX_MSG* msg, char* position, char** next_position) {
    SP_STACK_ELEMENT_TYPE return_value = UNKNOWN;
    bool problem_found = false;
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != position, msg, SP_AUX_NULL_PARAMETER);
    VERIFY_CONDITION_AND_SET_ERROR(NULL != next_position, msg, SP_AUX_NULL_PARAMETER);
    
    switch (*position) {
        case '+': return_value = PLUS; break;
        case '-': return_value = MINUS; break;
        case '*': return_value = MULTIPLICATION; break;
	    case '/': return_value = DIVISION; break;
	    case '$': return_value = DOLLAR; break;
	    default: problem_found = true;
    }
    
    VERIFY_CONDITION_AND_SET_ERROR(!problem_found, msg, SP_AUX_NOT_AN_OPERATION);
    
    *next_position = position + sizeof(*position);
    CLEAR_MSG(msg);
    
cleanup:
    return return_value;
}

bool isEndMessage(SP_AUX_MSG* msg, char* line) {
    bool return_value = false;
    int strcmp_result = 0;
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != line, msg, SP_AUX_NULL_PARAMETER);
    
    strcmp_result = strcmp(line, "<>\n");
    return_value = (0 == strcmp_result);
    CLEAR_MSG(msg);
    
cleanup:
    return return_value;
}

void pushNumber(SP_STACK_MSG* msg, SP_STACK* stack, double number)
{
    SP_STACK_ELEMENT element = {.type = NUMBER , .value = number};
    spStackPush(stack, element, msg);
}

void pushOperation(SP_STACK_MSG* msg, SP_STACK* stack, SP_STACK_ELEMENT_TYPE operation)
{
    SP_STACK_ELEMENT element = {.type = operation , .value = 0};
    spStackPush(stack, element, msg);
}

char* getErrorMessage(SP_STACK_MSG stack_msg, SP_AUX_MSG aux_msg)
{
    /* TODO */
    return "error";
}
