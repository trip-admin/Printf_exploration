/*
Copyright 2020 <COPYRIGHT HOLDER>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

typedef int status_t;
typedef status_t (*FIOFORMATV_OUTPUT_FUNCPTR) (const char *buf,
                size_t nchars,
                size_t outarg);

/* C standard fprintf section 6 flags */

#define FIO_FLAG_LEFT_JUSTIFIED '-'
#define FIO_FLAG_INCLUDE_SIGN '+'
#define FIO_FLAG_INCLUDE_SPACE_FOR_SIGN ' '
#define FIO_FLAG_ALT_FORM '#'
#define FIO_FLAG_PAD_ZEROES '0'

/* C standard fprintf section 6 flag positions */
#define FIO_FLAG_LJ_BIT  0x1
#define FIO_FLAG_IS_BIT  0x2
#define FIO_FLAG_ISS_BIT 0x4
#define FIO_FLAG_ALT_BIT 0x8
#define FIO_FLAG_PZ_BIT 0x10
#define FIO_FIELD_WIDTH_BIT 0x20
#define FIO_PREC_BIT 0x40

/* C standard length modifiers */
#define FIO_LEN_MOD_CHAR 'h' /* Combined with FIO_LEN_MOD_SHRT */
#define FIO_LEN_MOD_SHRT 'h'
#define FIO_LEN_MOD_LONG 'l'
#define FIO_LEN_MOD_LLONG 'l' /* Combined with FIO_LEN_MOD_LONG */
#define FIO_LEN_MOD_MAX_T 'j'
#define FIO_LEN_MOD_SIZE_T 'z'
#define FIO_LEN_MOD_PTRDIFF_T 'T'
#define FIO_LEN_MOD_LONG_DOUBLE 'L'

#define FIELD_WIDTH_FROM_ARG '*'
#define PREC_FROM_ARG '*'
#define HAS_SPECIFIC_PREC_CHAR '.'

/* Section 4 */
#define FIO_MAX_PRECISION INT_MAX
#define FIO_MAX_FIELD_WIDTH INT_MAX

enum FIO_LEN_MODS
    {
    FIO_LEN_CHAR,
    FIO_LEN_SHRT,
    FIO_LEN_INT,
    FIO_LEN_LONG,
    FIO_LEN_LLONG,
    FIO_LEN_MAX_T,
    FIO_LEN_SIZE_T,
    FIO_LEN_PTRDIFF_T,
    FIO_LEN_LONG_DOUBLE
    };

typedef struct {
    size_t nchars;
    size_t flags; /* Maybe change this to a unsigned or even a type with constant size as this is a bitmap */
    size_t field_width;
    size_t prec;
    enum FIO_LEN_MODS length;

} FIO_SHARED_VALUES;
typedef long long intmax_t;
char *conversion_case_lo;
char *conversion_case_up;

void getFioFormatFlags
(
    FIO_SHARED_VALUES *values,
    const char **fmt            /* format string */
);

void getFioFieldWidth
(
    FIO_SHARED_VALUES *values,
    const char **fmt,            /* format string */
    va_list     vaList         /* pointer to varargs list */
);

void getFioPrecision
(
    FIO_SHARED_VALUES *values,
    const char **fmt,            /* format string */
    va_list     vaList         /* pointer to varargs list */
);

void getFioLengthMod
(
    FIO_SHARED_VALUES *values,
    const char **fmt            /* format string */
);

void handle_int_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);
void handle_unsigned_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);

void handle_octal_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);

void handle_hex_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);

void handle_HEX_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);

void handle_char_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);       
void handle_string_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);
void handle_pointer_output
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    FIOFORMATV_OUTPUT_FUNCPTR outRoutine,/* handler for args as they're formatted */
    size_t outarg,               /* argument to routine */
    size_t *nchars
);
void handle_write_chars_written
(
    FIO_SHARED_VALUES *values,
    va_list     vaList,         /* pointer to varargs list */
    size_t *nchars
);

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
);
char * num_to_string (
    char *iter,
    unsigned long long val,
    unsigned base,
    char *conversion
    );

long long get_sarg
(
    FIO_SHARED_VALUES *values,
    va_list     vaList         /* pointer to varargs list */
);

long long get_uarg
(
    FIO_SHARED_VALUES *values,
    va_list     vaList         /* pointer to varargs list */
);
