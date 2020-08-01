#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "../fioFormatHeader.h"
#include "stdio.h"

#define BASE 16
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

    outBuf_iter = num_to_string
        (
        outBuf_iter,
        val,
        BASE,
        conversion_case_up
        );

    /* FIXME Try restricting the flags */
    values->flags = FIO_PREC_BIT;
    values->prec = 16;

    size_t num_size = (outBuf + sizeof(outBuf)) - outBuf_iter;

    output_number(0,
                outBuf_iter,
                num_size,
                alt,
                alt_sz,
                values,
                outRoutine,
                outarg,
                nchars);
    }
