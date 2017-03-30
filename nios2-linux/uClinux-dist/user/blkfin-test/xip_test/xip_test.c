#include <stdio.h>

int test_func(void)
{
    return 1;
}

int main(void)
{
#ifdef __BFIN_FDPIC__
    int (**pf)(void);

    pf = test_func;
    printf("Code is at 0x%08x!\n", (int)*pf);
#else
    int (*pf)(void);

    pf = test_func;
    printf("Code is at 0x%08x!\n", (int)pf);
#endif

    return 0;
}
