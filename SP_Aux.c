
#include "SP_Aux.h"
#include "common.h"

#define CLEAR_MSG(msg) *(msg) = SP_AUX_SUCCESS;


char* getLineFromUser(SP_AUX_MSG* msg);

void freeLineFromFser(char* line) {
    if (NULL != line) {
        free(line);
    }
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