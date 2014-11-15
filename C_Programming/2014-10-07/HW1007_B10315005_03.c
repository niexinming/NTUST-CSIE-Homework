#include <stdio.h>
#define TOILET_FLUSHES_PER_DAY 14
#define OLD_TOILET_COST_PER_FLUSH 15
#define NEW_TOILET_COST_PER_FLUSH 2
#define PRICE_NEW_TOILET 150

int main () {
    double population;
    printf("How many population in this city? ");
    scanf("%lf", &population);
    unsigned long long int count_of_toilet = (unsigned long long int)((population + 2) / 3);
    double saved_water_in_liters = count_of_toilet * TOILET_FLUSHES_PER_DAY * (OLD_TOILET_COST_PER_FLUSH - NEW_TOILET_COST_PER_FLUSH);
    unsigned long long int cost_to_change_all_the_toilet = count_of_toilet * PRICE_NEW_TOILET;
    printf("You can save %f liters water per day, and you need %llu$ for new toilets\n", saved_water_in_liters, cost_to_change_all_the_toilet);
    return 0;
}
