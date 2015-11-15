
#include "SP_Aux.h"
#include "common.h"


char* getLineFromUser(SP_AUX_MSG* msg);

void freeLineFromFser(char* line) {
    if (NULL != line) {
        free(line);
    }
}

char* advanceToNextToken(SP_AUX_MSG* msg, char* position);

unsigned int parseNumber(SP_AUX_MSG* msg, char* position, char** next_position);

SP_STACK_ELEMENT_TYPE parseOperation(SP_AUX_MSG* msg, char* position, char** next_position);

bool isEndMessage(SP_AUX_MSG* msg, char* line);