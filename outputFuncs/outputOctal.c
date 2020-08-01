#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

#define BASE 8

/* need to track this down */
typedef long long intmax_t;

static long long get_arg
(
    FIO_SHARED_VALUES *values,
    va_list     vaList         /* pointer to varargs list */
)
    {
    switch(values->length)
        {
        case FIO_LEN_CHAR:
            return (unsigned long long)(unsigned char)va_arg(vaList, unsigned);
        case FIO_LEN_SHRT:
            return (unsigned long long)(unsigned short)va_arg(vaList, unsigned);
        case FIO_LEN_INT:
            return (unsigned long long)va_arg(vaList, unsigned);
        case FIO_LEN_LONG:
            return (unsigned long long)va_arg(vaList, unsigned long);
        case FIO_LEN_LLONG:
            return va_arg(vaList, unsigned long long);
        case FIO_LEN_MAX_T:
            return (unsigned long long) va_arg(vaList, intmax_t);
        case FIO_LEN_SIZE_T:
            return (unsigned long long) va_arg(vaList, size_t);
        case FIO_LEN_PTRDIFF_T:
            return (unsigned long long) va_arg(vaList, ptrdiff_t);
        //case FIO_LEN_LONG_DOUBLE:
        default:
            return (unsigned long long)va_arg(vaList, unsigned int);
        }
    }

static char conversion [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

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
void handle_octal_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
)
    {
    char outBuf[1000];
    char *outBuf_iter = outBuf + sizeof(outBuf) - 1; /* Avoid stack smashing */
    unsigned long long val = get_arg(values, vaList);

    if (val == 0LL)
        {
        *outBuf_iter = '0';
        }
    else
        {
        while (val > 0ULL)
            {
            *outBuf_iter = conversion[val % BASE];
            --outBuf_iter;
            val /= BASE;
            }

            /* Put the iterator back to a valid char */
            ++outBuf_iter;
        }

    /* Handle alternative form */


    /* Handle precision. Include a prefix '0' if needed to signify its an octal value */
    if((values->flags & FIO_FLAG_ALT_BIT) != 0)
        {
        if (*outBuf_iter != '0')
            {
            --outBuf_iter;
            *outBuf_iter = '0';
            }
        }

    size_t num_size = (outBuf + sizeof(outBuf)) - outBuf_iter;

    size_t lead_zero_size = 0;
    if((values->flags & FIO_PREC_BIT) != 0)
        {
        if(num_size < values->prec)
            {
            lead_zero_size = values->prec - num_size;
            }
        }

    /* Handle sign prepend */

    size_t sign_size = 1;
    char * sign = "";
    if (val < 0)
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

    /* At this point we should have the full number
     */
#if 0
    printf("%s\n", outBuf_iter);
    printf("%lu\n", sz);
#endif
    if((values->flags & FIO_FLAG_LJ_BIT) != 0)
        {
        outRoutine(sign, sign_size, outarg); // Could make this an if sign_size != 0
        output_padding(lead_zero_size, '0', outRoutine, outarg); /* pad the precision */
        outRoutine(outBuf_iter, num_size, outarg); // Could make this an if sign_size != 0

        output_padding(padding_size, ' ', outRoutine, outarg); /* pad the field */
        }
    else
        {
        output_padding(padding_size, ' ', outRoutine, outarg); /* pad the field */

        outRoutine(sign, sign_size, outarg); // Could make this an if sign_size != 0
        output_padding(lead_zero_size, '0', outRoutine, outarg); /* pad the precision */
        outRoutine(outBuf_iter, num_size, outarg); // Could make this an if sign_size != 0
        }

    //outRoutine(outBuf_iter, overall_size, outarg);
    *nchars += overall_size;
    }
