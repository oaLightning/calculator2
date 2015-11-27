#ifndef SP_STACK_H_
#define SP_STACK_H_
#include <stdbool.h>

/**
 * Defines a messaging mechanism to indicate errors in operations.
 * Each operation will receive a pointer to a SP_STACK_MSG. This will store
 * the message returned by the operation. Each returned message will be defined
 * in the operation later.
 */
typedef enum  {
	SP_STACK_ERROR_IS_EMPTY = -1,
	SP_STACK_ERROR_ELEMENT_TYPE_NOT_SUPPORTED = -2,
	SP_STACK_ERROR_ALLOCATION_FAILED = -3,
	SP_STACK_ERROR_NULL_ARGUMENT = -4,
	SP_STACK_ERROR_INVALID_ARGUMENT = -5,
	SP_STACK_SUCCESS = 1
} SP_STACK_MSG;

/*************************************************
 * Definition of an element for the stack. The stack
 * element has a value and a type. The value is
 * not defined for all types beside NUMBER type. That
 * is if the type is NUMBER then the value field holds
 * the value of that number, otherwise the value is not defined.
 *************************************************/
typedef enum {
	UNKNOWN 		= 0,

	PLUS 			= 1,
	MINUS 			= 2,
	MULTIPLICATION 	= 3,
	DIVISION 		= 4,
	DOLLAR 			= 5,

	NUMBER          = 10,
} SP_STACK_ELEMENT_TYPE;

/**
 * SP_Stack will hold an element of type SP_STACK_ELEMENT.
 * The element has two fields:
 * 		SP_STACKELEMENT_TYPE type - This enum indicates the type of the element.
 * 		double value -  If the type of the element is Number then value 
 *						will hold the value of the number represented by the
 * 						element. Otherwise there is not defined.
 */
typedef struct {
	SP_STACK_ELEMENT_TYPE type;
	double value;
} SP_STACK_ELEMENT;

/**
 * This is a declaration of struct which represent a stack.
 * You will need to define the stack in SP_Stack.c
 * You can do so by copying the following code to SP_Stack.c:
 *
 * struct sp_stack_struct {
 * 		//Definition goes here
 * };
 */
typedef struct sp_stack_struct SP_STACK;

/**
 * Create a new empty stack.
 *
 * Messages:
 * 		P_STACK_ERROR_ALLOCATION_FAILED - In case allocation failed.
 * 		SP_STACK_SUCCESS - In case creation of the stack succeeded.
 *
 * @param
 * 		SP_STACK_MSG* msg - Pointer which has the memory location where the message
 * 					   		will be stored. if msg==NULL then the function doesn't
 * 							set the value of *msg.
 * @return
 *		A pointer to a new empty stack, if any error occurred NULL is returned.
 */
SP_STACK* spStackCreate(SP_STACK_MSG* msg);

/**
 * Free all allocation associated with the stack.
 * @param
 * SP_STACK* stack - Pointer to a stack which will be deleted.
 * 					 After invoking spStackDestroy, stack will no longer be valid to use.
 * 					 If stack==NULL nothing happens.
 */
void spStackDestroy(SP_STACK* stack);

/**
 * Looks at the top of the stack without removing it.
 * Note - The element returned is the SAME element in the stack. Changing the element
 * will cause in changing the top element value as well.
 * Messages:
 *		SP_STACK_ERROR_NULL_ARGUMENT - stack is NULL.
 *		SP_STACK_ERROR_IS_EMPTY - stack is empty
 *		SP_STACK_SUCCESS - in case of success.
 * @param
 * SP_STACK* stack   - The stack which holds the top element.
 * SP_STACK_MSG* msg - Pointer which has the memory location where the message
 * 					   will be stored. if msg==NULL then the function doesn't
 * 					   set the value of *msg.
 * @return
 * The top element of the stack if the stack is not empty, otherwise NULL is returned.
 */
SP_STACK_ELEMENT* spStackTop (SP_STACK* stack, SP_STACK_MSG* msg);

/**
 * Removes the top of the stack.
 * Messages:
 *		SP_STACK_ERROR_NULL_ARGUMENT - stack is NULL.
 *		SP_STACK_ERROR_IS_EMPTY - stack is empty
 *		SP_STACK_SUCCESS - in case of success.
 * @param
 * SP_STACK* stack   - The stack which holds the top element.
 * SP_STACK_MSG* msg - Pointer which has the memory location where the message
 * 					   will be stored. if msg==NULL then the function doesn't
 * 						set the value of *msg.
 * @return
 * A stack after the pop operation. If the stack is empty the new stack will be
 * the same as the old one. In case stack is NULL
 */
SP_STACK* spStackPop(SP_STACK* stack, SP_STACK_MSG* msg);

/**
 * Pushes a new element to the top of the stack. The element pushed to the stack
 * is a COPY of the argument newElement.
 * Messages:
 *		SP_STACK_ERROR_NULL_ARGUMENT - stack is NULL.
 *		P_STACK_ERROR_ALLOCATION_FAILED - In case allocation failed.
 *		SP_STACK_SUCCESS - in case of success.
 * @param
 * SP_STACK* stack   - The stack which the new element will be added to.
 * SP_STACK_ELEMENT newElement - The new element to be pushed.
 * 								 A copy of this argument will be allocated and
 * 								 pushed to the top of the stack.
 * SP_STACK_MSG* msg - Pointer which has the memory location where the message
 * 					   will be stored. if msg==NULL then the function doesn't
 * 						set the value of *msg.
 * @return
 * If the parameter 'stack is NULL then NULL is returned.
 * If memory allocation fails then 'stack' is returned.('stack' will not be affected)
 * In case of success, the stack after the push operation.
 *
 */
SP_STACK* spStackPush(SP_STACK* stack, SP_STACK_ELEMENT newElement,SP_STACK_MSG* msg);

/**
 * Checks if the stack is Empty.
 * Messages:
 *		SP_STACK_ERROR_NULL_ARGUMENT - stack is NULL.
 *		SP_STACK_SUCCESS - in case of success.
 * @param
 * SP_STACK* stack   - The stack which holds the top element.
 * SP_STACK_MSG* msg - Pointer which has the memory location where the message
 * 					   will be stored. if msg==NULL then the function doesn't
 * 						set the value of *msg.
 * @return
 * true in case stack is empty, otherwise the returned value is false.
 */
bool spStackIsEmpty(SP_STACK* stack, SP_STACK_MSG* msg);

#endif /* SP_STACK_H_ */
