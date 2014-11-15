#include <stdio.h>

int main () {
    char grade_name[128];
    double mini_avg_req, cur_avg_cur, percentage, res;
    printf("Enter desired grade> ");
    scanf("%128[^\n]s\n", grade_name);
    printf("Enter minimum average required> ");
    scanf("%lf", &mini_avg_req);
    printf("Enter current average in course> ");
    scanf("%lf", &cur_avg_cur);
    printf("Enter how mouch the final counts as a percentage "
           "of the course grade> ");
    scanf("%lf", &percentage);
    percentage /= 100;

    res = (mini_avg_req - cur_avg_cur * (1 - percentage)) / percentage;

    printf("You need a score of %f on the final to get a %s.\n",
           res, grade_name);
    return 0;
}
