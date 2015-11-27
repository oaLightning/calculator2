/*
 * Common Utilities
 */


#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>

#define VERIFY_CONDITION(cond)  \
    do {                        \
        if (!(cond)) {          \
            goto cleanup;       \
        }                       \
    } while (0)
    

#define SET_MESSAGE(msg, value)  \
    do {                         \
        if (NULL != (msg)) {     \
            *(msg) = (value);    \
        }                        \
    } while (0)

#define FAIL(msg, error_value)          \
    do {                                \
        SET_MESSAGE(msg, error_value);  \
        goto cleanup;                   \
    } while (0)

#define VERIFY_CONDITION_AND_SET_ERROR(cond, msg, error_value)  \
    do {                                                        \
        if (!(cond)) {                                          \
            FAIL(msg, error_value);                             \
        }                                                       \
    } while (0)

#endif // __COMMON__
