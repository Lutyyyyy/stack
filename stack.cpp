#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define StackPush push
#define StackPop  pop


typedef struct Stack_t_ {
    char**          array;
    int             currentSize;
    int             maxSize;
} Stack_t;

enum StkErrors {
    NOERR            =  0,
    STKBADALLOC      =  4,
    DATABADALLOC     =  5,
    STKUNDERFLOW     = -1,
    STKOWERFLOW      =  1488,
    STKFAILREALLOC   =  333,
    DATAFAILREALLOC  =  17,
    FATALERROR       =  3558
    
};

//=============================================================================================================================================

Stack_t*    ConstructStack  (int size);
void        DestructStack   (Stack_t* stk);
Stack_t*    ResizeStack     (Stack_t* stk);

bool        StackPush       (Stack_t* stk, const char* data);
bool        StackPop        (Stack_t* stk, char* data);

void        StackDump       (Stack_t* stk);

//=============================================================================================================================================

int main () {
    Stack_t* stack = ConstructStack (9);
    push (stack, "eber");
    push (stack, "vls");
    push (stack, "wweqweqewqeqw");


//    ResizeStack (stack);
//    push (stack, 41);

    StackDump (stack);

    char temp[100] = "";
    pop (stack, temp);

    printf ("%s\n", temp);

    StackDump (stack);

    DestructStack (stack);
    return 0;
}

Stack_t* ConstructStack (int size) {
    
    Stack_t* stk = (Stack_t*) calloc (1   , sizeof (Stack_t));
    char**   arr = (char**)   calloc (size, sizeof (char*));
    
    stk->array       = arr;
    stk->currentSize = 0;
    stk->maxSize     = size;

    return stk;
}

void DestructStack (Stack_t* stk) {
    
    for (int i = 0; i < stk->currentSize + 1; i++)
        free (stk->array[i]); 
    free (stk->array);
    free (stk);
    return;
}

bool StackPush (Stack_t* stk, const char* value) { 

    if (stk->currentSize < stk->maxSize) {
        if (stk->array[stk->currentSize] == NULL)
            stk->array[stk->currentSize] = (char*) calloc (strlen(value) + 1, sizeof (char));    
             
        else if (strlen(stk->array[stk->currentSize]) < strlen(value)) {
            char* ptr = (char*) realloc (stk->array[stk->currentSize], strlen(value) + 1);
            if (ptr == NULL) {
                printf ("ERROR: string is too short to contain data argument and cannot be realloced");
                return false;
            }
        }

        strcpy (stk->array[stk->currentSize++], value);
        printf ("Pushed-> %s\n", value);
        return true;
    }

    printf ("Undefined ERROR: cannot push string-> %s\n", value);
    return false;
}

bool StackPop (Stack_t* stk, char* dest) { 

    if (stk->currentSize > 0) {
        stk->currentSize--;
        strcpy (dest, stk->array[stk->currentSize]);

        printf ("Popped %s\n", dest);
        return true;
    }

    printf ("ERROR: cannot pop element from empty stack\n");
    return false;
}

void StackDump (Stack_t* stk) {

    printf ("\n\n================StackDump================\n\ncurrent size = %4d\nmax size     = %4d\n\n", stk->currentSize, stk->maxSize);
    int i = 0;
    printf ("      N    data\n----------------\n");
    for ( ; i < stk->currentSize; i++) 
        printf ("arr[%3d] = \"%s\"\n", i, stk->array[i]);
    
    printf ("----------------\n");
    for ( ; i < stk->maxSize; i++) 
        printf ("arr[%3d] is undefined\n", i);
    printf ("\n\n=========================================\n\n");
    return;
}

Stack_t* ResizeStack (Stack_t* stk) {

    stk->maxSize *= 2;
    Stack_t* ptr = (Stack_t*) realloc (stk, stk->maxSize);

    if (ptr) { 
        printf ("realloced successfully\n");
        return ptr;
    }
    else {
        printf ("Resize failed\n");
        return stk;
    }
}