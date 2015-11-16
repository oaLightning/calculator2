
// TODO - Think if we want to skip tabs as well in advanceToNextToken

#include "SP_Aux.h"
#include "common.h"

#define CLEAR_MSG(msg) *(msg) = SP_AUX_SUCCESS;
#define MAX_LINE_LENGTH (200)

char* getLineFromUser(SP_AUX_MSG* msg) {
    char* line = NULL;
    char* fgets_result = NULL;
    bool got_data = false;
    
    // Note - There is no line on purpose, so that it looks like an interperter
    printf(">>");
    
    line = malloc(*line * (MAX_LINE_LENGTH + 1));
    VERIFY_CONDITION_AND_SET_ERROR(NULL != line, msg, SP_AUX_ALLOCATION_ERROR);
    
    fgets_result = fgets(line, MAX_LINE_LENGTH, stdin);
    VERIFY_CONDITION_AND_SET_ERROR(0 == ferror(stdin), msg, SP_AUX_INPUT_ERROR);
    VERIFY_CONDITION_AND_SET_ERROR(fgets_result != NULL, msg, SP_AUX_INPUT_ERROR);
    
    got_data = true;
    CLEAR_MSG(msg);
    
cleanup:
    if (!got_data) {
        free(line);
    }
    return line;
}

void freeLineFromUser(char* line) {
    if (NULL != line) {
        free(line);
    }
}

char* copyConstMessageToPool(SP_AUX_MSG* msg, char* message) {
    char* return_value = NULL;
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != message, msg, SP_AUX_NULL_PARAMETER);
    // Note - Here we take into account the null byte at the end of the line
    int size_to_alloc = strlen(message) + sizeof(*message);
    
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
    char* current_position = position;
    const char CHAR_TO_PASS = ' ';
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != position, msg, SP_AUX_NULL_PARAMETER);
    
    while(*current_position) {
        if (CHAR_TO_PASS == *current_position) {
            current_position++;
        }
        else {
            return_value = current_position;
            break;
        }
    }
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != return_value, msg, SP_AUX_LINE_END);
    CLEAR_MSG(msg);

cleanup:
    return return_value;
}

unsigned int parseNumber(SP_AUX_MSG* msg, char* position, char** next_position);

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