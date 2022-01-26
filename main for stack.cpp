//#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>


typedef char type_of_data;
#define FORMAT_OF_DATA "%c"


//=============================================================================

// const int Max_size = 8;

//=============================================================================



//=============================================================================

struct stack
{
    type_of_data* array;
    int array_size;
    int Max_size;
};

//=============================================================================



//=============================================================================

bool stack_init (struct stack* stk);
bool stack_memory_free (struct stack* stk);
bool stack_verify (struct stack* stk);

void increase_stack_size (struct stack* stk);


bool stack_push (struct stack* stk, type_of_data  number);
bool stack_pop  (struct stack* stk, type_of_data* number);


void stack_dump (const struct stack* stk);

//=============================================================================

int main()
    {
    struct stack stk = {NULL, 0, 0};
    stack_init (&stk);

    stack_push (&stk, 'a');
    stack_push (&stk, 'b');
    stack_push (&stk, 'c');
    stack_push (&stk, 'x');
    stack_push (&stk, 'z');

    stack_dump (&stk);

    type_of_data x = 0;

    stack_pop (&stk, &x);
    printf("stack_pop = %d\n\n", x);
    stack_dump (&stk);

    stack_pop (&stk, &x);
    printf("stack_pop = %d\n\n", x);
    stack_dump (&stk);

    stack_pop (&stk, &x);
    printf("stack_pop = %d\n\n", x);
    stack_dump (&stk);

    stack_pop (&stk, &x);
    printf("stack_pop = %d\n\n", x);



    stack_memory_free (&stk);


    return 0;
    }

//=============================================================================

bool stack_init (struct stack* stk)
    {
    stk->array_size = 0;
    stk->Max_size = 1;
    stk->array = (type_of_data*) calloc (stk->Max_size, sizeof (*stk->array));

    return true;
    }

//-----------------------------------------------------------------------------

bool stack_memory_free (struct stack* stk)
    {
    free (stk->array);
    stk->array_size = 0;
    stk->array = NULL;

    return true;
    }

//-----------------------------------------------------------------------------

bool stack_push (struct stack* stk, type_of_data number)
    {
    stack_verify (stk);

    if (stk->array_size < stk->Max_size)
        {
        *(stk->array + stk->array_size) = number;
        (stk->array_size)++;
        }

    else
        {
        printf("ERROR: stack is full\n");

        return false;
        }

    stack_verify (stk);

    return true;
    }

//-----------------------------------------------------------------------------

bool stack_pop (struct stack* stk, type_of_data* number)
    {
    stack_verify (stk);

    if (stk->array_size <= 0)
        {
        printf("ERROR: stack is empty\n");
        return false;
        }

    stk->array_size--;
    *number = *(stk->array + stk->array_size);

    stack_verify (stk);

    return true;
    }

//-----------------------------------------------------------------------------

void stack_dump (const struct stack* stk)
    {
    printf("array_size = [%d]\n\n", stk->array_size);

    for (int i = 0; i < stk->array_size; i++)
        printf("array[%d] = " FORMAT_OF_DATA "\n", i, *(stk->array + i));

    printf ("=============================================================================\n");

    for (int i = stk->array_size; i < stk->Max_size; i++)
        printf("array[%d] = " FORMAT_OF_DATA "\n", i, *(stk->array + i));

    printf ("\n");
    }

//-----------------------------------------------------------------------------

bool stack_verify (struct stack* stk)
    {
    bool verification = true;

    if (stk == NULL)
        {
        printf("ERROR: stack address = NULL\n");
        return false;
        }

    if (stk->array_size < 0)
        printf("ERROR: array_size = %d (must be >= 0)\n", stk->array_size);
        verification = false;

    if (stk->array_size > stk->Max_size)
        printf("ERROR: stack is full (array_size > Max_size (%d > %d)\n", stk->array_size, stk->Max_size);
        verification = false;

    if (stk->array == NULL)
        {
        printf("ERROR: stack->array address = NULL\n");
        verification = false;
        }

    if (verification)
        printf("stack verification successfully completed");

    return verification;
    }

//-----------------------------------------------------------------------------

void increase_stack_size (struct stack* stk)
    {
    stk->Max_size *= 2;
    realloc (stk->array, stk->Max_size);
    }


