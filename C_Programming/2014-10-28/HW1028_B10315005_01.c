#include <stdio.h>

void swap(int *a, int *b) {
    int t;
    t = *a;
    *a = *b;
    *b = t;
}

int main () {
    int x, y, z;
    printf("x = ");
    scanf("%d", &x);
    printf("y = ");
    scanf("%d", &y);
    printf("z = ");
    scanf("%d", &z);

    if (z < x) {
        swap(&x, &z);
    }
    if (z < y) {
        swap(&z, &y);
    }
    if (y < x) {
        swap(&x, &y);
    }

    printf("x = %d, y = %d, z = %d\n", x, y, z);
    return 0;
}
