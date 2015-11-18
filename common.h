
#ifndef __COMMON__
#define __COMMON__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "limits.h"
#include "errno.h"

#define VERIFY_CONDITION(cond)  \
    if (!(cond)) {              \
        goto cleanup;           \
    }
    
    
// TODO - Maube add a debug print in here so we would get a stack trace of 
// TODO - errors when we debug. If we do that then this file will probably
// TOOD - be usefull for the next excersices as well
#define VERIFY_CONDITION_AND_SET_ERROR(cond, msg, error_value)  \
    if (!(cond)) {                                              \
        if (NULL != (msg)) {                                    \
            *(msg) = (error_value);                             \
        }                                                       \
        goto cleanup;                                           \
    }



#endif // __COMMON__
