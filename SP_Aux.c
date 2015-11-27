/*
 * Auxiliary Functions Module
 */

/* TODO: document internal functions */
/* TODO: re-add push-pop helper functions, and refactor the long functions (calculateExpression, performTopOperation, parseExpressionString) */

#include "SP_Aux.h"
#include "SP_Stack.h"
#include "common.h"

#define CLEAR_MSG(msg) SET_MESSAGE(msg, SP_AUX_SUCCESS)

/*
 * Internal Function Declarations
 */

void parseExpressionString(char *string, SP_STACK_ELEMENT *elements, unsigned int *elements_count, SP_AUX_MSG *msg);
bool isNumber(const char* string);
bool isDigit(char c);
SP_STACK_ELEMENT_TYPE parseOperation(const char* string, SP_AUX_MSG* msg);

double calculateExpression(const SP_STACK_ELEMENT* elements, unsigned int elements_count, SP_AUX_MSG* msg);
void performTopOperation(SP_STACK* numbers_stack, SP_STACK* operations_stack, SP_AUX_MSG* msg);
int getOperationPrecedence(SP_STACK_ELEMENT_TYPE op);
double performOperation(SP_STACK_ELEMENT_TYPE operation, double a, double b, SP_AUX_MSG* msg);
long rangeSum(long a, long b);

/*
 * Function Implementation
 */

void getLineFromUser(char* buffer, int size, SP_AUX_MSG* msg)
{
    char* fgets_result = NULL;

    fgets_result = fgets(buffer, size, stdin);
    VERIFY_CONDITION_AND_SET_ERROR(0 == ferror(stdin), msg, SP_AUX_INPUT_ERROR);
    VERIFY_CONDITION_AND_SET_ERROR(fgets_result != NULL, msg, SP_AUX_INPUT_ERROR);
    
    CLEAR_MSG(msg);
    
cleanup:
    return;
}

bool isEndMessage(const char*string, SP_AUX_MSG* msg)
{
    bool return_value = false;
    int strcmp_result = 0;

    VERIFY_CONDITION_AND_SET_ERROR(NULL != string, msg, SP_AUX_NULL_PARAMETER);

    /* TODO: should this accept strings like "  <>\n" ?? */
    strcmp_result = strcmp(string, "<>\n");
    return_value = (0 == strcmp_result);
    CLEAR_MSG(msg);

cleanup:
    return return_value;
}

double calculateExpressionString(char *string, SP_AUX_MSG* msg)
{
    double result = 0;
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;
    SP_STACK_ELEMENT elements[MAX_LINE_INPUT_LENGTH * 2]; /* TODO: explain */
    unsigned int elements_count;

    VERIFY_CONDITION_AND_SET_ERROR(string != NULL, msg, SP_AUX_NULL_PARAMETER);

    parseExpressionString(string, elements, &elements_count, &aux_msg);
    assert(aux_msg != SP_AUX_NULL_PARAMETER);
    VERIFY_AUX_MSG_OK(aux_msg);

    result = calculateExpression(elements, elements_count, &aux_msg);
    assert(aux_msg != SP_AUX_NULL_PARAMETER && aux_msg != SP_AUX_INVALID_ARGUMENT);
    VERIFY_AUX_MSG_OK(aux_msg);

    CLEAR_MSG(msg);

cleanup:
    if (aux_msg != SP_AUX_SUCCESS) {
        SET_MESSAGE(msg, aux_msg);
    }
    return result;
}

/*
 * Internal Functions Implementation
 */

/* TODO: allocate dynamically ??? */
/**
 * Parse the given string, and  if it represents a valid arithmetic expression,
 * set the elements in a pre-allocated array accordingly.
 * Note: This function alters the given string, putting '\0' chars in it as it is tokenized.
 *
 * Messages:
 *      SP_AUX_NULL_PARAMETER       - If string or elements or elements_count is NULL.
 *      SP_AUX_INVALID_EXPRESSION   - If the string doesn't represent a valid expression.
 *
 * @param
 *      char* string                    - String to parse.
 *      SP_STACK_ELEMENT* elements      - Pre-allocated array to fill with parsed elements
 *      unsigned int* elements_count    - Pointer to variable to update with the parsed elements count.
 *      SP_AUX_MSG* msg                 - Pointer which has the memory location where the message
 * 					   	                  will be stored. if msg==NULL then the function doesn't
 *           						      set the value of *msg.
 */
