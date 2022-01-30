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

bool stack_init (struct stack* stk)
    {
    stk->array_size = 0;
    stk->Max_size = 2;
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
    if (stack_verify (stk))
       {
       if (stk->array_size >= stk->Max_size)
           {
           printf("ERROR: stack is full\n");
           increase_stack_size (stk);
           }

       *(stk->array + stk->array_size) = number;
       (stk->array_size)++;
       }


    if (stack_verify (stk))
        {
        printf (FORMAT_OF_DATA " pushed\n\n", number);
        return true;
        }

    else
        (stk->array_size)--;
        return false;
    }

//-----------------------------------------------------------------------------

bool stack_pop (struct stack* stk, type_of_data* number)
    {
    if (stack_verify (stk))
        {
        if (stk->array_size == 0)
            {
            printf ("ERROR: stack is empty, element cannot be popped\n");
            return false;
            }
        else
            {
            stk->array_size--;
            *number = *(stk->array + stk->array_size);
            }
        }

    if (stack_verify (stk))
        {
        printf (FORMAT_OF_DATA " popped\n\n", *number);
        return true;
        }

    else
        return false;
    }

//-----------------------------------------------------------------------------

void stack_dump (const struct stack* stk)
    {
    if (stk->array_size > stk->Max_size)
        printf ("WARNING: array_size > Max_size (%d > %d)\n", stk->array_size, stk->Max_size);
    else
        printf ("array_size = [%d]\n\n", stk->array_size);

    for (int i = 0; i < stk->array_size; i++)
        printf ("array[%d] = " FORMAT_OF_DATA "\n", i, *(stk->array + i));

    printf ("=============================================================================\n");

    for (int i = stk->array_size; i < stk->Max_size; i++)
        printf ("array[%d] = " FORMAT_OF_DATA "\n", i, *(stk->array + i));

    printf ("\n");
    }

//-----------------------------------------------------------------------------

bool stack_verify (struct stack* stk)
    {
    bool verification = true;

    if (stk == NULL)
        {
        printf ("ERROR: stack address = NULL\n");
        return false;
        }

    if (stk->array_size < 0)
        {
        printf ("ERROR: array_size = %d (must be >= 0)\n", stk->array_size);
        verification = false;
        }

    if (stk->array_size == 0)
        printf ("WARNING: array_size = %d (no elements to pop)\n", stk->array_size);

    if (stk->array_size == stk->Max_size)
        printf ("WARNING: array_size = Max_size = %d (elements cannot be pushed)\n", stk->array_size);

    if (stk->array_size > stk->Max_size)
        {
        printf ("ERROR: stack is full (array_size > Max_size (%d > %d))\n", stk->array_size, stk->Max_size);
        verification = false;
        }

    if (stk->array == NULL)
        {
        printf ("ERROR: stack->array address = NULL\n");
        verification = false;
        }


    if (verification)
        printf ("Verified\n\n");

    return verification;
    }

//-----------------------------------------------------------------------------

void increase_stack_size (struct stack* stk)
    {
    stk->Max_size *= 2;

    realloc (stk->array, stk->Max_size);

    printf ("stack size [%d] increased (now: [%d])\n", stk->Max_size / 2, stk->Max_size);
    }
