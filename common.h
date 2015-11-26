
#ifndef __COMMON__
#define __COMMON__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "limits.h"
#include "errno.h"

/* TODO: wrap multiline macros with do { } while */

#define VERIFY_CONDITION(cond)  \
    if (!(cond)) {              \
        goto cleanup;           \
    }
    

#define SET_MESSAGE(msg, value)  \
    if (NULL != (msg)) {         \
        *(msg) = (value);        \
    }

// TODO - Maybe add a debug print in here so we would get a stack trace of
// TODO - errors when we debug. If we do that then this file will probably
// TODO - be useful for the next exercises as well
#define VERIFY_CONDITION_AND_SET_ERROR(cond, msg, error_value)  \
    if (!(cond)) {                                              \
        SET_MESSAGE(msg, error_value);                          \
        goto cleanup;                                           \
    }

#endif // __COMMON__
