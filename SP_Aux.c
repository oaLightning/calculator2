
#include "SP_Aux.h"
#include "common.h"

#define CLEAR_MSG(msg) *(msg) = SP_AUX_SUCCESS;


char* getLineFromUser(SP_AUX_MSG* msg);

void freeLineFromFser(char* line) {
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
    
cleanup:
    return return_value;
}

char* advanceToNextToken(SP_AUX_MSG* msg, char* position);

unsigned int parseNumber(SP_AUX_MSG* msg, char* position, char** next_position);

SP_STACK_ELEMENT_TYPE parseOperation(SP_AUX_MSG* msg, char* position, char** next_position);

bool isEndMessage(SP_AUX_MSG* msg, char* line) {
    bool return_value = false;
    int strcmp_result = 0;
    
    VERIFY_CONDITION_AND_SET_ERROR(NULL != line, msg, SP_AUX_NULL_PARAMETER);
    
    strcmp_result = strcmp(line, "<>\n");
    return_value = (0 == strcmp_result);
    
cleanup:
    return return_value;
}