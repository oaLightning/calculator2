/*
 * Auxiliary Functions Module
 */

#ifndef __SP_AUX__
#define __SP_AUX__

#include <stdbool.h>

/*
 * Utility Macros
 */

#define VERIFY_STACK_MSG_OK(msg)            \
    do {                                    \
        if (SP_STACK_SUCCESS != (msg)) {    \
            goto cleanup;                   \
        }                                   \
    } while (0)

#define VERIFY_AUX_MSG_OK(msg)          \
    do {                                \
        if (SP_AUX_SUCCESS != (msg)) {  \
            goto cleanup;               \
        }                               \
    } while (0)

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
 * Receive a single input line from the user.
 *
 * Messages:
 *    SP_AUX_INPUT_ERROR - In case there is a problem receiving the input
 *                         from the user.
 *
 * @param
 *      char* buffer    - Pre-allocated buffer to store line in.
 *      int size        - Buffer size.
 * 		SP_AUX_MSG* msg - Pointer which has the memory location where the message
 * 					   	  will be stored. if msg==NULL then the function doesn't
 *           			  set the value of *msg.
 */
void getLineFromUser(char* buffer, int size, SP_AUX_MSG* msg);

/**
 * Check if the given string represents the quit command.
 * 
 * Messages:
 *      SP_AUX_NULL_PARAMETER - If the given string is NULL.
 * 
 * @param
 *      char* string    - The string to check.
 *      SP_AUX_MSG* msg - Pointer which has the memory location where the message
 * 					   	  will be stored. if msg==NULL then the function doesn't
 *           			  set the value of *msg.
 * 
 * @return
 *      true iff the string represents a quit command.
 */
bool isEndMessage(char* string, SP_AUX_MSG* msg);

/**
 * Parse the given string, and  if it represents a valid arithmetic expression,
 * calculate it and return the result.
 * Note: This function alters the given string, putting '\0' chars in it as it is tokenized.
 *
 * Messages:
 *      SP_AUX_NULL_PARAMETER       - If string is NULL.
 *      SP_AUX_INVALID_EXPRESSION   - If the string doesn't represent a valid expression.
 *      SP_AUX_INTERNAL_STACK_ERROR - If an internal stack operation failed.
 *      SP_AUX_INVALID_RESULT       - If an operation in the expression can't be performed.
 *
 * @param
 *      char* string    - String to parse.
 *      SP_AUX_MSG* msg - Pointer which has the memory location where the message
 * 					   	  will be stored. if msg==NULL then the function doesn't
 *           			  set the value of *msg.
 *
 * @return
 *      Calculation result.
 */
double calculateExpressionString(char* string, SP_AUX_MSG* msg);

#endif // __SP_AUX__
