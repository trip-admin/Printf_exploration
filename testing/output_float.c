#include "fioFormatV.c"

#include <stdio.h>
#include <wchar.h>
#include <math.h>

status_t my_printf (const char *buf,
                size_t nchars,
                size_t outarg)
    {
    printf("%.*s", (int)nchars, buf);
    return 0;
    }

#define SANITY "this is a string\n"
#define SANITY_2 "this is a string\n%d aaaaaaaaa\n"
#include <stdlib.h>
#include <assert.h>
int main()
    {
    size_t nchars;
    int a[10];

#if 1
    nchars = wrapper(my_printf, 0, "%.20f\n" , 1.0f);
    // 0 is special in my thing, need to make it a special case
    nchars = wrapper(my_printf, 0, "%f\n" , 111111.f);
    nchars = wrapper(my_printf, 0, "%f\n" , 0.25f);
    nchars = wrapper(my_printf, 0, "%f\n" , 1.25f);
    nchars = wrapper(my_printf, 0, "%f\n" , 1111111111111111.0f);
    nchars = wrapper(my_printf, 0, "%.20f\n" , 1.0f / 1024);
    nchars = wrapper(my_printf, 0, "%.0f\n" , 11111111111.11111f);
    nchars = wrapper(my_printf, 0, "%#.0f\n" , 11111111111.11111f);
#endif
    nchars = wrapper(my_printf, 0, "%f\n" , INFINITY);
    nchars = wrapper(my_printf, 0, "%f\n" , NAN);
    nchars = wrapper(my_printf, 0, "%F\n" , INFINITY);
    nchars = wrapper(my_printf, 0, "%F\n" , NAN);
    return 0;
    }
