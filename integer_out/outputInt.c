#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

#define BASE 10
#define CHAR_OFFSET '0'

/* need to track this down */
#define LONG_LONG_MIN_AS_POS (1ULL << ((sizeof(long long) * 8) - 1))
#define LONG_LONG_NEG_MASK (LONG_LONG_MIN_AS_POS - 1)
void handle_int_output
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
    long long val = get_sarg(values, vaList);

    /* Get the absolute value */
    unsigned long long working;
    if (val < 0) // Assume 2's complement
        {
        if ((val & LONG_LONG_NEG_MASK) == 0) /* with 2's compl, the Abs Min val is when only the "sign" is set */
            {
            working = LONG_LONG_MIN_AS_POS;
            }
        else
            {
            working = (unsigned long long) -val;
            }
        }
    else
        {
        working = (unsigned long long) val;
        }

    outBuf_iter = num_to_string
        (
        outBuf_iter,
        working,
        BASE,
        conversion_case_lo
        );

    /* Handle precision */

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
