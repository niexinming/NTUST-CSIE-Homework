#include <stdlib.h>
#include <stdio.h>

void bbsort(double *arr, int len)
{
    int i;
    double t;
    if (len > 1) {
        for (i = 1; i < len; i++) {
            if (arr[0] > arr[i]) {
                t = arr[0];
                arr[0] = arr[i];
                arr[i] = t;
            }
        }
        bbsort(arr + 1, len - 1);
    }
}

void merge(double *dst, double *x, double *y, int cx, int cy)
{
    int i = 0, ix = 0, iy = 0;
    while (ix < cx || iy < cy) {
        if (x[ix] < y[iy] || cy == iy) {
            dst[i++] = x[ix++];
        } else {
            dst[i++] = y[iy++];
        }
    }
}

void clean_dup(double *dst, int *len)
{
    int i, j, l = *len - 1;
    for (i = 0; i < l; i++) {
        if (dst[i] == dst[i + 1]) {
            for (j = i; j < l; j++)
                dst[j] = dst[j + 1];
            l--;
            i--;
            (*len)--;
        }
    }
}

int main()
{
    int i, n1, n2, n_m;
    double *a1, *a2, *m;

    printf("How many element(s) in array 1? ");
    scanf("%d", &n1);
    printf("How many element(s) in array 2? ");
    scanf("%d", &n2);

    a1 = calloc(n1, sizeof(double));
    a2 = calloc(n2, sizeof(double));
    m = calloc(n1 + n2, sizeof(double));

    printf("Input for array 1: ");
    for (i = 0; i < n1; i++)
        scanf("%lf", a1 + i);
    printf("Input for array 2: ");
    for (i = 0; i < n2; i++)
        scanf("%lf", a2 + i);

    printf("Content of array 1:\n");
    for (i = 0; i < n1; i++)
        printf("%g ", a1[i]);
    puts("");
    printf("Content of array 2:\n");
    for (i = 0; i < n2; i++)
        printf("%g ", a2[i]);
    puts("");

    bbsort(a1, n1);
    bbsort(a2, n2);

    /*
    printf("Sorted content of array 1:\n");
    for (i = 0; i < n1; i++)
        printf("%g ", a1[i]);
    puts("");
    printf("Sorted content of array 2:\n");
    for (i = 0; i < n2; i++)
        printf("%g ", a2[i]);
    puts("");
    */

    n_m = n1 + n2;
    merge(m, a1, a2, n1, n2);
    clean_dup(m, &n_m);

    printf("Merged array:\n");
    for (i = 0; i < n_m; i++) {
        printf("%g ", m[i]);
    }
    puts("");

    free(a1);
    free(a2);
    free(m);
    return 0;
}
