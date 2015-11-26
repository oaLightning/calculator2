#include "SP_Stack.h"
#include <stdio.h>

#define FAIL(msg) do {\
		fprintf(stderr,"%s Line %d: %s", __FILE__, __LINE__, msg);\
		return false;\
	} while(0)

#define ASSERT_TRUE(expression) do { \
                if(!((expression))) { \
                        FAIL("expression is false :: "); \
                } \
        } while (0)

#define RUN_TEST(f) do { \
			if(f()==true){ \
				fprintf(stderr, "%s  PASS\n",#f);\
			}else{ fprintf(stderr, "%s  FAIL\n",#f);\
			} }while (0)

#define ASSERT_STACK_EMPTINESS(stack, expected_value)				\
	do { 															\
		SP_STACK_MSG _msg; 											\
		bool _isEmpty = spStackIsEmpty((stack), &_msg); 			\
		ASSERT_TRUE(_msg == SP_STACK_SUCCESS); 						\
		ASSERT_TRUE(_isEmpty == expected_value);	 				\
	} while(0)

#define ASSERT_STACK_EMPTY(stack) ASSERT_STACK_EMPTINESS(stack, true)
#define ASSERT_STACK_NOT_EMPTY(stack) ASSERT_STACK_EMPTINESS(stack, false)

bool TEST1(){
	SP_STACK_MSG msg;
	SP_STACK_ELEMENT* elementPtr = NULL;
	SP_STACK * stack = spStackCreate(&msg);
	ASSERT_TRUE(msg == SP_STACK_SUCCESS && stack!=NULL);
	elementPtr = spStackTop(stack,&msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_IS_EMPTY && elementPtr == NULL);
	SP_STACK_ELEMENT newElement = {.type = NUMBER , .value = 100};
	stack = spStackPush(stack,newElement,&msg);
	ASSERT_TRUE(msg == SP_STACK_SUCCESS);
	elementPtr = spStackTop(stack,&msg);
	ASSERT_TRUE(elementPtr->type == newElement.type && elementPtr->value == newElement.value && msg==SP_STACK_SUCCESS);
	spStackDestroy(stack);
	return 1;
}

bool TEST_FULL_SCENARIO(){
	SP_STACK_MSG msg;
	SP_STACK_ELEMENT* elementPtr;
	SP_STACK* returnedStack;

	SP_STACK* stack = spStackCreate(&msg);
	ASSERT_TRUE(msg == SP_STACK_SUCCESS && stack != NULL);
	ASSERT_STACK_EMPTY(stack);

	elementPtr = spStackTop(stack,&msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_IS_EMPTY && elementPtr == NULL);
	ASSERT_STACK_EMPTY(stack);

	stack = spStackPop(stack, NULL);
	ASSERT_TRUE(stack != NULL);
	stack = spStackPop(stack, &msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_IS_EMPTY && stack != NULL);
	ASSERT_STACK_EMPTY(stack);

	SP_STACK_ELEMENT element = {.type = NUMBER , .value = 0};
	for (int i = 1; i <= 1024; ++i) {
		element.value = i;
		returnedStack = spStackPush(stack, element, &msg);
		ASSERT_TRUE(msg == SP_STACK_SUCCESS && returnedStack == stack);
		ASSERT_STACK_NOT_EMPTY(stack);
	}

	for (int i = 1024; i  >= 1; --i) {
		ASSERT_STACK_NOT_EMPTY(stack);

		elementPtr = spStackTop(stack, &msg);
		ASSERT_TRUE(msg == SP_STACK_SUCCESS && elementPtr != NULL);
		ASSERT_TRUE(elementPtr->type == NUMBER && elementPtr->value == i);

		stack = spStackPop(stack, &msg);
		ASSERT_TRUE(msg == SP_STACK_SUCCESS && stack != NULL);
	}

	ASSERT_STACK_EMPTY(stack);

	stack = spStackPop(stack, &msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_IS_EMPTY && stack != NULL);

	spStackDestroy(stack);

	return true;
}

bool TEST_NULL_ARGUMENTS()
{
	SP_STACK_MSG msg;
	SP_STACK_ELEMENT* elementPtr;
	SP_STACK* returnedStack;

	spStackIsEmpty(NULL, NULL);
	spStackIsEmpty(NULL, &msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_NULL_ARGUMENT);

	elementPtr = spStackTop(NULL, NULL);
	ASSERT_TRUE(elementPtr == NULL);
	elementPtr = spStackTop(NULL, &msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_NULL_ARGUMENT && elementPtr == NULL);


	returnedStack = spStackPop(NULL, NULL);
	ASSERT_TRUE(returnedStack == NULL);
	returnedStack = spStackPop(NULL, &msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_NULL_ARGUMENT && returnedStack == NULL);

	SP_STACK_ELEMENT element = {.type = NUMBER , .value = 0};
	returnedStack = spStackPush(NULL, element, NULL);
	ASSERT_TRUE(returnedStack == NULL);
	returnedStack = spStackPush(NULL, element, &msg);
	ASSERT_TRUE(msg == SP_STACK_ERROR_NULL_ARGUMENT && returnedStack == NULL);

	spStackDestroy(NULL);

	SP_STACK* stack = spStackCreate(NULL);
	if (stack != NULL) {
		spStackDestroy(stack);
	}

	return true;
}

int main(){
	RUN_TEST(TEST1);
	RUN_TEST(TEST_NULL_ARGUMENTS);
	RUN_TEST(TEST_FULL_SCENARIO);
	return 0;
}
