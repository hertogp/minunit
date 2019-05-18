#include <stdio.h>
#include "x.h"

int
summ(int a, int b)
{
    return a + b;
}

int
main(void)
{
    printf("To summ it up:\n");
    printf("2 + 2 = %d\n", summ(2, 2));

    return 0;
}
