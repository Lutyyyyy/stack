#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define StackPush push
#define StackPop  pop


#define DEBUG_MODE 1

#if 

typedef struct Stack_t_ {
    char**          array;
    int             currentSize;
    int             maxSize;
} Stack_t;

enum StkError {
    NOERR            =  0,
    ARRBADALLOC      =  4,
    DATABADALLOC     =  5,
    STKUNDERFLOW     = -1,
    STKOWERFLOW      =  1488,
    STKFAILREALLOC   =  333,
    DATAFAILREALLOC  =  17,
    FATALERROR       =  3558 
};

//=============================================================================================================================================

enum StkError   ConstructStack  (Stack_t* stk, int siz);
enum StkError   DestructStack   (Stack_t* stk);
enum StkError   ResizeStack     (Stack_t* stk);

enum StkError   StackPush       (Stack_t* stk, const char* data);
enum StkError   StackPop        (Stack_t* stk, char* data);

void            StackDump       (Stack_t* stk);

//=============================================================================================================================================

int main () {
    Stack_t stk = {}; /*(Stack_t*) calloc (1, sizeof(Stack_t)); */
    Stack_t* stack = &stk;
    ConstructStack (stack, 9);
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

enum StkError ConstructStack (Stack_t* stk, int size) {
    
    char**   arr = (char**)   calloc (size, sizeof (char*));
    if (arr == NULL)
        return ARRBADALLOC;
    
    stk->array       = arr;
    stk->currentSize = 0;
    stk->maxSize     = size;
//add defend 
//add hash
    return NOERR;
}

enum StkError DestructStack (Stack_t* stk) {
    
    for (int i = 0; i < stk->currentSize + 1; i++)
        free (stk->array[i]); 
    
    free (stk->array);
    free (stk);
    return NOERR;
}

enum StkError StackPush (Stack_t* stk, const char* value, D) { 

    if (stk->currentSize >= stk->maxSize)  
        return STKOWERFLOW;
    
    if (stk->array[stk->currentSize] == NULL) {
        stk->array[stk->currentSize] = (char*) calloc (strlen(value) + 1, sizeof (char)); 
        if (stk->array[stk->currentSize] == NULL)
            return DATABADALLOC;     
    }
        
    else if (strlen(stk->array[stk->currentSize]) < strlen(value)) {
        char* ptr = (char*) realloc (stk->array[stk->currentSize], strlen(value) + 1);
        if (ptr == NULL) 
            return DATAFAILREALLOC;
    }

    strcpy (stk->array[stk->currentSize++], value);
    printf ("Pushed-> %s\n", value);
    return NOERR;
}

enum StkError StackPop (Stack_t* stk, char* dest) { 

    if (stk->currentSize <= 0)
        return STKUNDERFLOW;

    stk->currentSize--;
    strcpy (dest, stk->array[stk->currentSize]);
    printf ("Popped %s\n", dest);
    return NOERR;
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

enum StkError ResizeStack (Stack_t* stk) {

    stk->maxSize *= 2;
    Stack_t* ptr = (Stack_t*) realloc (stk, stk->maxSize);
    if (ptr == NULL)
        return STKFAILREALLOC;

    return NOERR;
}