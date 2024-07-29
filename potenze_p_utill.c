#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256

int main()
{
    int i = 0;
    int p = 53;
    long long int p_pow = 1;
    int m = 1e9 + 9;
    //long long sum = 0;
    //int hash = 0;

    printf("p_pow = {");
    for (i = 0; i < CMD_LEN; i++)
    {
        p_pow = (p_pow * p)%m;
        printf("%lld,",p_pow);
    }
    printf("}\n");
    return 0;
}
