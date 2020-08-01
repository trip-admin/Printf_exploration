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

void handle_float_output_1
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
        
        /* This might be useless because the 0.0f that gets passed in becomes not 0 */
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

        /* Transfer the constructed value to the buffer*/
        int iter = acc.next - 1;
        while(iter > acc.dec_point)
            {
            put_cell_full(acc.val[iter], cell_iter);
            cell_iter += CELL_SIZE;
            --iter;
            }

        // Assume that there is a decimal point always. This is only false
        // when the precision is 0 and alt form is not specified
        dec_point = cell_iter;
        *cell_iter = '.';
        cell_iter += 1;
        while(iter > acc.dec_next)
            {
            put_cell_full(acc.val[iter], cell_iter);
            cell_iter += CELL_SIZE;
            --iter;
            }
        
        // Skip leading 0s
        for(int i = 0; i < CELL_SIZE - 1; ++i)
            {
            if(*start == '0')
                ++start;
            else
                break;
            }
        num_size = dec_point + values->prec + 1 - start; // first case is generally false
        dec_point[values->prec + 1] = '\0';
        // Handle alt
        if((values->flags & FIO_FLAG_ALT_BIT) == 0 && (values->prec == 0))
            {
            num_size = dec_point - start;
            *dec_point = '\0';
            }
        // NOTE: NEED rounding
        else if(cell_iter > dec_point + values->prec + 1)
            {
            dec_point[values->prec + 1] = '\0';
            }
        else
            {
            cell_iter[0] = '\0';
            }

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

    size_t overall_size = num_size + lead_zero_size + sign_size + padding_size;

    if((values->flags & FIO_FLAG_LJ_BIT) != 0)
        {
        outRoutine(sign, sign_size, outarg); // Could make this an if sign_size != 0
        outRoutine(start, num_size, outarg); // Could make this an if sign_size != 0
        if (!(cell_iter > dec_point + values->prec + 1)) /* Complete the number if for large precision*/
            {
            output_padding(dec_point + values->prec + 1 - cell_iter, '0', outRoutine, outarg);
            }

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
        }

    *nchars += overall_size;
#endif
    }
