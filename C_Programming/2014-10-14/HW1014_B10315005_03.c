#include <stdio.h>

int main () {
    double length, velocity;
    printf("Input length (m): ");
    scanf("%lf", &length);
    printf("Input velocity (km/hr): ");
    scanf("%lf", &velocity);

    velocity = velocity * 1000 / 3600;

    double acc = velocity * velocity / 2 / length;
    double time = 2 * length / velocity;

    printf("acceleration = %g m^2 / s\n", acc);
    printf("time = %g sec\n", time);

    return 0;
}
