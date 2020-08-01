/*
Copyright 2020 <COPYRIGHT HOLDER>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#define CELL_SIZE 9
#include <string.h>
#include <stdlib.h>
struct bigInt
    {
    int sign;
    unsigned long val[1024];
    /* indexes */
    int next; /* points at the next available whole part */
    int dec_point; /* points at first fractional part */
    int dec_next; /* points at next open fractional part */
    };

#if 0
void initBigInt(struct bigInt *val);
void printBigInt(struct bigInt *val); /* Have this guy include a decimal point somewhere */
void addBigInt(struct bigInt *val1, unsigned long add_val);
void combineBigInt(struct bigInt *val1, struct bigInt *val2);
void multBySingleDigit(struct bigInt *val, unsigned long mul);
void divBySingleDigit(struct bigInt *val, unsigned long div);
#endif
