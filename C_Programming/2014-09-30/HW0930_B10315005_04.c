#include <stdio.h>

int main () {
    double VTBI, rate;
    printf("Volumn to be infused (ml) => ");
    scanf("%lf", &VTBI);
    printf("Minutes over which to infuse => ");
    scanf("%lf", &rate);

    printf("VTBI: %.0f ml\n", VTBI);
    printf("Rate: %.0f ml/hr\n", VTBI * 60 / rate);
    return 0;
}
