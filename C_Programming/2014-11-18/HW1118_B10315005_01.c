#include <stdio.h>

void bsort(int arr[], int count) {
    int i, j, t;
    for (i = 0; i < count; i++)
        for (j = 0; j < count; j++)
            if (arr[i] < arr[j]) {
                t = arr[i]; arr[i] = arr[j]; arr[j] =t ;
            }
}

int main () {
    int i, arr[10];
    printf("Input 10 numbers: ");
    for (i = 0; i < 10; i++)
        scanf("%d", arr + i);
    bsort(arr, 10);
    for (i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    return 0;
}
