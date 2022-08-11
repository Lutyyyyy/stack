#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define Int_t

#ifdef Int_t
    typedef int DataType;
//    DataType CopyData   (DataType data) { return data; } 
    void     PrintData  (DataType data) { printf ("%d", data); }
    void     FprintData (FILE* file, DataType data) { fprintf (file, "%d", data); }
#endif

#ifdef Double_t
    typedef double DataType;
//    DataType CopyData   (DataType data) { return data; } 
    void     PrintData  (DataType data) { printf ("%g", data); }
    void     FprintData (FILE* file, DataType data) { fprintf (file, "%g", data); }
#endif

#ifdef Char_t
    typedef char* DataType;
//    DataType CopyData   (DataType data) { return data; } 
    void     PrintData  (DataType data) { printf ("%s", data); }
    void     FprintData (FILE* file, DataType data) { fprintf (file, "%s", data); }
#endif




#define GET_INFO() struct DumpInfo info = \
    {\
     __LINE__,\
     __FILE__,\
     __PRETTY_FUNCTION__\
    };

typedef unsigned long long canary_t;
const canary_t LcanaryDefault = 0xB1BAB0BA;
const canary_t RcanaryDefault = 0xD01B0EBA;

#define StackBuffer(i)    *((DataType*)(stk->buffer + sizeof (canary_t) + i * sizeof (DataType)))
#define LcanaryValue(stk) *((canary_t*) stk->buffer)
#define RcanaryValue(stk) *((canary_t*) (stk->buffer + sizeof (canary_t) + stk->capacity * sizeof (DataType)))

#define StackPush       push
#define StackPop        pop
#define getName(var)    #var


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
StkError        ResizeStack     (Stack_t* stk, int newSize);

enum StkError   StackPush       (Stack_t* stk, DataType value);
enum StkError   StackPop        (Stack_t* stk, DataType* dest);

enum StkError   StackVerify     (Stack_t* stk);
void            LogDump         (Stack_t* stk, enum StkError errorCode, struct DumpInfo info);

int             DumpCounter     ();
void            PrepareLogFile  ();
//=============================================================================================================================================

int main () {
    
    PrepareLogFile();
    Stack_t* stack = ConstructStack (9);
    enum StkError errorCode = StackVerify (stack);
    push (stack, 13);
    push (stack, 157);
    push (stack, 132);
    printf ("\n\n%p\n\n", stack->buffer);
    ResizeStack (stack, 18);
    printf ("\n\n%p\n\n", stack->buffer);
    
    int temp = 3;
    pop (stack, &temp);
    printf ("%d\n", temp);


    push (stack, 2);
    push (stack, 21);
    push (stack, 201);

    /*
    int ThisCounter = DumpCounter();
    for (int i = 0; i < 10; i++) {
        printf ("%d\n", ThisCounter);
        ThisCounter = DumpCounter();
    }
    */
/*    
    char array[100][100] = {};
    strcpy (array[0], "eber");
    strcpy (array[1], "vls");
    strcpy (array[2], "zdtm");
    strcpy (array[3], ".lo,ikmujnyhbtgvr");/

*/
    
    push (stack, 41);
     
    DestructStack (stack);
    return 0;

}

Stack_t* ConstructStack (int size) {
   
    GET_INFO();

    Stack_t* stk = (Stack_t*) calloc (1, sizeof (Stack_t)); 
    if (stk == NULL) {
        LogDump (stk, STACKBADALLOC, info);
        return NULL;
    }

    stk->buffer = (char*) calloc (size * sizeof (DataType) + 2 * sizeof (canary_t), sizeof (char));
    if (stk->buffer == NULL) {
        LogDump (stk, BUFFERBADALLOC, info);
        return stk;
    }

    stk->Lcanary     = (canary_t*) (stk->buffer);
    *(stk->Lcanary)  = LcanaryDefault;

    stk->Rcanary     = (canary_t*) (stk->buffer + size * sizeof (DataType) + 1 * sizeof (canary_t));
    *(stk->Rcanary)  = RcanaryDefault;

    stk->currentSize = 0;
    stk->capacity    = size;
    
//add hash
    return stk;
}

void DestructStack (Stack_t* stk) {
    
    /* for (int i = 0; i < stk->currentSize + 1; i++) free (stk->array[i]); */
    free (stk->buffer);
    free (stk);
    return;
}

enum StkError StackPush (Stack_t* stk, DataType value) { 

    GET_INFO();
    enum StkError errorCode = StackVerify(stk);
    
