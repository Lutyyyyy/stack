#include "stack.h"
#include "guard.h"

#ifdef Int_t
    void     FprintData (FILE* file, DataType data) { fprintf (file, "%d", data); }
#endif

#ifdef Double_t
    void     FprintData (FILE* file, DataType data) { fprintf (file, "%g", data); }
#endif

#ifdef Char_t
    void     FprintData (FILE* file, DataType data) { fprintf (file, "%s", data); }
#endif

//=============================================================================================================================================

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