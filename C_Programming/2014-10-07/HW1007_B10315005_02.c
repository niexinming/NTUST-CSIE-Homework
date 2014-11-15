#include <stdio.h>
#define GALLONS_PER_BARREL 4.2e1
#define ENERGY_PER_BARRERL 5.8e6

int main () {
    const double source_oil = 100; // gallons
    const double efficiency = .65; // 65%
    double energy = 100 / GALLONS_PER_BARREL * ENERGY_PER_BARRERL * efficiency;
    printf("You'll got %f BTU in this burning. (with %f gallons' oil and %f%% efficiency)\n", energy, source_oil, efficiency * 100);
    return 0;
}
