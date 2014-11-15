#include <stdio.h>

int main () {
    double m, n;

    printf("m = ");
    scanf("%lf", &m);
    printf("n = ");
    scanf("%lf", &n);

    if (m <= n) {
        printf("m must bigger than n\n");
        return 0;
    }

    printf("side1 = %g\n", m * m - n * n);
    printf("side2 = %g\n", 2 * m * n);
    printf("hypotenuse = %g\n", m * m + n * n);

    return 0;
}
