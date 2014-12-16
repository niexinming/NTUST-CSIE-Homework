#include <stdio.h>

int main()
{
    int i, j;
    printf("%4s", "");
    for (i = 6; i <= 10; i++)
        printf("%-4d", i);
    puts("");
    for (i = 6; i <= 10; i++) {
        printf("%-4d", i);
        for (j = 6; j <= 10; j++) {
            printf("%-4d", i * j);
        }
        puts("");
    }
    return 0;
}
