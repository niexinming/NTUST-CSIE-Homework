#include <stdio.h>
#define SPEED_SQUARE_FEET_PER_HOURS 2

int main () {
    double house_l, house_w, yard_l, yard_w;
    printf("Input length of house in feet: ");
    scanf("%lf", &house_l);
    printf("Input width of house in feet: ");
    scanf("%lf", &house_w);
    printf("Input length of yard in feet: ");
    scanf("%lf", &yard_l);
    printf("Input width of yard in feet: ");
    scanf("%lf", &yard_w);

    double house_area, yard_area;
    house_area = house_l * house_w;
    yard_area = yard_l * yard_w;
    double area = yard_area - house_area;
    double time = area / SPEED_SQUARE_FEET_PER_HOURS;
    printf("You need %f hours to deal with %f square feets\n", time, area);
    return 0;
}
