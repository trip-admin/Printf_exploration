#include "fioFormatV.c"

#include <stdio.h>

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
#if 0
    nchars = wrapper(my_printf, 0, SANITY);
    printf("%lu %lu\n", nchars, sizeof(SANITY) - 1);
    assert(nchars == sizeof(SANITY) - 1);    
#endif
    /* %d testing, should be testing all requirements */

    nchars = wrapper(my_printf, 0, "Hi %X I\n" , 0xFFFF);
    assert(nchars == sizeof("Hi ffff I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %.5X I\n" , 0xFFFF);
    assert(nchars == sizeof("Hi 0ffff I\n") - 1); /* Does not include NUL char */

    /* alt form */
    nchars = wrapper(my_printf, 0, "Hi %#X I\n" , 0x717);
    assert(nchars == sizeof("Hi 0x717 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#X I\n", 0);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#.10X I\n", 0);
    assert(nchars == sizeof("Hi 0000000000 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#.10X I\n", 1);
    assert(nchars == sizeof("Hi 0x0000000001 I\n") - 1); /* Does not include NUL char */
    return 0;
    }
