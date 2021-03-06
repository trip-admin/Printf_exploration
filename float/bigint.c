/*
Copyright 2020 <COPYRIGHT HOLDER>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdlib.h>
#include <string.h>
#include "bigint.h"
void initBigInt(struct bigInt *val)
    {
    memset(val->val, 0, sizeof(val->val));
    val->next = (sizeof(val->val)/sizeof(*(val->val)))/2 + 1;
    val->dec_point = (sizeof(val->val)/sizeof(*(val->val)))/2;
    val->dec_next = (sizeof(val->val)/sizeof(*(val->val)))/2;
    }

#include <stdio.h>
/* Call it good enough here */
void printBigInt(struct bigInt *val) /* Have this guy include a decimal point somewhere */
    {
    int iter = val->next - 1;
    printf("%lu", val->val[iter]);
    --iter;
#if 1
    while(iter > val->dec_point)
        {
        printf("%.9lu", val->val[iter]);
        --iter;
        }
#endif
    printf("."); /* use dec_point*/
#if 1
    while(iter > val->dec_next)
        {
        /* For now make this extremely lazy*/
        printf("%.9lu", val->val[iter]);
        --iter;
        }
#endif
    }

#define ULONG_MAX ((~0ul))
#define CUT 1000000000ul

void addBigInt(struct bigInt *val1, unsigned long add_val)
    {
    int iter = val1->dec_point + 1;
    while(iter != val1->next)
        {
        unsigned long to_add = add_val % CUT;
        val1->val[iter] += to_add;
        unsigned long carry = val1->val[iter] / CUT;
        val1->val[iter] = val1->val[iter] % CUT;
        add_val = (add_val / CUT) + carry;
        ++iter;
        }

    while (add_val > 0)
        {
        val1->val[val1->next] = add_val % CUT;
        ++(val1->next);
        add_val /= CUT;
        }
    }


/* Adds val2 to val1 */
void combineBigInt(struct bigInt *val1, struct bigInt *val2)
    {
    unsigned long carry = 0;
    int iter1 = val1->dec_next + 1;
    int iter2 = val2->dec_next + 1;
    
    
    /* get iter 2's stuff added on */
    while(iter2 != (val2->next))
        {
        unsigned long to_set = val1->val[iter2] + val2->val[iter2] + carry;
        val1->val[iter2] = to_set % CUT;
        carry = to_set / CUT;
        ++iter2;
        }
    while (carry != 0)
        {
        unsigned long to_set = val1->val[iter1] + carry;
        val1->val[iter2] = to_set % CUT;
        carry = to_set / CUT;
        ++iter2;
        }
    if (iter2 > val1->next)
        {
        val1->next = iter2;
        }
    if (val2->dec_next < val1->dec_next)
        {
        val1->dec_next = val2->dec_next;
        }
    }
#include <assert.h>
void multBySingleDigit(struct bigInt *val, unsigned long mul)
    {
    if (mul > 100)
        {
        assert(0);
        }
    int iter = val->dec_next + 1;
    unsigned long carry = 0;
    while(iter != val->next)
        {
        unsigned long to_set = (val->val[iter] * mul) + carry;
        val->val[iter] = to_set % CUT;
        carry = to_set / CUT;
        ++iter;
        }

    if (carry > 0)
        {
        val->val[iter] = carry;
        ++(val->next);
        }
    }
void divBySingleDigit(struct bigInt *val, unsigned long div)
    {
    if (div > 100)
        {
        assert(0);
        }
    int iter = val->next - 1;
    unsigned long carry = 0;
    while(iter != val->dec_next)
        {
        unsigned long tmp = (val->val[iter] + carry) % div;
        unsigned long to_set = (val->val[iter] + carry) / div;
        val->val[iter] = to_set;
        carry = tmp * CUT;
        --iter;
        }

    /* Needs to loop */
    while (iter != 0 && carry > 0)
        {
        unsigned long tmp = carry % div;
        val->val[iter] = carry / div;
        carry = tmp * CUT;
        --iter;
        }
    if (iter < val->dec_next)
        {
        val->dec_next = iter;
        }
    }
