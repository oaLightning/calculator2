/*
 * Main Module
 */

#include "common.h"
#include "SP_Aux.h"

/*
 * Function Declarations
 */

bool interact(SP_AUX_MSG* msg);

/*
 * Functions
 */

int main()
{
    int return_value = EXIT_FAILURE;
    SP_AUX_MSG msg = SP_AUX_SUCCESS;

    bool should_exit = false;
    while (!should_exit) {
        should_exit = interact(&msg);
        VERIFY_AUX_MSG_OK(msg);
    }

    return_value = EXIT_SUCCESS;

cleanup:
    if (msg != SP_AUX_SUCCESS) {
        fprintf(stderr, "Error (%d)", msg);
    }
    return return_value;
}

/* TODO: document */
bool interact(SP_AUX_MSG* msg)
{
    bool should_exit = false;
    char* calculatedLine = NULL;
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;

    char line[MAX_LINE_INPUT_LENGTH + 1];
    getLineFromUser(line, sizeof(line), &aux_msg);
    VERIFY_AUX_MSG_OK(aux_msg);

    should_exit = isEndMessage(line, &aux_msg);
    VERIFY_AUX_MSG_OK(aux_msg);

    int printf_retval;
    if (should_exit) {
        printf_retval = printf("Exiting...\n");
    } else {
        double expression_result = calculateExpressionString(line, &aux_msg);
        switch (aux_msg) {
            case SP_AUX_SUCCESS:
                printf_retval = printf("res = %f\n", expression_result);
                break;
            case SP_AUX_INVALID_EXPRESSION:
                printf_retval = printf("Invalid Expression!\n");
                break;
            case SP_AUX_INVALID_RESULT:
                printf_retval = printf("Invalid Result!\n");
                break;
            default:
                goto cleanup;
        }
        aux_msg = SP_AUX_SUCCESS;
    }

    VERIFY_CONDITION_AND_SET_ERROR(printf_retval >= 0, msg, SP_AUX_PRINT_ERROR);
    VERIFY_CONDITION_AND_SET_ERROR(ferror(stdout) == 0, msg, SP_AUX_PRINT_ERROR);

    SET_MESSAGE(msg, SP_AUX_SUCCESS);

cleanup:
    if (aux_msg != SP_AUX_SUCCESS) {
        SET_MESSAGE(msg, aux_msg);
    }

    free(calculatedLine);
    return should_exit;
}