void parseExpressionString(char* string,
                           SP_STACK_ELEMENT* elements,
                           unsigned int* elements_count,
                           SP_AUX_MSG* msg)
{
    const char* DELIMITERS = " \t\r\n";
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;
    char* token;

    VERIFY_CONDITION_AND_SET_ERROR(string != NULL, msg, SP_AUX_NULL_PARAMETER);
    VERIFY_CONDITION_AND_SET_ERROR(elements != NULL, msg, SP_AUX_NULL_PARAMETER);
    VERIFY_CONDITION_AND_SET_ERROR(elements_count != NULL, msg, SP_AUX_NULL_PARAMETER);
    *elements_count = 0;

    token = strtok(string, DELIMITERS);
    VERIFY_CONDITION_AND_SET_ERROR(token != NULL && isNumber(token),
                                   msg,
                                   SP_AUX_INVALID_EXPRESSION);
    SP_STACK_ELEMENT element = {.type=NUMBER, .value=(double)atoi(token)};
    elements[*elements_count] = element;
    *elements_count += 1;

    while (true) {
        token = strtok(NULL, DELIMITERS);
        if (token == NULL) {
            break;
        }

        SP_STACK_ELEMENT_TYPE operation = parseOperation(token, &aux_msg);
        assert(aux_msg != SP_AUX_NULL_PARAMETER);
        VERIFY_AUX_MSG_OK(aux_msg);
        element.type = operation;
        element.value = 0;
        elements[*elements_count] = element;
        *elements_count += 1;

        token = strtok(NULL, DELIMITERS);
        VERIFY_CONDITION_AND_SET_ERROR(token != NULL && isNumber(token),
                                       msg,
                                       SP_AUX_INVALID_EXPRESSION);
        element.type = NUMBER;
        element.value = (double)atoi(token);
        elements[*elements_count] = element;
        *elements_count += 1;
    }

    assert(*elements_count % 2 == 1);
    CLEAR_MSG(msg);

cleanup:
    if (aux_msg != SP_AUX_SUCCESS) {
        SET_MESSAGE(msg, aux_msg);
    }
    return;
}

/**
 * Check if the given string is a number made of digits only.
 * Note: this function return false for strings such as: "-123", "+123", "1.0".
 *
 * @param
 *      const char* string - String to check.
 *
 * @return
 *      true iff string is a number made of digits only.
 *      if string is NULL false is returned.
 */
bool isNumber(const char* string)
{
    if (string == NULL) {
        return false;
    }

    const char* c = string;

    if (*c == '\0') {
        return false;
    }

    do {
        if (!isDigit(*c)) {
            return false;
        }
        c++;
    } while (*c != '\0');

    return true;
}

/**
 * Check if the given character is a digit.
 *
 * @param
 *      char c - Character to check.
 *
 * @return
 *      true iff c is a digit.
 */
bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

/**
 * Parse the given string, and return an operation if it represents one.
 *
 * Messages:
 *      SP_AUX_NULL_PARAMETER       - If string is NULL.
 *      SP_AUX_INVALID_EXPRESSION   - If the string doesn't represent an operation.
 *
 * @param
 *      const char* string  - String to parse.
 *      SP_AUX_MSG* msg     - Pointer which has the memory location where the message
 * 					   	      will be stored. if msg==NULL then the function doesn't
 * 						      set the value of *msg.
 *
 * @return
 *      Parsed operation.
 */
SP_STACK_ELEMENT_TYPE parseOperation(const char* string, SP_AUX_MSG* msg)
{
    SP_STACK_ELEMENT_TYPE return_value = UNKNOWN;

    VERIFY_CONDITION_AND_SET_ERROR(NULL != string, msg, SP_AUX_NULL_PARAMETER);
    VERIFY_CONDITION_AND_SET_ERROR(strlen(string) == 1, msg, SP_AUX_INVALID_EXPRESSION);

    char c = *string;
    switch (c)
    {
        case '+':
            return_value = PLUS;
            break;
        case '-':
            return_value = MINUS;
            break;
        case '*':
            return_value = MULTIPLICATION;
            break;
        case '/':
            return_value = DIVISION;
            break;
        case '$':
            return_value = DOLLAR;
            break;
        default:
            FAIL(msg, SP_AUX_INVALID_EXPRESSION);
    }

    CLEAR_MSG(msg);

cleanup:
    return return_value;
}

