/*
Copyright 2020 <COPYRIGHT HOLDER>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "bigint.h"
#include "../fioFormatHeader.h"
#include "stdio.h"
#include <math.h>

#define BASE 10
#define CHAR_OFFSET '0'

/* need to track this down */
typedef long long intmax_t;

static char conversion [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static void output_padding
(
    size_t size,
    char what,
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine, /* handler for args as they're formatted */
    size_t outarg               /* argument to routine */
)
    {
    while(size > 0)
        {
        outRoutine(&what, 1, outarg);
        --size;
        }
    }

#define LONG_LONG_MAX (1ULL << ((sizeof(long long) * 8) - 1))
#define LONG_LONG_NEG_MASK (LONG_LONG_MAX - 1)
static char *put_cell_full(int num, char*where)
    {
    memset (where, 0, CELL_SIZE *sizeof (char));
    char *start = where + CELL_SIZE - 1;
    int sz = CELL_SIZE;
    while(sz > 0)
        {
        *start = (num % 10) + '0';
        num /= 10;
        --start;
        --sz;
        }
    return start;
    }

static int simple_log(unsigned long val)
    {
    unsigned long base = 10;
    for (int i = 0; i < CELL_SIZE; ++i)
        {
        if (val < base)
            return i;
        base *= 10;
        }
    return CELL_SIZE;
    }

void handle_float_output_e
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
)
    {
    /* No long doubles for now */
    double val = va_arg(vaList, double);
    char outBuf[2000];
    char *cell_iter = outBuf + CELL_SIZE; /* Considerations for rounding */
    char *start = cell_iter;
    int isNeg = val < 0;
    int num_size;
    char *dec_point = NULL;
    char exp_out[10];
    char *exp_out_iter = exp_out + 8;
    *exp_out_iter = '\0';
    int exp_denote_sz;

    if (isnan(val) != 0) /* Spoof a value */
        {
        start = "nan";
        num_size = 3;
        }
    else if (isinf(val) != 0)
        {
        start = "inf";
        num_size = 3;
        }
    else /* General case, construct the value */
        {

        struct bigInt baseVal;
        initBigInt(&baseVal);
        addBigInt(&baseVal, 1);

        int exponent;
        double mantissa = 0.0f;
        mantissa = frexp(val, &exponent);
        
        if (val == 0.0f)
            {
            mantissa = 0;
            }
        if (exponent > 0)
            {
            while (exponent > 0)
                {
                multBySingleDigit(&baseVal, 2);
                --exponent;
                }
            }
        else
            {
            while (exponent < 0)
                {
                divBySingleDigit(&baseVal, 2);
                ++exponent;
                }
            }
        
        /* Start calc */
        struct bigInt acc;
        struct bigInt copied;
        initBigInt(&acc);
        initBigInt(&copied);

        while(mantissa != 0.0f)
            {
            divBySingleDigit(&baseVal, 10);
            mantissa *= 10;
            int digit = (int)(mantissa); /* Maybe use 0.5f to round? */
            memcpy(&copied,  &baseVal, sizeof(copied));
            multBySingleDigit(&copied, digit);
            combineBigInt(&acc, &copied);
            mantissa -= digit;
            }
        // Start off with a baseline value for values absolutely larger than 1
        int iter = acc.next - 1;
        int base_10_exp = (acc.next - 1 - (sizeof(acc.val)/sizeof(*(acc.val)))/2 - 1) * CELL_SIZE;
        base_10_exp += simple_log(acc.val[acc.next - 1]);

        if (base_10_exp == 0 && acc.val[acc.next - 1] == 0)
            {
            
            iter = acc.dec_point;
            while(acc.val[iter] == 0)
                {
                base_10_exp -= CELL_SIZE;
                --iter;
                }
            base_10_exp -= CELL_SIZE - simple_log(acc.val[iter]); // Flipped because we are going "downwards"
            }
        do
            {
            put_cell_full(acc.val[iter], cell_iter);
            cell_iter += CELL_SIZE;
            --iter;
            }
        while (iter > acc.dec_next);

        // Skip leading 0s
        for(int i = 0; i < CELL_SIZE - 1; ++i)
            {
            if(*start == '0')
                ++start;
            else
                break;
            }
        //*cell_iter = 0;
        // NOTE: NEED rounding
        if(cell_iter > start + values->prec + 1)
            {
            start[values->prec + 2] = '\0';
            }
        else
            {
            cell_iter[0] = '\0';
            }

        // Handle alt @This is wrong but whatever
        if(((values->flags & FIO_FLAG_ALT_BIT) == 0 && (values->prec == 0)))
            {
            start[1] = '\0';
            num_size = 1; // Only one digit printed
            }
        else
            {
            start[-1] = start[0];
            start[0] = '.';
            dec_point = start;
            start = start - 1;
            num_size = values->prec + 2;
            }

            {
            exp_out_iter[1] = 0;/* ensure there is an end */
            int val = base_10_exp < 0 ? -base_10_exp: base_10_exp;
            do
                {
                *exp_out_iter = conversion[val % 10];
                --exp_out_iter;
                val /= 10;
                }
            while (val > 0ULL);
            if (exp_out_iter - exp_out > 6)
                {
                *exp_out_iter = '0';
                --exp_out_iter;
                }
            if (base_10_exp < 0)
                {
                *exp_out_iter = '-';
                }
            else
                {
                *exp_out_iter = '+';
                }
                --exp_out_iter;
                *exp_out_iter = 'e';
            }
        exp_denote_sz = exp_out + 9 - exp_out_iter;
        }

#if 1
    size_t lead_zero_size = 0; // Just for compatibility. In our case there is no leading 0s... via precision

    size_t sign_size = 1;
    char * sign = "";
    if (isNeg)
        {
        sign = "-";
        }
    else if ((values->flags & FIO_FLAG_IS_BIT) != 0)
        {
        sign = "+";
        }
    else if ((values->flags & FIO_FLAG_ISS_BIT) != 0)
        {
        sign = " ";
        }
    else
        {
        sign_size = 0;
        /* Nothing to do */
        }

    size_t padding_size = 0;

    if((values->flags & FIO_FIELD_WIDTH_BIT) != 0)
        {
        if(values->field_width > num_size + lead_zero_size + sign_size)
            {
            /* If the 0 flag is specified (note that there cannot be a precision)
             * pad the fieldwidth with zeroes similar to a precision instead of
             * with spaces
             */
            
            if ((values->flags & FIO_FLAG_PZ_BIT) != 0)
                {
                lead_zero_size = values->field_width - (num_size + sign_size);
                }
            else
                {
                padding_size = values->field_width - (num_size + lead_zero_size + sign_size);
                }

            }
        }

    size_t overall_size = num_size + lead_zero_size + sign_size + padding_size + exp_denote_sz;

    if((values->flags & FIO_FLAG_LJ_BIT) != 0)
        {
        outRoutine(sign, sign_size, outarg); // Could make this an if sign_size != 0
        outRoutine(start, num_size, outarg); // Could make this an if sign_size != 0
        if (!(cell_iter > dec_point + values->prec + 1)) /* Complete the number if for large precision*/
            {
            output_padding(dec_point + values->prec + 1 - cell_iter, '0', outRoutine, outarg);
            }
        outRoutine(exp_out_iter, exp_denote_sz, outarg); // Could make this an if sign_size != 0
        output_padding(padding_size, ' ', outRoutine, outarg); /* pad the field */
        }
    else
        {
        output_padding(padding_size, ' ', outRoutine, outarg); /* pad the field */

        outRoutine(sign, sign_size, outarg); // Could make this an if sign_size != 0
        outRoutine(start, num_size, outarg); // Could make this an if sign_size != 0
        if (!(cell_iter > dec_point + values->prec + 1)) /* Complete the number if for large precision*/
            {
            output_padding(dec_point + values->prec + 1 - cell_iter, '0', outRoutine, outarg);
            }

        outRoutine(exp_out_iter, exp_denote_sz, outarg); // Could make this an if sign_size != 0
        }

    *nchars += overall_size;
#endif
    }
