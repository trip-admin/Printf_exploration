#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <wchar.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

#define BASE 10
#define CHAR_OFFSET '0'

void handle_char_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
)
    {
    wint_t extd_char;
    unsigned char val;
    if (values->length == FIO_LEN_LONG)
        {
        wchar_t outBuf[2];
        outBuf[0] = (wchar_t) va_arg(vaList, wint_t);
        outBuf[1] = (wchar_t)'\0';
        outRoutine((char*)outBuf, sizeof(outBuf), outarg); /* Don't know what to do here */
        *nchars += sizeof(wchar_t);
        }
    else
        {
        unsigned char val = (unsigned char)va_arg(vaList, unsigned);
        outRoutine(&val, 1, outarg);
        *nchars += 1;
        }
    }
