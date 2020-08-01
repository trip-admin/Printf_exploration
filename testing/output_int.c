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
    nchars = wrapper(my_printf, 0, "Hi %d I\n", 123);
    assert(nchars == sizeof("Hi 123 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %d I\n", -123);
    assert(nchars == sizeof("Hi -123 I\n") - 1); /* Does not include NUL char */

    /* basic flags testing  +, ,*/
    nchars = wrapper(my_printf, 0, "Hi %+d I\n", 123);
    assert(nchars == sizeof("Hi +123 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi % d I\n", 123);
    assert(nchars == sizeof("Hi  123 I\n") - 1); /* Does not include NUL char */

    /* Padding */
    nchars = wrapper(my_printf, 0, "Hi %10d I\n", 123);
    assert(nchars == sizeof("Hi        123 I\n") - 1); /* Does not include NUL char */

    /* Precision */
    nchars = wrapper(my_printf, 0, "Hi %.10d I\n", 123);
    assert(nchars == sizeof("Hi 0000000123 I\n") - 1); /* Does not include NUL char */

    /* precision and padding */
    nchars = wrapper(my_printf, 0, "Hi %20.10d I\n", 123);
    assert(nchars == sizeof("Hi           0000000123 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %20.10d I\n", -123);
    assert(nchars == sizeof("Hi          -0000000123 I\n") - 1); /* Does not include NUL char */

    /* zero flag */
    nchars = wrapper(my_printf, 0, "Hi %010d I\n", 123);
    assert(nchars == sizeof("Hi 0000000123 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %010d I\n", -123);
    assert(nchars == sizeof("Hi -000000123 I\n") - 1); /* Does not include NUL char */

    /* right justify */
    nchars = wrapper(my_printf, 0, "Hi %-10d I\n", 123);
    assert(nchars == sizeof("Hi 123        I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %-10d I\n", -123);
    assert(nchars == sizeof("Hi -123       I\n") - 1); /* Does not include NUL char */

    /* length mod testing */
    nchars = wrapper(my_printf, 0, "Hi %hhd I\n", 255);
    assert(nchars == sizeof("Hi -1 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %hhd I\n", 256);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %hd I\n", 65535);
    assert(nchars == sizeof("Hi -1 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %hd I\n", 65536);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %ld I\n", -1ll);
    assert(nchars == sizeof("Hi -1 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %ld I\n", 18446744073709551616ull);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %lld I\n", -1ll);
    assert(nchars == sizeof("Hi -1 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %lld I\n", 18446744073709551616ull);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */
 
    /* ignore space when + is spec */
    nchars = wrapper(my_printf, 0, "Hi %+ d I\n", 123);
    assert(nchars == sizeof("Hi +123 I\n") - 1); /* Does not include NUL char */
    /* ignore 0 flag if prec specified */
    nchars = wrapper(my_printf, 0, "Hi %010.5d I\n", 123);
    assert(nchars == sizeof("Hi      00123 I\n") - 1); /* Does not include NUL char */

    /* asterisks */
    nchars = wrapper(my_printf, 0, "Hi %*.*d I\n", 10, 5, 123);
    assert(nchars == sizeof("Hi      00123 I\n") - 1); /* Does not include NUL char */

    /* more stuff */
    nchars = wrapper(my_printf, 0, "Hi %d I %d\n", 123, -123);
    assert(nchars == sizeof("Hi 123 I -123\n") - 1); /* Does not include NUL char */

    /* ignore prec when negative */
    nchars = wrapper(my_printf, 0, "Hi %*.*d I\n", 10, -5, 123);
    assert(nchars == sizeof("Hi        123 I\n") - 1); /* Does not include NUL char */
    /* left justify if field is negative*/
    nchars = wrapper(my_printf, 0, "Hi %*.*d I\n", -10, 5, 123);
    assert(nchars == sizeof("Hi 00123      I\n") - 1); /* Does not include NUL char */

    /* Limit */
    nchars = wrapper(my_printf, 0, "Hi %lld I\n", 9223372036854775807LL);
    assert(nchars == sizeof("Hi 9223372036854775807 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %lld I\n", -9223372036854775808LL);
    assert(nchars == sizeof("Hi -9223372036854775808 I\n") - 1); /* Does not include NUL char */
#endif
    return 0;
    }
