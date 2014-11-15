#include <stdio.h>

int main ()
{
    double a, b;
    printf("MILEAGE REIMBURSEMENT CALCUALTOR\n");
    printf("Enter geginning odometer reading=> ");
    scanf("%lf", &a);
    printf("Enter ending odometer reading=> ");
    scanf("%lf", &b);
    printf("You traveled %f miles. At $0.35 per mile,\n", b - a);
    printf("your reimbursement is $%f.", (b - a) * 0.35);
    return 0;
}
