

#include "common.h"
#include "SP_Stack.h"
#include "SP_Aux.h"

#define VERIFY_STACK_MSG_OK(msg)        \
    if (SP_STACK_SUCCESS != (msg)) {    \
        goto cleanup;                   \
    }

int main() {
    int return_value = -1;
    SP_STACK_MSG msg = SP_STACK_SUCCESS;
    SP_STACK* numbers_stack = NULL;
    SP_STACK* operations_stack = NULL;
    
    numbers_stack = spStackCreate(&msg);
    VERIFY_STACK_MSG_OK(msg);
    
    operations_stack = spStackCreate(&msg);
    VERIFY_STACK_MSG_OK(msg);
    
    
cleanup:
    spStackDestroy(operations_stack);
    spStackDestroy(numbers_stack);
    
    return return_value;
    
}