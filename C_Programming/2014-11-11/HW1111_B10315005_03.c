#include <stdio.h>
#include <math.h>

#define FALSE 0
#define TRUE  1
#define MAX_TRIES 100

int newton_method(double, double, double f(double), double f_d(double), int, double *);
double m(double);
double m_d(double);
double g(double);
double g_d(double);
double h(double);
double h_d(double);
double func(double);
double func_d(double);

double N, C;

int main () {
    double epsilon, root;
    printf("Enter tolerance: ");
    scanf("%lf", &epsilon);

    printf("Enter n: ");
    scanf("%lf", &N);
    printf("Enter c: ");
    scanf("%lf", &C);

    if (newton_method(C / 2, epsilon, m, m_d, MAX_TRIES, &root))
        printf("m(%.7f) = %e\n", root, m(root));

    //printf("Funcion g\n");
    //if (newton_method(1.5, epsilon, g, g_d, MAX_TRIES, &root))
    //    printf("  g(%.7f) = %e\n", root, g(root));

    //printf("Funcion h\n");
    //if (newton_method(-4, epsilon, h, h_d, MAX_TRIES, &root))
    //    printf("  h(%.7f) = %e\n", root, h(root));

    //printf("Funcion func\n");
    //if (newton_method(0.5, epsilon, func, func_d, MAX_TRIES, &root))
    //    printf("  h(%.7f) = %e\n", root, func(root));

    return 0;
}

int newton_method(double x_guess, double epsilon, double f(double), double f_d(double), int max, double *result) {
    double y;
    int root_found = FALSE;
    int count = 0;

    printf("initx = %.7f\tf(guess) = %.7f\n", x_guess, y);

    while (max-- > 0 && !root_found) {
        x_guess = x_guess - f(x_guess) / f_d(x_guess);
        y = f(x_guess);
        printf("guess = %.7f\tf(guess) = %.7f\n", x_guess, y);
        if (fabs(y) <= epsilon) {
            root_found = TRUE;
            *result = x_guess;
        }
    }
    return root_found;
}

double m(double x) {
    return pow(x, N) - C;
}

double m_d(double x) {
    return N * pow(x, N - 1);
}

// g(x) = 5x^3 - 2x^2 + 3
double g(double x) {
    return 5 * pow(x, 3) - 2 * pow(x, 2) + 3;
}

// g'(x) = 15x^2 - 4x
double g_d(double x) {
    return 15 * pow(x, 2) - 4 * x;
}

// h(x) = x^4 - 3x^2 - 8
double h(double x) {
    return pow(x, 4) - 3 * pow(x, 2) - 8;
}

// h'(x) = 4^x3 - 6x
double h_d(double x) {
    return 4 * pow(x, 3) - 6 * x;
}

// func(x) = x^2 + 2x + 1
double func(double x) {
    return pow(x, 2) + 2 * x + 1;
}

// func'(x) = 2x + 2
double func_d(double x) {
    return 2 * x + 2;
}
