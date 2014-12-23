#include <stdio.h>

unsigned long long int fact_calc(int n)
{
    unsigned long long int result = 1;
    if (n > 1) {
        printf("%d! = ", n);
        while (n > 1) {
            printf("%d x ", n);
            result *= n--;
        }
        printf("1 = %lld\n", result);
    } else {
        printf("%d! = 1\n", n);
    }
    return result;
}

void display_message()
{
    printf("Enter an interger between 0 and 9 or -1 to quit: ");
}

int main()
{
    int n;
    display_message();
    while (scanf("%d", &n) != EOF) {
        if (n == -1)
            break;
        if (0 <= n && n <= 9) {
            fact_calc(n);
        } else {
            printf("Invalid entry\n");
        }
        display_message();
    }
    return 0;
}
