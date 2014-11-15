#include <stdio.h>
#define GRAVITY    9.8
#define EFFICIENCY 0.9

int main () {
    double height, flow_speed, kg_per_sec, work, work_in_mega;
    printf("Input height: ");
    scanf("%lf", &height);
    printf("Input flow speed: ");
    scanf("%lf", &flow_speed);
    kg_per_sec  = flow_speed * 1000;
    work = kg_per_sec * GRAVITY * height * EFFICIENCY;
    work_in_mega = work / 1000000;
    printf("Work = %f MW\n", work_in_mega);
    return 0;
}
