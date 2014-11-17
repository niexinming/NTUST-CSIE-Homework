#include <stdio.h>
#include <math.h>

double calc_area(double, double, double, double f(double));
double g(double);
double h(double);

int main () {
    double delta;
    printf("Input delta x: ");
    scanf("%lf", &delta);

    printf("Function g -> %g\n", calc_area(0, 3.14159, delta, g));
    printf("Function h -> %g\n", calc_area(-2, 2, delta, h));
    printf("Sine       -> %g\n", calc_area(0, M_PI, delta, sin));

    return 0;
}

double calc_area(double x_start, double x_end, double delta_x, double f(double)) {
    double area = 0, x = x_start;
    double y, last_y = f(x_start);
    x_end -= delta_x;
    while (x < x_end) {
        x += delta_x;
        y = f(x);
        area += fabs((last_y + y) * delta_x / 2);
        last_y = y;
    }
    return area;
}

double g(double x) {
    return pow(x, 2) * sin(x);
}

double h(double x) {
    return sqrt(4 - pow(x, 2));
}
