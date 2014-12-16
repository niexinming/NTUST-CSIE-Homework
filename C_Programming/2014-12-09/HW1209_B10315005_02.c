#include <stdio.h>
#include <math.h>

void order(double *n1, double *n2);

int main()
{
    double n1, n2, n3, n4;

    printf("Input 4 numbers: ");
    scanf("%lf%lf%lf%lf", &n1, &n2, &n3, &n4);

    order(&n1, &n2);
    order(&n1, &n3);
    order(&n1, &n4);
    order(&n2, &n3);
    order(&n2, &n4);
    order(&n3, &n4);

    printf("n1 = %g\n", n1);
    printf("n2 = %g\n", n2);
    printf("n3 = %g\n", n3);
    printf("n4 = %g\n", n4);

    return 0;
}

void order(double *n1, double *n2)
{
    if (*n1 > *n2) {
        double t = *n1;
        *n1 = *n2;
        *n2 = t;
    }
}
