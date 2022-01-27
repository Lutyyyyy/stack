#include <stdio.h>

bool is_digit (char c);

int main()
    {
    char a = '4';
    int res = is_digit(a);
    if (res)
        printf("[%d] %d", res, a - '0');


    return 0;
    }


//-----------------------------------------------------------------------------

bool is_digit (char c)
    {
    int res = c - '0';
    if (res >= 0 && res <= 9)
        return true;

    else
        return false;
    }
