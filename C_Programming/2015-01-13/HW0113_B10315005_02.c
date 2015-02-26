#include <stdio.h>

int main()
{
    double hours, rate, pay;
    double total_pay = 0.0;
    int number_emp;
    int count_emp = 0;

    printf("Input count of employees >");
    scanf("%d", &number_emp);

    while (count_emp < number_emp) {
        printf("Hours >");
        scanf("%lf", &hours);
        printf("Rate > $");
        scanf("%lf", &rate);
        pay = hours * rate;
        printf("Pay is $%6.2f\n\n", pay);
        total_pay += pay;
        count_emp++;
    }

    printf("All employees processed\n");
    printf("Total payroll is $%8.2f\n", total_pay);

    return 0;
}
