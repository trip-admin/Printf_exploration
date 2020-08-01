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

    nchars = wrapper(my_printf, 0, "Hi %p I\n" , "asdf");
    assert(nchars == sizeof("Hi 0x0000000000000000 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %.1p I\n" , NULL);
    assert(nchars == sizeof("Hi 0x0000000000000000 I\n") - 1); /* Does not include NUL char */


    return 0;
    }
