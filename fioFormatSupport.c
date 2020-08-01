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



/* DFA for conversion flags
 * Conforms to C standard. Any amount of flags allowed in any order
 * Repeat flags do not cause an error
 */

void getFioFormatFlags
(
    FIO_SHARED_VALUES *values,
    const char **fmt            /* format string */
)
    {
    while(**fmt != '\0')
        {
        switch (**fmt)
            {
            case FIO_FLAG_LEFT_JUSTIFIED:
                values->flags |= FIO_FLAG_LJ_BIT;
                break;
            case FIO_FLAG_INCLUDE_SIGN:
                values->flags |= FIO_FLAG_IS_BIT;
                break;
            case FIO_FLAG_INCLUDE_SPACE_FOR_SIGN:
                values->flags |= FIO_FLAG_ISS_BIT;
                break;
            case FIO_FLAG_ALT_FORM:
                values->flags |= FIO_FLAG_ALT_BIT;
                break;
            case FIO_FLAG_PAD_ZEROES:
                values->flags |= FIO_FLAG_PZ_BIT;
                break;
            default:
                /* If both FIO_FLAG_INCLUDE_SIGN and
                 * FIO_FLAG_INCLUDE_SPACE_FOR_SIGN are given
                 * FIO_FLAG_INCLUDE_SPACE_FOR_SIGN is ignored
                 * (section 6 space flag)
                 */
                if (values->flags & FIO_FLAG_IS_BIT)
                    {
                    values->flags &= ~FIO_FLAG_ISS_BIT;
                    }
                return;
            }
        ++(*fmt);
        }

    return;
    }

/* Local copy of atoi... Thats a very bad name
 * Consumes all numeric chars to construct and
 * integer. Does not specify a size limit
 */
size_t local_atoi(const char **fmt)
    {
    size_t val = 0ULL;
    while(isdigit(**fmt))
        {
        val = val * 10 + (**fmt - '0');
        ++(*fmt);
        }

    return val;
    }

/* DFA for field width
 * The field width is specified by either a * and a argument or a
 * non negative decimal integer
 */
void getFioFieldWidth
(
    FIO_SHARED_VALUES *values,
    const char **fmt,            /* format string */
    va_list     vaList         /* pointer to varargs list */
)
    {
    if(**fmt == FIELD_WIDTH_FROM_ARG)
        {
        values->flags |= FIO_FIELD_WIDTH_BIT;
        int width = va_arg(vaList, int);
        if(width < 0)
            {
            values->flags |= FIO_FLAG_LJ_BIT;
            values->field_width = (size_t)(-width);
            }
        else
            {
            values->field_width = (size_t)(width);
            }
        ++(*fmt); /* Consume the FIELD_WIDTH_FROM_ARG flag */
        }
    else /* The fieldwidth is given as a non negative decimal integer */
        {
        const char *orig_fmt = *fmt;
        values->field_width = local_atoi(fmt);
        if(orig_fmt != *fmt)
            {
            values->flags |= FIO_FIELD_WIDTH_BIT;
            }
        }
    }

/* DFA for precision
 * The field width is specified by either a * and a argument or a
 * non negative decimal integer
 */
void getFioPrecision
(
    FIO_SHARED_VALUES *values,
    const char **fmt,            /* format string */
    va_list     vaList         /* pointer to varargs list */
)
    {
    if(**fmt != HAS_SPECIFIC_PREC_CHAR)
        {
        return;
        }
    ++(*fmt); /* Skip HAS_SPECIFIC_PREC_CHAR now that its served its purpose */

    values->flags |= FIO_PREC_BIT;

    /* If both FIO_FLAG_PAD_ZEROES and
     * a precision are given
     * FIO_FLAG_PAD_ZEROES is ignored
     * (section 6 0 flag)
     */
    values->flags &= ~FIO_FLAG_PZ_BIT;
 
    if(**fmt == PREC_FROM_ARG)
        {
        int width = va_arg(vaList, int);
        if(width < 0)
            {
            values->flags &= ~FIO_PREC_BIT;
            }
        else
            {
            values->prec = (size_t)(width);
            }
        ++(*fmt); /* Consume the PREC_FROM_ARG flag */
        }
    else /* The precision is given as a non negative decimal integer */
        {

        /* Note that local_atoi() correctly provides 0 if no value is given */

        values->prec = local_atoi(fmt);
        }
    }


/* DFA for length modifiers
 * Conforms to C standard.
 */
void getFioLengthMod
(
    FIO_SHARED_VALUES *values,
    const char **fmt            /* format string */
)
    {
    values->length = FIO_LEN_INT;

    if(**fmt == '\0') return;

    /* Now guaranteed to have 1 more char.
     * This switch will consume up to 2 characters to determine
     * length modifier
     */

    switch (**fmt)
        {
        case FIO_LEN_MOD_SHRT:
            if((*fmt)[1] == FIO_LEN_MOD_CHAR)
                {
                values->length = FIO_LEN_CHAR;
                ++(*fmt);
                }
            else
                {
                values->length = FIO_LEN_SHRT;
                }
            break;
        case FIO_LEN_MOD_LONG:
            if((*fmt)[1] == FIO_LEN_MOD_LLONG)
                {
                values->length = FIO_LEN_LLONG;
                ++(*fmt);
                }
            else
                {
                values->length = FIO_LEN_LONG;
                }
            break;
        case FIO_LEN_MOD_MAX_T:
            values->length = FIO_LEN_MAX_T;
            break;
        case FIO_LEN_MOD_SIZE_T:
            values->length = FIO_LEN_SIZE_T;
            break;
        case FIO_LEN_MOD_PTRDIFF_T:
            values->length = FIO_LEN_PTRDIFF_T;
            break;
        case FIO_LEN_MOD_LONG_DOUBLE:
            values->length = FIO_LEN_LONG_DOUBLE;
            break;
        default:
            return;
        }

    ++(*fmt);

    return;
    }


