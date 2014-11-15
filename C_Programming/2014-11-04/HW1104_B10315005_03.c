#include <stdio.h>
#include <math.h>

long long int gcd(long long int a, long long int b) {
    a = (long long int)abs(a);
    b = (long long int)abs(b);
    long long int c;
    while (a && b) {
        a %= b;
        c = a;
        a = b;
        b = c;
    }
    return a;
}

int main () {
    long long int a, b;
    printf("Input number a:");
    scanf("%lld", &a);
    printf("Input number b:");
    scanf("%lld", &b);

    printf("gcd(%lld, %lld) = %lld\n", a, b, gcd(a, b));
    return 0;
}
