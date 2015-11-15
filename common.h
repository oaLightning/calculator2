
#ifndef __COMMON__
#define __COMMON__

#include "stdio.h"
#include "stdlib.h"

#define VERIFY_CONDITION(cond)  \
    if (!(cond)) {              \
        goto cleanup;           \
    }
    
    
#define VERIFY_CONDITION_AND_SET_ERROR(cond, msg, error_value)  \
    if (!(cond)) {                                              \
        if (NULL != (msg)) {                                    \
            *(msg) = (error_value);                             \
        }                                                       \
        goto cleanup;                                           \
    }



#endif // __COMMON__
