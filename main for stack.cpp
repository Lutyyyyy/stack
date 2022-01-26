#include <stdio.h>

const int Max_size = 8;

struct stack
{
    int array[Max_size];
    int array_size;
};

//=============================================================================

bool stack_init (struct stack* stk);

bool stack_push (struct stack* stk, int  number);
bool stack_pop  (struct stack* stk, int* number);

void stack_dump (const struct stack* stk);



//=============================================================================

int main()
    {
    struct stack stk = {};
    stack_init (&stk);

    stack_push (&stk, 10);
    stack_push (&stk, 20);
    stack_push (&stk, 30);

    stack_dump (&stk);

    int x = 0;

    stack_pop (&stk, &x);
    printf("stack_pop = %d\n", x);
    stack_dump (&stk);

    stack_pop (&stk, &x);
    printf("stack_pop = %d\n", x);
    stack_dump (&stk);

    stack_pop (&stk, &x);
    printf("stack_pop = %d\n", x);
    stack_dump (&stk);

    return 0;
    }

//=============================================================================

bool stack_init (struct stack* stk)
    {
    for (int i = 0; i < Max_size; i++)
        stk->array[i] = 0;

    stk->array_size = 0;

    return true;
    }

//-----------------------------------------------------------------------------

bool stack_push (struct stack* stk, int number)
    {
    if (stk->array_size < Max_size)
        {
        stk->array[stk->array_size++] = number;
        return true;
        }
    else
        return false;
    }

//-----------------------------------------------------------------------------

bool stack_pop (struct stack* stk, int* number)
    {
    if (stk->array_size <= 0)
        return false;

    *number = stk->array [-- stk->array_size];

    return true;
    }

//-----------------------------------------------------------------------------

void stack_dump (const struct stack* stk)
    {
    printf("array_size = [%d]\n\n", stk->array_size);

    for (int i = 0; i < stk->array_size; i++)
        printf("array[%d] = %d\n", i, stk->array[i]);

    printf ("=============================================================================\n");

    for (int i = stk->array_size; i < Max_size; i++)
        printf("array[%d] = %d\n", i, stk->array[i]);

    printf ("\n\n");
    }

//-----------------------------------------------------------------------------

///




