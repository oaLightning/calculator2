
#ifndef __SP_AUX__
#define __SP_AUX__

#include "SP_Stack.h"

/**
 * Defines a messaging mechanism to indicate errors in operations.
 * Each operation will receive a pointer to a SP_AUX_MSG. This will store
 * the message returned by the operation. Each returned message will be defined
 * in the operation later.
 */
typedef enum {
  SP_AUX_SUCCESS = 0,
  SP_AUX_NULL_PARAMETER,
  SP_AUX_LINE_END,
  SP_AUX_ALLOCATION_ERROR,
  SP_AUX_NOT_NUMBER,
  SP_AUX_INTERNAL_PARSE_NUMBER_ERROR,
  SP_AUX_NOT_AN_OPERATION,
} SP_AUX_MSG;

/**
 * Receives a single input line from the user 
 *
 * Messages:
 * 		SP_AUX_ALLOCATION_ERROR - In case allocation failed.
 *
 * @param
 * 		SP_AUX_MSG* msg - Pointer which has the memory location where the message
 * 					   	  will be stored. if msg==NULL then the function doesn't
 * 						  set the value of *msg.
 * @return
 *		A line of text entered by the user
 */
char* getLineFromUser(SP_AUX_MSG* msg);

/**
 * Frees a previously received line from the user
 * @param char* line -  The previously received line to free. If line == NULL nothing
 *                      happens
 */
void freeLineFromUser(char* line);

/**
 * Copies a static message (likley error message) to dynamically allocated memory
 * to unify flows in the code;
 *
 * Messages:
 *    SP_AUX_NULL_PARAMETER - In case message is NULL
 * 		SP_AUX_ALLOCATION_ERROR - In case allocation failed.
 *
 * @param
 * 		SP_AUX_MSG* msg - Pointer which has the memory location where the message
 * 					   	        will be stored. if msg==NULL then the function doesn't
 * 						          set the value of *msg.
 *    char* message   - The message to copy to dynamic memory 
 * @return
 *		A copy of the given message that needs to be freed with
 *    the freeLineFromUser function
 */
char* copyConstMessageToPool(SP_AUX_MSG* msg, char* message);

/**
 * Advances the current position in the line to the next symbol or number 
 * that need to be parsed
 * 
 * Messages:
 *      SP_AUX_LINE_END - Says that the line ends after the current position
 *                        and that there are no more tokens to parse
 *      SP_AUX_NULL_PARAMETER - If the given position in NULL
 * 
 * @param
 *      SP_AUX_MSG* msg - The message passed in that conveys outward the function's status
 *      char* position  - The current place in the line to start looking from
 * 
 * @return
 *      The position in the line where the next token starts at
 */
char* advanceToNextToken(SP_AUX_MSG* msg, char* position);

/**
 * Reads a number from the given input, and returns the place where we should 
 * continue parsing the line from
 * 
 * Messages:
 *      SP_AUX_NULL_PARAMETER - If the given position or next_position is NULL
 *      SP_AUX_NOT_NUMBER     - If the input at the given position isn't a valid number
 *      SP_AUX_INTERNAL_PARSE_NUMBER_ERROR - Says that there was an internal error in
 *                                           parsing the number, ie the number is bigger
 *                                           or equal to 2^32
 * 
 * @param
 *      SP_AUX_MSG* msg - The message passed in that conveys outward the function's status
 *      char* position  - The current place in the line to start parsing from
 *      char** next_position - The place from which to continue parsing the rest of the line
 * 
 * @return
 *      The number parsed from the input
 */
unsigned int parseNumber(SP_AUX_MSG* msg, char* position, char** next_position);

/**
 * Reads an operation from the given input, and returns the place where we should 
 * continue parsing the line from
 * 
 * Messages:
 *      SP_AUX_NULL_PARAMETER - If the given position or next_position is NULL
 *      SP_AUX_NOT_AN_OPERATION - If the input at the given position isn't a valid operation
 * 
 * @param
 *      SP_AUX_MSG* msg - The message passed in that conveys outward the function's status
 *      char* position  - The current place in the line to start parsing from
 *      char** next_position - The place from which to continue parsing the rest of the line
 * 
 * @return
 *      The operation type used (because we only support operations that have an
 *      SP_STACK_ELEMENT_TYPE)
 */
SP_STACK_ELEMENT_TYPE parseOperation(SP_AUX_MSG* msg, char* position, char** next_position);

/**
 * Says if the given line contains only the "quit" message
 * 
 * Messages:
 *      SP_AUX_NULL_PARAMETER - If the given line is NULL
 * 
 * @param
 *      SP_AUX_MSG* msg - The message passed in that conveys outward the function's status
 *      char* line      - The line to compare
 * 
 * @return
 *      Weather or not the program just received a "quit" command
 */
bool isEndMessage(SP_AUX_MSG* msg, char* line);


#endif // __SP_AUX__