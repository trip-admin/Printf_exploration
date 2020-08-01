/*
Copyright 2020 <COPYRIGHT HOLDER>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#include "fioFormatHeader.h"
#include <stdio.h>




/* Handles a conversion (a '%' character conversion) */
size_t formatHandler
(
    const char **fmt,            /* format string */
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
)
{
    const char * save = *fmt;
    ++(*fmt); /* On entry assume that fmt is at a '%' char */
    FIO_SHARED_VALUES values = {0};

    /* Get all information */
    getFioFormatFlags(&values, fmt);
    getFioFieldWidth(&values, fmt, vaList);
    getFioPrecision(&values, fmt, vaList);
    getFioLengthMod(&values, fmt);

    /* Get the conversion, assume its on the char which specifies the conversion */

    switch(**fmt)
        {
        case 'd':
        case 'i':
            handle_int_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'u':
            handle_unsigned_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'o':
            handle_octal_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'x':
            handle_hex_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'X':
            handle_HEX_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'c':
            handle_char_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 's':
            handle_string_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'p':
            handle_pointer_output(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'n':
            handle_write_chars_written(&values, vaList, nchars);
            break;
        case 'f':
            if (!(values.flags & FIO_PREC_BIT))
                {
                // Value by default is 6 if its unset
                values.prec = 6;
                }
            handle_float_output_1(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'F':
            if (!(values.flags & FIO_PREC_BIT))
                {
                // Value by default is 6 if its unset
                values.prec = 6;
                }
            handle_float_output_F(&values, vaList, outRoutine, outarg, nchars);
            break;
         case 'e':
            if (!(values.flags & FIO_PREC_BIT))
                {
                // Value by default is 6 if its unset
                values.prec = 6;
                }
                handle_float_output_e(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'E':
            if (!(values.flags & FIO_PREC_BIT))
                {
                // Value by default is 6 if its unset
                values.prec = 6;
                }
                handle_float_output_E(&values, vaList, outRoutine, outarg, nchars);
            break;
        case 'a':
            if (!(values.flags & FIO_PREC_BIT))
                {
                // Value by default is 6 if its unset
                values.prec = 6;
                }
                handle_float_output_a(&values, vaList, outRoutine, outarg, nchars);
            break;

        case '%':
            outRoutine("%", 1, outarg);
            break;
        default:
            outRoutine(save, *fmt - save + 1, outarg);
            break;
        }

    /* Conversion is done */
    ++(*fmt);

}

size_t fioFormatV
(
    const char *fmt,            /* format string */
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg               /* argument to routine */
)
{
    size_t nchars = 0;
    size_t max_subseq_no_format = 0;
    while(*fmt != '\0')
        {
        if(*fmt == '%')
            {
            outRoutine(fmt - max_subseq_no_format, max_subseq_no_format, outarg);
            nchars += max_subseq_no_format;
            max_subseq_no_format = 1; /* Not sure what to do here. I think I want to set it to 1 */
            // Do something
            size_t fio_nchars = formatHandler(&fmt, vaList, outRoutine, outarg, &nchars);
            //Get error if it exists
            //nchars += fio_nchars;
            }
        else
            {
            ++max_subseq_no_format;
            }
        ++fmt;
        }

    /* Output remaining characters */

    outRoutine(fmt - max_subseq_no_format, max_subseq_no_format, outarg);
    nchars += max_subseq_no_format;
    return nchars;
}

size_t wrapper(FIOFORMATV_OUTPUT_FUNCPTR outRoutine, size_t outarg, const char *fmt, ...){

    va_list test;
    va_start(test, fmt);
    size_t nchars = fioFormatV(fmt, test, outRoutine, outarg);
    va_end(test);
    return nchars;
}
