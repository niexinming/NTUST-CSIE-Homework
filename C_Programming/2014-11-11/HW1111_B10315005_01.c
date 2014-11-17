#include <stdio.h>
#include <math.h>

#define FALSE 0
#define TRUE  1
#define NO_ROOT -99999.0

double bisect(double, double, double, double f(double), int, const char *);
double g(double);
double h(double);
double func(double);

int main () {
    double x_left, x_right, epsilon, root;
    printf("Enter interval endpoints: ");
    scanf("%lf%lf", &x_left, &x_right);
    printf("Enter tolerance: ");
    scanf("%lf", &epsilon);
    printf("Funcion g\n");
    root = bisect(x_left, x_right, epsilon, g, FALSE, "g");

    printf("Function h\n");
    root = bisect(x_left, x_right, epsilon, h, FALSE, "h");

    // printf("Function func\n");
    // root = bisect(x_left, x_right, epsilon, func, FALSE, "func");

    return 0;
}

double bisect(double x_left, double x_right, double epsilon, double f(double), int sub_search, const char * func_name) {
    double x_mid, f_left, f_mid, f_right;
    int root_found;

    if (x_right - x_left > 1) {
        printf("Sub-search is performing...\n");

        double new_right = x_left + 1;

        while (new_right <= x_right) {
            double result = bisect(x_left, new_right, epsilon, f, TRUE, func_name);
            if (result != NO_ROOT)
                printf("  %s(%.7f) = %e\n", func_name, result, f(result));
            x_left = new_right;
            new_right += 1;
        }
        return NO_ROOT;
    }

    if (sub_search) printf("\n");

    f_left = f(x_left);
    f_right = f(x_right);

    if (f_left * f_right > 0) {
        printf("May be no root in [%.7f, %.7f]\n", x_left, x_right);
        if (sub_search) printf("\n");
        return NO_ROOT;
    }

    root_found = FALSE;
    while (fabs(x_right - x_left) > epsilon && !root_found) {
        x_mid = (x_left + x_right) / 2;
        f_mid = f(x_mid);

        if (f_mid == 0.0) {
            root_found = TRUE;
        } else if (f_left * f_mid < 0.0) {
            x_right = x_mid;
        } else {
            x_left = x_mid;
        }

        if (root_found)
            printf("Root found at x = %.7f, midpoint of [%.7f, %.7f]\n", x_mid, x_left, x_right);
        else
            printf("New interval is [%.7f, %.7f]\n", x_left, x_right);
    }

    if (sub_search) printf("\n");
    return (x_left + x_right) / 2;
}

// g(x) = 5x^3 - 2x^2 + 3
double g(double x) {
    return 5 * pow(x, 3) - 2 * pow(x, 2) + 3;
}

// h(x) = x^4 - 3x^2 - 8
double h(double x) {
    return pow(x, 4) - 3 * pow(x, 2) - 8;
}

double func(double x) {
    return pow(x, 2) + 2 * x + 1;
}
