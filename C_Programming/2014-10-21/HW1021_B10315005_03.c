#include <stdio.h>
#include <math.h>

int main () {
    double side_a, side_b, alpha;
    printf("Input side a: ");
    scanf("%lf", &side_a);
    printf("Input side b: ");
    scanf("%lf", &side_b);
    printf("Input alpha: ");
    scanf("%lf", &alpha);

    double side_c = sqrt(pow(side_a, 2) + pow(side_b, 2) - 2 * side_a * side_b * cos(alpha * M_PI / 180.0));
    printf("side c = %f\n", side_c);

    return 0;
}
