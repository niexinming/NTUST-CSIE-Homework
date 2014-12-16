#include <stdio.h>
#include <math.h>

void sum_n_square(double *n1, double *n2, double *n3, double *sum, double *sqr);

int main()
{
    double n1, n2, n3, sum, sqr;

    printf("Input 3 numbers: ");
    scanf("%lf%lf%lf", &n1, &n2, &n3);

    sum_n_square(&n1, &n2, &n3, &sum, &sqr);

    printf("sum = %g\nsquare = %g\n", sum, sqr);

    return 0;
}

void sum_n_square(double *n1, double *n2, double *n3, double *sum, double *sqr)
{
    *sum = *n1 + *n2 + *n3;
    *sqr = sqrt(*sum);
}
