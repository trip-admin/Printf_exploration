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

    nchars = wrapper(my_printf, 0, "Hi %c I\n" , 'a');
    assert(nchars == sizeof("Hi a I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %c I\n" , 255);
    assert(nchars == sizeof("Hi   I\n") - 1); /* Does not include NUL char */

    /* I don't get it */
    /* Note: wchar in our case is 2 chars */
    nchars = wrapper(my_printf, 0, "Hi %lc I\n" , L'a');
//    printf("%lu\n", sizeof(wchar_t));
//    printf("%lu\n", nchars);
//    printf("%lu\n", sizeof("Hi aaaa I\n") - 1);
    assert(nchars == sizeof("Hi aaaa I\n") - 1); /* Does not include NUL char */

    return 0;
    }
