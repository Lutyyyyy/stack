#ifndef GUARD_H_
#define GUARD_H_


//=============================================================================================================================================


#define GET_INFO() struct DumpInfo info = \
    {\
    __LINE__,\
    __FILE__,\
    __PRETTY_FUNCTION__\
    };


const canary_t LcanaryDefault = 0xB1BAB0BA;
const canary_t RcanaryDefault = 0xD01B0EBA;


#define StackBuffer(i)    *((DataType*) (stk->buffer + sizeof (canary_t) + i * sizeof (DataType)))
#define LcanaryValue(stk) *((canary_t*) stk->buffer)
#define RcanaryValue(stk) *((canary_t*) (stk->buffer + sizeof (canary_t) + stk->capacity * sizeof (DataType)))


//=============================================================================================================================================

enum StkError   StackVerify     (Stack_t* stk);
void            LogDump         (Stack_t* stk, enum StkError errorCode, struct DumpInfo info);

int             DumpCounter     ();
void            PrepareLogFile  ();

//=============================================================================================================================================

#endif