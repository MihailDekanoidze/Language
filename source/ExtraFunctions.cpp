#include <math.h>
#include "../include/ExtraFunctions.h"
#include "stdio.h"

static const double EPSILON = 0.000001;

int CmpDbl (const double a, const double b)
{
    return (fabs(a - b) < EPSILON || (isnan(a) && isnan(b)));
}

void printf_n_sym(const char sym, const size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        printf("%c", sym);
    }
}

int char_to_int(const char sym)
{
    int number = 0;
    number = (int)sym;

    return number;
}