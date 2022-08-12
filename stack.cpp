#include "stack.h"
#include "guard.h"

#ifdef Int_t
//    DataType CopyData   (DataType data) { return data; } 
    void     PrintData  (DataType data) { printf ("%d", data); }
#endif

#ifdef Double_t
//    DataType CopyData   (DataType data) { return data; } 
    void     PrintData  (DataType data) { printf ("%g", data); }
#endif

#ifdef Char_t
//    DataType CopyData   (DataType data) { return data; } 
    void     PrintData  (DataType data) { printf ("%s", data); }
#endif

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

enum StkError ResizeStack (Stack_t* stk, size_t newSize) {    // ne rabotaet

    GET_INFO();
    enum StkError errorCode = StackVerify(stk);
    
    
    if (errorCode == NOERR) {
        char* term = (char*) realloc (stk->buffer, newSize * sizeof (DataType) + 2 * sizeof (canary_t));
        if (term == NULL) {
            LogDump (stk, STKFAILREALLOC, info);
            return STKFAILREALLOC;
        }
        
        stk->buffer       = term;
        stk->capacity     = newSize;
        
        LcanaryValue(stk) = LcanaryDefault; //norm li tak s tochki zreniya defendirovaniya????
        RcanaryValue(stk) = RcanaryDefault;
        
        GET_INFO();
        errorCode = StackVerify(stk);
        
        if (errorCode == NOERR) { 
            printf ("Stack was successfully resized. Current capacity = %lu\n", newSize);
            return NOERR;
        }      
    }

    LogDump (stk, errorCode, info);
    return errorCode;
}