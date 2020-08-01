#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <wchar.h>
#include <string.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

#define BASE 10
#define CHAR_OFFSET '0'

void handle_string_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
)
    {
    const char *val = va_arg(vaList, const char *);

    size_t strSize = strlen(val);

    if ((values->flags & FIO_PREC_BIT) != 0)
        {
        if (strSize > values->prec)
            {
            strSize = values->prec;
            }
        }

    if (values->length == FIO_LEN_LONG)
        {
        /* Nothing for now */
        }
    else
        {
        outRoutine(val, strSize, outarg);
        *nchars += strSize;
        }
    }
