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

#if 0
    nchars = wrapper(my_printf, 0, "%a\n" , 0.25f);
    nchars = wrapper(my_printf, 0, "%a\n" , 1.0f);
    nchars = wrapper(my_printf, 0, "%a\n" , 100000000000.0f);
    nchars = wrapper(my_printf, 0, "%a\n" , 32.0f);
    nchars = wrapper(my_printf, 0, "%a\n" , 31.0f);
    nchars = wrapper(my_printf, 0, "%a\n" , 0.25f);
    nchars = wrapper(my_printf, 0, "%a\n" , 1.0f/ (1ul << 34));
    nchars = wrapper(my_printf, 0, "%a\n" , 1.0e101);
    nchars = wrapper(my_printf, 0, "%a\n" , 1.0e-101);
    nchars = wrapper(my_printf, 0, "%a\n" , INFINITY);
    nchars = wrapper(my_printf, 0, "%a\n" , NAN);
    nchars = wrapper(my_printf, 0, "%.0a\n" , 1.0f);
    nchars = wrapper(my_printf, 0, "%#.0a\n" , 1.0f);
    nchars = wrapper(my_printf, 0, "%.20a\n" , 1.0f);
    nchars = wrapper(my_printf, 0, "%.20e\n" , 1.0f);
#endif


#if 0
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
#if 0
    nchars = wrapper(my_printf, 0, "%f\n" , INFINITY);
    nchars = wrapper(my_printf, 0, "%f\n" , NAN);
    nchars = wrapper(my_printf, 0, "%F\n" , INFINITY);
    nchars = wrapper(my_printf, 0, "%F\n" , NAN);
#endif
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
    nchars = wrapper(my_printf, 0, "Hi %*.*o I\n", -10, 5, 0717);
    assert(nchars == sizeof("Hi 00717      I\n") - 1); /* Does not include NUL char */
#endif

    nchars = wrapper(my_printf, 0, "Hi %x I\n" , 0xFFFF);
    assert(nchars == sizeof("Hi ffff I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %.5x I\n" , 0xFFFF);
    assert(nchars == sizeof("Hi 0ffff I\n") - 1); /* Does not include NUL char */

    /* alt form */
    nchars = wrapper(my_printf, 0, "Hi %#x I\n" , 0x717);
    assert(nchars == sizeof("Hi 0x717 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#x I\n", 0);
    assert(nchars == sizeof("Hi 0 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#.10x I\n", 0);
    assert(nchars == sizeof("Hi 0000000000 I\n") - 1); /* Does not include NUL char */
    nchars = wrapper(my_printf, 0, "Hi %#.10x I\n", 1);
    assert(nchars == sizeof("Hi 0x0000000001 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %p I\n" , "asdf");
    assert(nchars == sizeof("Hi 0x0000000000000000 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %.1p I\n" , NULL);
    assert(nchars == sizeof("Hi 0x0000000000000000 I\n") - 1); /* Does not include NUL char */

nchars = wrapper(my_printf, 0, "Hi %lu I\n", 18446744073709551615ull);
    assert(nchars == sizeof("Hi 18446744073709551615 I\n") - 1); /* Does not include NUL char */

    nchars = wrapper(my_printf, 0, "Hi %llu I\n", 18446744073709551615ull);
    assert(nchars == sizeof("Hi 18446744073709551615 I\n") - 1); /* Does not include NUL char */
 

    return 0;
    }
