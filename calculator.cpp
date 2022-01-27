#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//=============================================================================

#define stack_push push
#define stack_pop pop

const char NUMBER = '0';
int num_buffer[100] = {};   //buffer for numbers got from string
int buf_size = 0;           //first free buffer position


//=============================================================================
//                             Stack functions

typedef int type_of_data;
#define FORMAT_OF_DATA "%d"

#include "STACK.h"

//=============================================================================
//                        Calculator functions

int calculate (struct stack* stk, char* exp);

char get_operand (char* str);
char* extract_operand (char* str);      //(skipping spaces) sets string pointer to the next element (operand) of the string


int string_to_number (char* str);

bool is_digit (char c);
char* skip_spaces (char* str);

//=============================================================================


int main()
    {
    struct stack stk = {NULL, 0, 0};
    stack_init (&stk);

    char* expression = "\t4 4 + 2 3 + - 6 * 9 -\n";

    int res = calculate (&stk, expression);

    stack_memory_free (&stk);

    return 0;
    }

//-----------------------------------------------------------------------------

int calculate (struct stack* stk, char* exp)
    {
    int result = 0;

    while (char op = get_operand (exp))
        {
        int num1 = 0, num2 = 0;

        switch (op)
            {
            case NUMBER:
                push (stk, num_buffer[--buf_size]);
                break;

            case '+':
                pop (stk, &num1);
                pop (stk, &num2);

                push (stk, num1 + num2);
                break;

            case '*':
                pop (stk, &num1);
                pop (stk, &num2);

                push (stk, num1 * num2);
                break;

            case '-':
                pop (stk, &num1);
                pop (stk, &num2);

                push (stk, num2 - num1);
                break;

            case '/':
                pop (stk, &num1);
                pop (stk, &num2);

                if (num1)
                    push (stk, num2 / num1);
                else
                    printf("ERROR: cannot divide by 0\n");
                break;

            case '\n':
                pop (stk, &result);
                printf ("result = %d\n", result);
                break;

            default:
                printf ("ERROR: expression contains invalid symbols\n");
                return -1;
            }

        exp = extract_operand (exp);
        }

    return result;
    }

//-----------------------------------------------------------------------------

char get_operand (char* str)
    {
    char* symbol_pointer = skip_spaces (str);

    if (*symbol_pointer != EOF)
        {
        if (is_digit (*symbol_pointer))
            {
            num_buffer[buf_size++] = string_to_number (symbol_pointer);
            return NUMBER;
            }

        else
            return *symbol_pointer;
        }

    else
        return 0;
    }

//-----------------------------------------------------------------------------

char* extract_operand (char* str)
    {
    char* operand_pointer = skip_spaces (str);

    if(*operand_pointer == EOF)
        return operand_pointer;

    int i = 0;
    if (is_digit (*operand_pointer))
        {
        while (is_digit (*(operand_pointer + i)))
            i++;
        }

    else
        i = 1;

    return (operand_pointer + i);
    }

//-----------------------------------------------------------------------------

int string_to_number (char* str) //converts digits of string to number (skipping spaces),
    {                            //and sets string pointer to the first non digit element of the string
    int i = 0, result = 0;
    char* p = skip_spaces (str);

    while (is_digit (p[i]))
        {
        result *= 10;
        result += p[i++] - '0';
        }

    return result;
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

//-----------------------------------------------------------------------------

char* skip_spaces (char* str)
    {
    int i = 0;
    while (*(str + i) == ' ' || *(str + i) == '\t')
        i++;

    return str + i;
    }








