#include "bigint.c"

int main()
    {
    struct bigInt ex;
    struct bigInt ex2;
    initBigInt(&ex);
    initBigInt(&ex2);
    addBigInt(&ex2, 1);
    printBigInt(&ex2);
    printf("\n");
    int count  = 0;
    count  = 0;
    while (count < 1024)
        {
        multBySingleDigit(&ex2, 2);
        count++;
        }
    printBigInt(&ex2);
    printf("\n");
    initBigInt(&ex2);
    addBigInt(&ex2, 1);
    printBigInt(&ex2);
    printf("\n");
    count  = 0;
    while (count < 1024)
        {
        divBySingleDigit(&ex2, 2);
        count++;
        }
        printBigInt(&ex2);
        printf("\n");

#if 0
    divBySingleDigit(&ex2, 3);
    printBigInt(&ex2);
    printf("\n");
#endif
#if 0
    printBigInt(&ex2);
    printf("\n");
    multBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    multBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    multBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    multBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    divBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    divBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    divBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    divBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    divBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
    divBySingleDigit(&ex2, 5);
    printBigInt(&ex2);
    printf("\n");
#endif
#if 0
    printBigInt(&ex);
    printf("\n");
    addBigInt(&ex, CUT * 2 - 1);
    printBigInt(&ex);
    printf("\n");


    addBigInt(&ex2, ULONG_MAX);
    printBigInt(&ex2);
    printf("\n%lu\n", ULONG_MAX);
    addBigInt(&ex2, ULONG_MAX);
    printBigInt(&ex2);
    printf("\n");
    addBigInt(&ex2, ULONG_MAX);
    printBigInt(&ex2);
    printf("\n");
    addBigInt(&ex2, ULONG_MAX);
    printBigInt(&ex2);
    printf("\n");

    combineBigInt(&ex, &ex2);
    printBigInt(&ex);
#endif
    return 0;
    }
