#include "fioFormatV.c"

#include <stdio.h>
#include <wchar.h>

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

    nchars = wrapper(my_printf, 0, "%n12345%n6789%n\n" , a, a + 1, a+2);
    assert(a[0] == 0);
    assert(a[1] == 5);
    assert(a[2] == 9);
    assert(nchars == 10);

    nchars = wrapper(my_printf, 0, "%%\n" , a, a + 1, a+2);

    return 0;
    }
