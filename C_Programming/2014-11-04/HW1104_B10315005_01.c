#include <stdio.h>

int isUpper(char x) {
    return 'A' <= x && x <= 'Z';
}

int isLower(char x) {
    return 'a' <= x && x <= 'z';
}

int isAlpha(char x) {
    return isUpper(x) || isLower(x);
}

int main () {
    char x;
    int i, j, n;
    printf("Which character? ");
    scanf("%c", &x);
    if (!isAlpha(x)) {
        printf("This is not a alpha (%c)\n", x);
        return 0;
    }
    printf("How many? ");
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++)
            printf("%c", x);
        puts("");
    }
    if (isUpper(x))
        x = x - ('A' - 'a');
    else
        x = x - ('a' - 'A');
    for (i = n; i > 0; i--) {
        for (j = 0; j < i; j++)
            printf("%c", x);
        puts("");
    }
    return 0;
}
