#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

#define BASE 16

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

static char conversion [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

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
void handle_pointer_output
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
    char *alt = "";
    size_t alt_sz = 0;
    unsigned long long val = (unsigned long long)va_arg(vaList, void *);

    /* Assume alternate form for hex*/
    alt = "0x";
    alt_sz = 2;

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

    size_t num_size = (outBuf + sizeof(outBuf)) - outBuf_iter;

    size_t lead_zero_size = 0;
    if(num_size < sizeof(void *) * 2)
        {
        lead_zero_size = (sizeof(unsigned long long) * 2) - num_size;
        }

    size_t overall_size = num_size + lead_zero_size + alt_sz;

    /* At this point we should have the full number
     */
#if 0
    printf("%s\n", outBuf_iter);
    printf("%lu\n", sz);
#endif
    outRoutine(alt, alt_sz, outarg); // Could make this an if sign_size != 0
    output_padding(lead_zero_size, '0', outRoutine, outarg); /* pad the precision */
    outRoutine(outBuf_iter, num_size, outarg); // Could make this an if sign_size != 0

    //outRoutine(outBuf_iter, overall_size, outarg);
    *nchars += overall_size;
    }
