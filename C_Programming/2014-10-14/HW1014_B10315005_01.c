#include <stdio.h>

int main () {
    double x1, y1, x2, y2;
    printf("Input example: 1.5,4.2\n");
    printf("Input p1(x, y): ");
    scanf("%lf,%lf", &x1, &y1);
    printf("Input p2(x, y): ");
    scanf("%lf,%lf", &x2, &y2);
    printf("(%g, %g) - (%g, %g)\n", x1, y1, x2, y2);

    double mx = (x1 + x2) / 2, my = (y1 + y2) / 2;

    if (x1 == x2) {
        printf("y = %g\n", my);
    } else if (y1 == y2) {
        printf("x = %g\n", mx);
    } else {
        double slope = - (x1 - x2) / (y1 - y2);
        double y_intercept = my - slope * mx;
        printf("y = %g * x + %g\n", slope, y_intercept);
    }

    return 0;
}