/**
 * Calculate the result of an arithmetic expression of the form:
 *     [num] <op> [num] <op> ... <op> [num]
 *
 * Messages:
 *      SP_AUX_NULL_PARAMETER         - If elements is NULL.
 *      SP_AUX_INVALID_ARGUMENT       - If elements_count == 0,
 *                                      or if the elements are not in the correct form.
 *      SP_AUX_INTERNAL_STACK_ERROR   - If an internal stack operation failed.
 *      SP_AUX_INVALID_RESULT         - If an operation in the expression can't be performed.
 *
 * @param
 *      const SP_STACK_ELEMENT* elements    - Pointer to array of arithmetic elements.
 *      unsigned int elements_count         - Number of elements in the array given.
 *      SP_AUX_MSG* msg                     - Pointer which has the memory location where the message
 * 					   	                      will be stored. if msg==NULL then the function doesn't
 * 						                      set the value of *msg.
 *
 * @return
 *      Calculation result.
 */
double calculateExpression(const SP_STACK_ELEMENT* elements,
                           unsigned int elements_count,
                           SP_AUX_MSG* msg)
{
    double result = 0;
    SP_STACK_MSG stack_msg = SP_STACK_SUCCESS;
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;
    SP_STACK *numbers_stack = NULL;
    SP_STACK *operations_stack = NULL;

    VERIFY_CONDITION_AND_SET_ERROR(elements != NULL, msg, SP_AUX_NULL_PARAMETER);
    VERIFY_CONDITION_AND_SET_ERROR(elements_count > 0, msg, SP_AUX_INVALID_ARGUMENT);

    numbers_stack = spStackCreate(&stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    operations_stack = spStackCreate(&stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    spStackPush(numbers_stack, elements[0], &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    for (const SP_STACK_ELEMENT *element = elements + 1; element < elements + elements_count; ++element) {
        if (element->type == NUMBER) {
            spStackPush(numbers_stack, *element, &stack_msg);
            VERIFY_STACK_MSG_OK(stack_msg);
        } else {
            SP_STACK_ELEMENT_TYPE operation = element->type;
            while (true) {
                bool isEmpty = spStackIsEmpty(operations_stack, &stack_msg);
                VERIFY_STACK_MSG_OK(stack_msg);
                if (isEmpty) {
                    break;
                }

                SP_STACK_ELEMENT *top_element = spStackTop(operations_stack, &stack_msg);
                VERIFY_STACK_MSG_OK(stack_msg);
                SP_STACK_ELEMENT_TYPE top_operation = top_element->type;

                if (getOperationPrecedence(operation) > getOperationPrecedence(top_operation)) {
                    break;
                }

                performTopOperation(numbers_stack, operations_stack, &aux_msg);
                assert(aux_msg != SP_AUX_NULL_PARAMETER);
                VERIFY_AUX_MSG_OK(aux_msg);
            }

            spStackPush(operations_stack, *element, &stack_msg);
            VERIFY_STACK_MSG_OK(stack_msg);
        }
    }

    while (!spStackIsEmpty(operations_stack, NULL)) {
        performTopOperation(numbers_stack, operations_stack, &aux_msg);
        assert(aux_msg != SP_AUX_NULL_PARAMETER);
        VERIFY_AUX_MSG_OK(aux_msg);
    }

    SP_STACK_ELEMENT *top_element = spStackTop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
    result = top_element->value;

    spStackPop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
    assert(spStackIsEmpty(numbers_stack, NULL));
    assert(spStackIsEmpty(operations_stack, NULL));

    CLEAR_MSG(msg);

cleanup:
    if (stack_msg != SP_STACK_SUCCESS) {
        SET_MESSAGE(msg, SP_AUX_INTERNAL_STACK_ERROR);
    }
    if (aux_msg != SP_AUX_SUCCESS) {
        SET_MESSAGE(msg, aux_msg);
    }

    spStackDestroy(operations_stack);
    spStackDestroy(numbers_stack);
    return result;
}

/**
 * Get the precedence value of an operation.
 * This value can be compared against the value of another
 * operation to determine which one precedent the other.
 *
 * @param
 *      SP_STACK_ELEMENT_TYPE operation - Operation to examine.
 *
 * @preconditions
 *      operation is one of {PLUS, MINUS, MULTIPLICATION, DIVISION, DOLLAR}.
 *
 * @return
 *      Operation precedence value.
 */
int getOperationPrecedence(SP_STACK_ELEMENT_TYPE operation)
{
    switch (operation)
    {
        case PLUS:
            return 1;
        case MINUS:
            return 1;
        case MULTIPLICATION:
            return 2;
        case DIVISION:
            return 2;
        case DOLLAR:
            return 3;
        default:
            assert(false);
    }
}

/**
 * Pop the top operation and numbers from the given stacks,
 * perform the operation, and push the result to the number stack.
 *
 * Messages:
 *      SP_AUX_NULL_PARAMETER         - If numbers_stack or operations_stack is NULL.
 *      SP_AUX_INTERNAL_STACK_ERROR   - If an internal stack operation failed.
 *      SP_AUX_INVALID_ARGUMENT       - If the popped operation or numbers are invalid.
 *      SP_AUX_INVALID_RESULT         - If the popped operation can't be performed on the popped numbers.
 *
 * @param
 *      SP_STACK* numbers_stack     - Stack from which to pop and push numbers.
 *      SP_STACK* operations_stack  - Stack from which to pop operation.
 *      SP_AUX_MSG* msg             - Pointer which has the memory location where the message
 * 					   	              will be stored. if msg==NULL then the function doesn't
 * 						              set the value of *msg.
 */
void performTopOperation(SP_STACK* numbers_stack, SP_STACK* operations_stack, SP_AUX_MSG* msg)
{
    SP_STACK_MSG stack_msg = SP_STACK_SUCCESS;
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;

    VERIFY_CONDITION_AND_SET_ERROR(numbers_stack != NULL, msg, SP_AUX_NULL_PARAMETER);
    VERIFY_CONDITION_AND_SET_ERROR(operations_stack != NULL, msg, SP_AUX_NULL_PARAMETER);

    SP_STACK_ELEMENT* top_element = spStackTop(operations_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
    SP_STACK_ELEMENT_TYPE operation = top_element->type;
    spStackPop(operations_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    top_element = spStackTop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
    VERIFY_CONDITION_AND_SET_ERROR(top_element->type == NUMBER, msg, SP_AUX_INVALID_ARGUMENT);
    double x = top_element->value;
    spStackPop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    top_element = spStackTop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
    VERIFY_CONDITION_AND_SET_ERROR(top_element->type == NUMBER, msg, SP_AUX_INVALID_ARGUMENT);
    double y = top_element->value;
    spStackPop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    double result = performOperation(operation, y, x, &aux_msg);
    VERIFY_AUX_MSG_OK(aux_msg);

    SP_STACK_ELEMENT element = {.type=NUMBER, .value=result};
    spStackPush(numbers_stack, element, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    CLEAR_MSG(msg);

cleanup:
    if (stack_msg != SP_STACK_SUCCESS) {
        SET_MESSAGE(msg, SP_AUX_INTERNAL_STACK_ERROR);
    }
    if (aux_msg != SP_AUX_SUCCESS) {
        SET_MESSAGE(msg, aux_msg);
    }
}

/**
 * Perform an arithmetic operation on two numbers in the following order: a <op> b.
 *
 * Messages:
 *      SP_AUX_INVALID_ARGUMENT - If the given operation is invalid.
 *      SP_AUX_INVALID_RESULT   - If operation can't be performed on the given numbers.
 *
 * @param
 *      SP_STACK_ELEMENT_TYPE operation - Operation to perform
 *      double a                        - First operand
 *      double b                        - Second operand
 *      SP_AUX_MSG* msg                 - Pointer which has the memory location where the message
 * 					   	                  will be stored. if msg==NULL then the function doesn't
 * 						                  set the value of *msg.
 *
 * @return
 *      Operation result.
 */
double performOperation(SP_STACK_ELEMENT_TYPE operation, double a, double b, SP_AUX_MSG* msg)
{
    double result = 0;

    switch (operation)
    {
        case PLUS:
            result = a + b;
            break;
        case MINUS:
            result = a - b;
            break;
        case MULTIPLICATION:
            result = a * b;
            break;
        case DIVISION:
            if (b == 0) {
                FAIL(msg, SP_AUX_INVALID_RESULT);
            }
            result = a / b;
            break;
        case DOLLAR:
        {
            long a_ = (long) a;
            long b_ = (long) b;
            if (a_ > b_) {
                FAIL(msg, SP_AUX_INVALID_RESULT);
            }
            result = rangeSum(a_, b_);
            break;
        }
        default:
            FAIL(msg, SP_AUX_INVALID_ARGUMENT);
    }

    CLEAR_MSG(msg);

cleanup:
    return result;
}

/**
 * Calculate the sum of all integers in range [a, b].
 *
 * @param
 *      long a - Range start
 *      long b - Range end
 *
 * @preconditions
 *      a <= b
 *
 * @return
 *      Calculation result.
 */
long rangeSum(long a, long b)
{
    assert(a <= b);
    return (a + b) * (b - a + 1) / 2;
}
