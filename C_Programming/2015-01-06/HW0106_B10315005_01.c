#include <stdio.h>

int factorial(int n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

int main ()
{
    int b;
    printf("Input : ");
    scanf("%d", &b);
    printf("factorial(%d) = %d\n", b, factorial(b));
    return 0;
}
