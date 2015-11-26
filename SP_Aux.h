/*
 * Auxiliary Functions Module
 */

#ifndef __SP_AUX__
#define __SP_AUX__

#include <stdbool.h>

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

/**
 * Defines a messaging mechanism to indicate errors in operations.
 * Each operation will receive a pointer to a SP_AUX_MSG. This will store
 * the message returned by the operation. Each returned message will be defined
 * in the operation later.
 */
typedef enum {
  SP_AUX_SUCCESS = 0,
  SP_AUX_NULL_PARAMETER,
  SP_AUX_INTERNAL_STACK_ERROR,
  SP_AUX_INVALID_ARGUMENT,
  SP_AUX_INVALID_EXPRESSION,
  SP_AUX_INVALID_RESULT,
  SP_AUX_INPUT_ERROR,
  SP_AUX_PRINT_ERROR,
} SP_AUX_MSG;

#define MAX_LINE_INPUT_LENGTH (200)

/*
 * Functions
 */

/**
 * Receives a single input line from the user 
 *
 * Messages:
 *    SP_AUX_INPUT_ERROR      - In case there is a problem receiving the input
 *                              from the user
 *
 * @param
 *      char* buffer    - pre-allocated buffer to store line in
 *      int size        - buffer size
 * 		SP_AUX_MSG* msg - Pointer which has the memory location where the message
 * 					   	  will be stored. if msg==NULL then the function doesn't
 * 						  set the value of *msg.
 */
void getLineFromUser(char* buffer, int size, SP_AUX_MSG* msg);

/**
 * Says if the given line contains only the "quit" message
 * 
 * Messages:
 *      SP_AUX_NULL_PARAMETER - If the given line is NULL
 * 
 * @param
 *      char* line      - The line to compare
 *      SP_AUX_MSG* msg - The message passed in that conveys outward the function's status
 * 
 * @return
 *      Weather or not the program just received a "quit" command
 */
bool isEndMessage(char* line, SP_AUX_MSG* msg);

/* TODO: doc */
double calculateExpressionLine(char* line, SP_AUX_MSG* msg);

#endif // __SP_AUX__