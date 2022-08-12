#include "stack.h"
#include "guard.h"

int main () {
    
    PrepareLogFile();
    Stack_t* stack = ConstructStack (3);

    push (stack, 13);
    push (stack, 157);
    push (stack, 132);

    ResizeStack (stack, 6);

    push (stack, 2);
    push (stack, 21);
    push (stack, 199);
    push (stack, 41);
     
    DestructStack (stack);
    return 0;

}
