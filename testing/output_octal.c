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
#if 1
    nchars = wrapper(my_printf, 0, "Hi %o I\n", 0717);
    assert(nchars == sizeof("Hi 717 I\n") - 1); /* Does not include NUL char */

    /* basic flags testing  +, ,*/
    nchars = wrapper(my_printf, 0, "Hi %+o I\n", 0717);
    assert(nchars == sizeof("Hi +717 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi % o I\n", 0717);
    assert(nchars == sizeof("Hi  717 I\n") - 1); /* Does not include NUL char */

    /* Padding */
    nchars = wrapper(my_printf, 0, "Hi %10o I\n", 0717);
    assert(nchars == sizeof("Hi        717 I\n") - 1); /* Does not include NUL char */

    /* Precision */
    nchars = wrapper(my_printf, 0, "Hi %.10o I\n", 0717);
    assert(nchars == sizeof("Hi 0000000717 I\n") - 1); /* Does not include NUL char */

    /* precision and padding */
    nchars = wrapper(my_printf, 0, "Hi %20.10o I\n", 0717);
    assert(nchars == sizeof("Hi           0000000717 I\n") - 1); /* Does not include NUL char */

    /* zero flag */
    nchars = wrapper(my_printf, 0, "Hi %010o I\n", 0717);
    assert(nchars == sizeof("Hi 0000000717 I\n") - 1); /* Does not include NUL char */

    /* right justify */
    nchars = wrapper(my_printf, 0, "Hi %-10o I\n", 0717);
    assert(nchars == sizeof("Hi 717        I\n") - 1); /* Does not include NUL char */

    /* length mod testing */
    nchars = wrapper(my_printf, 0, "Hi %hho I\n", 255);
    assert(nchars == sizeof("Hi 377 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %hho I\n", 256);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %ho I\n", 65535);
    assert(nchars == sizeof("Hi 177777 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %ho I\n", 65536);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %lo I\n", 18446744073709551615ull);
    assert(nchars == sizeof("Hi 1777777777777777777777 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %llo I\n", 18446744073709551615ull);
    assert(nchars == sizeof("Hi 1777777777777777777777 I\n") - 1); /* Does not include NUL char */
 
    /* ignore space when + is spec */
    nchars = wrapper(my_printf, 0, "Hi %+ o I\n", 0717);
    assert(nchars == sizeof("Hi +717 I\n") - 1); /* Does not include NUL char */
    /* ignore 0 flag if prec specified */
    nchars = wrapper(my_printf, 0, "Hi %010.5o I\n", 0717);
    assert(nchars == sizeof("Hi      00717 I\n") - 1); /* Does not include NUL char */

    /* asterisks */
    nchars = wrapper(my_printf, 0, "Hi %*.*o I\n", 10, 5, 0717);
    assert(nchars == sizeof("Hi      00717 I\n") - 1); /* Does not include NUL char */

    /* more stuff */
    nchars = wrapper(my_printf, 0, "Hi %o I %o\n", 0717, 0717);
    assert(nchars == sizeof("Hi 717 I 717\n") - 1); /* Does not include NUL char */

    /* ignore prec when negative */
    nchars = wrapper(my_printf, 0, "Hi %*.*o I\n", 10, -5, 0717);
    assert(nchars == sizeof("Hi        717 I\n") - 1); /* Does not include NUL char */
    /* left justify if field is negative*/
    nchars = wrapper(my_printf, 0, "Hi %*.*o I\n", -10, 5, 0717);
    assert(nchars == sizeof("Hi 00717      I\n") - 1); /* Does not include NUL char */

    /* alt form */
    nchars = wrapper(my_printf, 0, "Hi %#o I\n", 0717);
    assert(nchars == sizeof("Hi 0717 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#o I\n", 0);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#llo I\n", 18446744073709551615ull);
    assert(nchars == sizeof("Hi 01777777777777777777777 I\n") - 1); /* Does not include NUL char */

#endif
    return 0;
    }