    if (errorCode == NOERR) {
        if (stk->currentSize >= stk->capacity) {
            LogDump (stk, STKOVERFLOW, info);
            return STKOVERFLOW;
        }

        StackBuffer (stk->currentSize) = value;
        stk->currentSize++;  
        
        errorCode = StackVerify (stk);
        LogDump (stk, errorCode, info);
        if (errorCode == NOERR) {
            printf ("Pushed ");
            PrintData (value);
            printf ("\n");
        }

        return NOERR;
    }

    LogDump (stk, errorCode, info);
    return errorCode;    
}

enum StkError StackPop (Stack_t* stk, DataType* dest) { 

    GET_INFO();
    enum StkError errorCode = StackVerify(stk);
    
    if (errorCode == NOERR) {
        if (stk->currentSize <= 0) {
            LogDump (stk, STKUNDERFLOW, info);
            return STKUNDERFLOW;
        }
            
        stk->currentSize--;
        *dest = StackBuffer (stk->currentSize);

        errorCode = StackVerify (stk);
        LogDump (stk, errorCode, info);
        if (errorCode == NOERR) {
            printf ("Popped ");
            PrintData (*dest);
            printf ("\n");
        }
        
        return NOERR;
    }

    LogDump (stk, errorCode, info);
    return errorCode;
}

enum StkError ResizeStack (Stack_t* stk, int newSize) {    // ne rabotaet

    GET_INFO();
    enum StkError errorCode = StackVerify(stk);
    
    if (errorCode == NOERR) {
        stk->buffer = (char*) realloc (stk->buffer, newSize * sizeof (DataType) + 2 * sizeof (canary_t));
        if (buffer == NULL) {
            LogDump (stk, STKFAILREALLOC, info);
            return STKFAILREALLOC;
        }
        
        stk->Lcanary      = (canary_t*) stk->buffer;
        stk->Rcanary      = (canary_t*) (stk->buffer + sizeof (canary_t) + newSize * sizeof (DataType));
        stk->capacity     = newSize;

        GET_INFO();
        errorCode = StackVerify(stk);
        
        if (errorCode == NOERR) { 
            printf ("Stack was successfully resized. Current capacity = %d", newSize);
            return NOERR;
        }      
    }

    LogDump (stk, errorCode, info);
    return errorCode;
}

enum StkError StackVerify (Stack_t* stk) {
    if (stk == NULL)
        return STACKBADALLOC;
    
    if (stk->buffer == NULL)
        return BUFFERBADALLOC;
    
    if (stk->currentSize < 0)
        return STKUNDERFLOW;
    
    if (stk->currentSize > stk->capacity)
        return STKOVERFLOW;
    
    if (LcanaryValue(stk) != LcanaryDefault)
        return LEFTCANARYDEAD;

    if (RcanaryValue(stk) != RcanaryDefault)
        return RIGHTCANARYDEAD;

// hash check    
    return NOERR;
}

void LogDump (Stack_t* stk, enum StkError errorCode, struct DumpInfo info) {

    FILE* logfile = fopen ("log.txt", "a");

    fprintf (logfile, "===========================================================================\n"
                      "\n\tDump No. %d\n"
                      "\n===========================================================================\n", DumpCounter());
    
    if (errorCode != NOERR) {
        fprintf (logfile,   "An error has occured in file: %s\n\n"\
                            "Detected in Line: %d \n\n"\
                            "While executing function: %s\n\n"
                            "Error code is %d\n", info.file, info.line, info.function, errorCode);
    }    
    
    else {
        fprintf (logfile, "\nEverything is OK\n\n");
        fprintf (logfile, "current size = %4d\ncapacity     = %4d\n\n", stk->currentSize, stk->capacity);
        
        fprintf (logfile, "Lcanary pointer = %p\nLcanary value = %llu\n\n", stk->Lcanary, LcanaryValue(stk));
        fprintf (logfile, "Rcanary pointer = %p\nRcanary value = %llu\n\n", stk->Rcanary, RcanaryValue(stk));
        
        fprintf (logfile, "      N    data\n---------------------\n");
        
        int i = 0;
        for ( ; i < stk->currentSize; i++) {
            fprintf    (logfile, "arr[%3d] = \"", i);
            FprintData (logfile, StackBuffer (i));
            fprintf    (logfile, "\"\n");
        }
        fprintf (logfile, "---------------------\n\nOther elements of stack are undefined\n");
/*        for ( ; i < stk->capacity; i++) 
        printf ("arr[%3d] is undefined\n", i);*/
    }

    fprintf (logfile, "===========================================================================\n\n\n");   
    
    fclose (logfile);
    return;              
}

int DumpCounter () {
    static int counter = 0;
    return ++counter;
}

void PrepareLogFile() {
    
    FILE* log = fopen ("log.txt", "w");
    assert (log != NULL);
    fclose (log);
    return;
}