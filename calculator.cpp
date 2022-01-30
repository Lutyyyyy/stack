#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//=============================================================================

#define stack_push push
#define stack_pop pop

const char NUMBER = '0';
double num_buffer[100] = {};   //buffer for numbers got from string
int buf_size = 0;           //first free buffer position


//=============================================================================
//                             Stack functions

typedef double type_of_data;
#define FORMAT_OF_DATA "%lg"

#include "STACK.h"

//=============================================================================
//                        Calculator functions

double calculate (struct stack* stk, char* exp);

char get_operand (char* str);
char* extract_operand (char* str);      //(skipping spaces) sets string pointer to the next element (operand) of the string


double string_to_number (char* str);

bool is_digit (char* str);
char* skip_spaces (char* str);

//=============================================================================


int main()
    {
    struct stack stk = {NULL, 0, 0};
    stack_init (&stk);

    char* expression = "-.234 -4 * 10.4 + 1 2 3 4 + - * /\n";

//    printf ("%lg", string_to_number (expression));

    calculate (&stk, expression);
    stack_memory_free (&stk);

    return 0;
    }

//-----------------------------------------------------------------------------

double calculate (struct stack* stk, char* exp)
    {
    double result = 0;

    while (char op = get_operand (exp))
        {
        double num1 = 0, num2 = 0;

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

                if (num1 != 0.0)
                    push (stk, num2 / num1);
                else
                    printf("ERROR: cannot divide by 0\n");
                break;

            case '\n':
                pop (stk, &result);
                printf ("result = %lg\n", result);
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
        if (is_digit (symbol_pointer))
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
    if (is_digit (operand_pointer))
        {
        if (*operand_pointer == '-')
            i++;

        while (is_digit (operand_pointer + i) && *(operand_pointer + i) != '-')
            i++;
        }

    else
        i = 1;

    return (operand_pointer + i);
    }

//-----------------------------------------------------------------------------

double string_to_number (char* str) //converts digits of string to number (skipping spaces),
    {                            //and sets string pointer to the first non digit element of the string
    int i = 0;
    double result = 0, frac_part = 0;
    char* p = skip_spaces (str);

    int sign = 1;
    if (*p == '-')
        sign = -1;


    if (*p == '-')
        i++;

    while (is_digit (p + i) && *(p + i) != '-' && *(p + i) != '.')
        {
        result *= 10;
        result += p[i++] - '0';
        }

    if (*(p + i) == '.')
        {
        int point_position = i;
        i++;

        while (is_digit (p + i) && *(p + i) != '-' && *(p + i) != '.')
            {
            frac_part *= 10;
            frac_part += p[i++] - '0';
            }

        for (int rank_counter = --i - point_position; rank_counter > 0; rank_counter--)
            frac_part /= 10;

        return sign * (result + frac_part);
        }

    return sign * result;
    }

//-----------------------------------------------------------------------------

bool is_digit (char* str)               // why? -12-4 == -12
    {
    int res = *str - '0';

    if (res >= 0 && res <= 9)
        return true;

    else if ((*str == '-' || *str == '.') && *(str + 1) - '0' >= 0
                                          && *(str + 1) - '0' <= 9)
        return true;

    else if (*str == '-' && *(str + 1) == '.' && *(str + 2) - '0' >= 0
                                              && *(str + 2) - '0' <= 9)
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








