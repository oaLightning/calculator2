#include "SP_Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FAIL(msg) do {\
		fprintf(stderr,"%s Line %d: %s", __FILE__, __LINE__, msg);\
		return false;\
	} while(0)

#define ASSERT_TRUE(expression) do { \
                if(!((expression))) { \
                        FAIL("expression is false :: "); \
                } \
        } while (0)

#define ASSERT_FALSE(expression) do { \
                if((expression)) { \
                        FAIL("expression is true  ::"); \
                } \
		} while (0)

#define RUN_TEST(f) do { \
			if(f()==true){ \
				fprintf(stderr, "%s  PASSS\n",#f);\
			}else{ fprintf(stderr, "%s  FAIL\n",#f);\
			} }while (0)

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

int main(){
	RUN_TEST(TEST1);
	return 0;
}
