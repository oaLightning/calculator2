/*
 * Auxiliary Functions Module
 */

/* TODO: document internal functions */

#include "SP_Aux.h"
#include "SP_Stack.h"
#include "common.h"

#define CLEAR_MSG(msg) SET_MESSAGE(msg, SP_AUX_SUCCESS)

/*
 * Internal Function Declarations
 */

void parseLine(char* line, SP_STACK_ELEMENT* elements, unsigned int* elements_count, SP_AUX_MSG* msg);
bool isNumber(char* string);
bool isDigit(char c);
SP_STACK_ELEMENT_TYPE parseOperation(char* string, SP_AUX_MSG* msg);

double calculateExpression(SP_STACK_ELEMENT* elements, unsigned int elements_count, SP_AUX_MSG* msg);
void performTopOperation(SP_STACK* numbers_stack, SP_STACK* operations_stack, SP_AUX_MSG* msg);
int getOperationPrecendence(SP_STACK_ELEMENT_TYPE op);
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

bool isEndMessage(char* line, SP_AUX_MSG* msg)
{
    bool return_value = false;
    int strcmp_result = 0;

    VERIFY_CONDITION_AND_SET_ERROR(NULL != line, msg, SP_AUX_NULL_PARAMETER);

    strcmp_result = strcmp(line, "<>\n");
    return_value = (0 == strcmp_result);
    CLEAR_MSG(msg);

cleanup:
    return return_value;
}

double calculateExpressionLine(char* line, SP_AUX_MSG* msg)
{
    double result = 0;
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;
    SP_STACK_ELEMENT elements[MAX_LINE_INPUT_LENGTH];
    unsigned int elements_count;

    parseLine(line, elements, &elements_count, &aux_msg);
    VERIFY_AUX_MSG_OK(aux_msg);

    result = calculateExpression(elements, elements_count, &aux_msg);

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
void parseLine(char* line,
               SP_STACK_ELEMENT* elements,
               unsigned int* elements_count,
               SP_AUX_MSG* msg)
{
    const char* DELIMITERS = " \t\r\n";
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;
    char* token;

    VERIFY_CONDITION_AND_SET_ERROR(elements_count != NULL, msg, SP_AUX_NULL_PARAMETER);
    *elements_count = 0;

    token = strtok(line, DELIMITERS);
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

bool isNumber(char* string)
{
    if (string == NULL) {
        return false;
    }

    char* c = string;

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
bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

SP_STACK_ELEMENT_TYPE parseOperation(char* string, SP_AUX_MSG* msg)
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

double calculateExpression(SP_STACK_ELEMENT* elements,
                           unsigned int elements_count,
                           SP_AUX_MSG* msg)
{
    double result = 0;
    SP_STACK_MSG stack_msg = SP_STACK_SUCCESS;
    SP_AUX_MSG aux_msg = SP_AUX_SUCCESS;
    SP_STACK *numbers_stack = NULL;
    SP_STACK *operations_stack = NULL;

    VERIFY_CONDITION_AND_SET_ERROR(elements_count > 0, msg, SP_AUX_INVALID_ARGUMENT);

    numbers_stack = spStackCreate(&stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    operations_stack = spStackCreate(&stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    spStackPush(numbers_stack, elements[0], &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    for (SP_STACK_ELEMENT *element = elements + 1; element < elements + elements_count; ++element) {
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

                if (getOperationPrecendence(operation) > getOperationPrecendence(top_operation)) {
                    break;
                }

                performTopOperation(numbers_stack, operations_stack, &aux_msg);
                VERIFY_AUX_MSG_OK(aux_msg);
            }

            spStackPush(operations_stack, *element, &stack_msg);
            VERIFY_STACK_MSG_OK(stack_msg);
        }
    }

    while (!spStackIsEmpty(operations_stack, NULL)) {
        performTopOperation(numbers_stack, operations_stack, &aux_msg);
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

int getOperationPrecendence(SP_STACK_ELEMENT_TYPE op)
{
    switch (op)
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
    double x = top_element->value;
    spStackPop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);

    top_element = spStackTop(numbers_stack, &stack_msg);
    VERIFY_STACK_MSG_OK(stack_msg);
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
            assert(false);
    }

    CLEAR_MSG(msg);

cleanup:
    return result;
}

long rangeSum(long a, long b)
{
    assert(a <= b);
    return (a + b) * (b - a + 1) / 2;
}
