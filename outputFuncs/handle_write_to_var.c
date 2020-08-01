#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <wchar.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

typedef long long intmax_t;

void handle_write_chars_written
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    size_t *nchars
)
{
    /* Yay spec contradicts itself for length modifiers Yay */
    switch(values->length)
        {
        case FIO_LEN_CHAR:
            *va_arg(vaList, char *) = (char)*nchars;
            break;
        case FIO_LEN_SHRT:
            *va_arg(vaList, short *) = (short)*nchars;
            break;
        case FIO_LEN_INT:
            *va_arg(vaList, int *) = (int)*nchars;
            break;
        case FIO_LEN_LONG:
            *va_arg(vaList, long *) = (long)*nchars;
            break;
        case FIO_LEN_LLONG:
            *va_arg(vaList, long long *) = (long long)*nchars;
            break;
        case FIO_LEN_MAX_T:
            *va_arg(vaList, intmax_t *) = (intmax_t)*nchars;
            break;
        case FIO_LEN_SIZE_T:
            *va_arg(vaList, size_t*) = (size_t)*nchars;
            break;
        case FIO_LEN_PTRDIFF_T:
            *va_arg(vaList, ptrdiff_t*) = (ptrdiff_t)*nchars;
            break;
        default:
            *va_arg(vaList, int *) = (int)*nchars;
            break;
        }
}
