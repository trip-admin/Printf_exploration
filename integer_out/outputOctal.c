#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

#define BASE 8
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
    unsigned long long val = get_uarg(values, vaList);

    outBuf_iter = num_to_string
        (
        outBuf_iter,
        val,
        BASE,
        conversion_case_lo
        );

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

    output_number(val < 0,
                outBuf_iter,
                num_size,
                "",
                0,
                values,
                outRoutine,
                outarg,
                nchars);
    }
