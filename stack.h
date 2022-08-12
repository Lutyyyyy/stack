#ifndef STACK_H_
#define STACK_H_ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//============ Choose type of data using in stack =============================================================================================
//  Example:
//  #define Int_t
//  typedef int DataType;

#define Int_t
typedef int DataType;

//=============================================================================================================================================



#define StackPush       push
#define StackPop        pop
#define getName(var)    #var


//=============================================================================================================================================

typedef unsigned long long canary_t;

typedef struct Stack_t_ {
    canary_t*       Lcanary;
    
    char*           buffer;
    int             currentSize;
    int             capacity;

    canary_t*       Rcanary;
} Stack_t;

struct DumpInfo {

    int         line;
    const char* file;
    const char* function;
};

enum StkError {
    NOERR            =  0,
    STACKBADALLOC    =  3,
    BUFFERBADALLOC   =  4,
    STKUNDERFLOW     = -1,
    STKOVERFLOW      =  1488,
    STKFAILREALLOC   =  322,
    DATAFAILREALLOC  =  17,
    LEFTCANARYDEAD   =  25,
    RIGHTCANARYDEAD  =  26,
    FATALERROR       =  666

};


//=============================================================================================================================================

Stack_t*        ConstructStack  (int size);
void            DestructStack   (Stack_t* stk);
StkError        ResizeStack     (Stack_t* stk, size_t newSize);

enum StkError   StackPush       (Stack_t* stk, DataType value);
enum StkError   StackPop        (Stack_t* stk, DataType* dest);

//=============================================================================================================================================

#endif