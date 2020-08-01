#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "../fioFormatHeader.h"
#include "stdio.h"


//char conversion_case_lo [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
//char conversion_case_up [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char *conversion_case_lo  = "0123456789abcdef";
char *conversion_case_up  = "0123456789ABCDEF";
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

long long get_sarg
(
    FIO_SHARED_VALUES *values,
    va_list     vaList         /* pointer to varargs list */
)
    {
    switch(values->length)
        {
        case FIO_LEN_CHAR:
            return (long long)(char)va_arg(vaList, int);
        case FIO_LEN_SHRT:
            return (long long)(short)va_arg(vaList, int);
        case FIO_LEN_INT:
            return (long long)va_arg(vaList, int);
        case FIO_LEN_LONG:
            return (long long)va_arg(vaList, long);
        case FIO_LEN_LLONG:
            return va_arg(vaList, long long);
        case FIO_LEN_MAX_T:
            return (long long) va_arg(vaList, intmax_t);
        case FIO_LEN_SIZE_T:
            return (long long) va_arg(vaList, size_t);
        case FIO_LEN_PTRDIFF_T:
            return (long long) va_arg(vaList, ptrdiff_t);
        //case FIO_LEN_LONG_DOUBLE:
        default:
            return (long long)va_arg(vaList, int);
        }
    }

long long get_uarg
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

char * num_to_string (
    char *iter,
    unsigned long long val,
    unsigned base,
    char *conversion
    )
    {
     if (val == 0ULL)
        {
        *iter = '0';
        }
    else
        {
        while (val > 0ULL)
            {
            *iter = conversion[val % base];
            --iter;
            val /= base;
            }

        /* Put the iterator back to a valid char */
        ++iter;
        }
    return iter;
    }



void output_number
(
    int has_sign,
    char * num_begin,
    int   num_size,
    char * alt_str,
    int   alt_sz,
    FIO_SHARED_VALUES *values,
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
)
    {

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
    if (has_sign != 0)
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
                lead_zero_size = values->field_width - (num_size + sign_size + alt_sz);
                }
            else
                {
                padding_size = values->field_width - (num_size + lead_zero_size + sign_size + alt_sz);
                }

            }
        }

    size_t overall_size = num_size + lead_zero_size + sign_size + padding_size + alt_sz;

    /* At this point we should have the full number
     */
#if 0
    printf("%s\n", outBuf_iter);
    printf("%lu\n", sz);
#endif
    if((values->flags & FIO_FLAG_LJ_BIT) != 0)
        {
        outRoutine(sign, sign_size, outarg); // Could make this an if sign_size != 0
        outRoutine(alt_str, alt_sz, outarg); // Could make this an if sign_size != 0
        output_padding(lead_zero_size, '0', outRoutine, outarg); /* pad the precision */
        outRoutine(num_begin, num_size, outarg); // Could make this an if sign_size != 0

        output_padding(padding_size, ' ', outRoutine, outarg); /* pad the field */
        }
    else
        {
        output_padding(padding_size, ' ', outRoutine, outarg); /* pad the field */

        outRoutine(sign, sign_size, outarg); // Could make this an if sign_size != 0
        outRoutine(alt_str, alt_sz, outarg); // Could make this an if sign_size != 0
        output_padding(lead_zero_size, '0', outRoutine, outarg); /* pad the precision */
        outRoutine(num_begin, num_size, outarg); // Could make this an if sign_size != 0
        }

    //outRoutine(outBuf_iter, overall_size, outarg);
    *nchars += overall_size;
    }
