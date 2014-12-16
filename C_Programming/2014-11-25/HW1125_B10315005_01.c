#include <stdio.h>

#define DEMAND_CHG 70.0
#define PER_1k_CHG_LESS 1.0
#define PER_1k_CHG 1.1
#define LATE_CHG 5.0
#define DATA_COUNT 3

void instruct_water();
double comp_use_charge(int last_year_used, int this_year_used);
double comp_late_charge(double unpaid);
void display_bill(double late_charge, double bill, double unpaid, int months);

int main () {
    int last_year_used, this_year_used;
    double unpaid, bill;
    int used;
    double use_charge, late_charge;
    int i;

    // print use instruction
    instruct_water();

    // read unpaid balance
    printf("Enter unpaid balance > ");
    scanf("%lf", &unpaid);

    // calculate charge with basic charge and late charge
    late_charge = comp_late_charge(unpaid);
    bill = DEMAND_CHG * DATA_COUNT + unpaid + late_charge;

    // read data for each month
    for (i = 0; i < DATA_COUNT; i++) {
        printf("Data #%d:\n", i + 1);
        printf("Enter previous year used > ");
        scanf("%d", &last_year_used);
        printf("Enter this year used > ");
        scanf("%d", &this_year_used);

        use_charge = comp_use_charge(last_year_used, this_year_used);
        bill += use_charge;
    }

    // output the bill
    display_bill(late_charge, bill, unpaid, DATA_COUNT);
    return 0;
}

void instruct_water() {
    printf("================================================================\n");
    printf("This program figures a water bill ");
    printf("based on the demand charge\n");
    printf("($%.2f) and a $%.2f per 1000 ", DEMAND_CHG, PER_1k_CHG);
    printf("gallons use charge.\n\n");
    printf("A $%.2f surcharge is added to ", LATE_CHG);
    printf("accounts with an unpaid balance.\n\n");
    printf("Enter unpaid balance, last year use ");
    printf("and this year use\n");
    printf("on separate lines after the prompts.\n");
    printf("Press <return> or <enter> after ");
    printf("typing each number.\n\n");
    printf("================================================================\n");
}

double comp_use_charge(int last_year_used, int this_year_used) {
    double rate = (double)this_year_used / last_year_used;

    if (rate > 1.02) { // if rate > 1.02, chrage -> 2x
        return this_year_used * PER_1k_CHG * 2;
    } else if (rate < 0.98) { // if rate < 0.98, charge -> $1.0
        return this_year_used * PER_1k_CHG_LESS;
    }

    // normal situation
    return this_year_used * PER_1k_CHG;
}

double comp_late_charge(double unpaid) {
    // if there is unpaid balance, you have to pay late charge
    return (unpaid > 0) ? LATE_CHG : 0;
}

void display_bill(double late_charge, double bill, double unpaid, int months) {
    // display the bill with...
    // 1. late charge
    // 2. unpaid balance
    // 3. total bill
    // 4. N months

    printf("\n");
    printf("This bill is for %d months\n", months);

    // if there is late charge
    if (late_charge > 0.0) {
        printf("\nBill includes $%.2f late charge", late_charge);
        printf(" on unpaid balance of $%.2f\n", unpaid);
    }
    printf("\nTotal due = $%.2f", bill);
}
