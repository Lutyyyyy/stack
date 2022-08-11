#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define Double_t

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


#define StackPush    push
#define StackPop     pop
#define getName(var) #var



typedef struct Stack_t_ {
    DataType*       array;
    int             currentSize;
    int             capacity;
} Stack_t;

struct DumpInfo {
    int         line;
    const char* file;
    const char* function;
};


enum StkError {
    NOERR            =  0,
    STACKBADALLOC    =  3,
    ARRBADALLOC      =  4,
    DATABADALLOC     =  5,
    STKUNDERFLOW     = -1,
    STKOVERFLOW      =  1488,
    STKFAILREALLOC   =  322,
    DATAFAILREALLOC  =  17,
    FATALERROR       =  666 
};

//=============================================================================================================================================

Stack_t*        ConstructStack  (int size);
void            DestructStack   (Stack_t* stk);
StkError        ResizeStack     (Stack_t* stk, int newSize);

enum StkError   StackPush       (Stack_t* stk, DataType value);
enum StkError   StackPop        (Stack_t* stk, DataType* dest);

void            StackDump       (Stack_t* stk);
void            LogDump         (Stack_t* stk, enum StkError errorCode, struct DumpInfo info);
enum StkError   StackVerify     (Stack_t* stk);
int             DumpCounter     ();
void            PrepareLogFile  ();
//=============================================================================================================================================

int main () {
    
        
    DestructStack (stack);
    return 0;

}

Stack_t* ConstructStack (int size) {
   
    Stack_t*  stk = (Stack_t*)  calloc (1,    sizeof (Stack_t)); 
    DataType* arr = (DataType*) calloc (size, sizeof (DataType));
    
    stk->array       = arr;
    stk->currentSize = 0;
    stk->capacity    = size;
//add defend 
//add hash
    return stk;
}

void DestructStack (Stack_t* stk) {
    
    /* for (int i = 0; i < stk->currentSize + 1; i++) free (stk->array[i]); */
    free (stk->array);
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

        stk->array[stk->currentSize] = value;
        stk->currentSize++;  
        //errorCode = StackVerify (stk);
        //LogDump (stk, errorCode, info);
        //vozmozhna owibka, nuwno li check?
        printf ("Pushed-> ");
        PrintData (value);
        printf ("\n");
        LogDump (stk, NOERR, info);
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
        *dest = stk->array[stk->currentSize];
        printf ("Popped ");
        PrintData (*dest);
        printf ("\n");
        LogDump (stk, NOERR, info);
        return NOERR;
    }

    LogDump (stk, errorCode, info);
    return errorCode;
}

enum StkError ResizeStack (Stack_t* stk, int newSize) {    // ne rabotaet

    GET_INFO();
    enum StkError errorCode = StackVerify(stk);
    
    if (errorCode == NOERR) {
        DataType* ptr = (DataType*) realloc (stk->array, newSize);
        if (ptr == NULL) {
            LogDump (stk, STKFAILREALLOC, info);
            return STKFAILREALLOC;
        }
        //resize comment
        stk->array    = ptr;
        stk->capacity = newSize; 
        return NOERR;
    }

    LogDump (stk, errorCode, info);
    return errorCode;
}

enum StkError StackVerify (Stack_t* stk) {
    if (stk == NULL)
        return STACKBADALLOC;
    
    if (stk->array == NULL)
        return ARRBADALLOC;
    
    if (stk->currentSize < 0)
        return STKUNDERFLOW;
    
    if (stk->currentSize >= stk->capacity)
        return STKOVERFLOW;

// canary and hash check    
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
        fprintf (logfile, "      N    data\n---------------------\n");
        
        int i = 0;
        for ( ; i < stk->currentSize; i++) {
            fprintf    (logfile, "arr[%3d] = \"", i);
            FprintData (logfile, stk->array[i]);
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