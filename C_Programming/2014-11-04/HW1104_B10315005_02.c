#include <stdio.h>
#include <math.h>

int main () {
    int N, i;
    printf("How many numbers: ");
    scanf("%d", &N);
    int n;
    int count_of_odd = 0, count_of_even = 0;
    int sum_of_odd = 0, sum_of_even = 0;
    for (i = 0; i < N; i++) {
        printf("Input number #%d:", i + 1);
        scanf("%d", &n);
        if (n % 2 == 0) {
            count_of_even++;
            sum_of_even += n;
        } else {
            count_of_odd++;
            sum_of_odd += n;
        }
    }
    puts("");
    printf("Count of even = %d\nCount of odd = %d\n",
            count_of_even, count_of_odd);
    printf("Sum of even = %d\nSum of odd = %d\n",
            sum_of_even, sum_of_odd);
    printf("Square of sum of even = %d\nSquare of sum of odd = %d\n",
            (int)pow(sum_of_even, 2), (int)pow(sum_of_odd, 2));
    return 0;
}
