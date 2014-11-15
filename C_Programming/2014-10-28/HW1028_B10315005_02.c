#include <stdio.h>

int main () {
    int x, y, res;
    char op = '\n';
    printf("x = ");
    scanf("%d", &x);
    printf("y = ");
    scanf("%d", &y);
    printf("op = ");
    do {
        scanf("%c", &op);
    } while (op == '\n'); // we don't want '\n'

    if (op == '+') {
        res = x + y;
    } else if (op == '-') {
        res = x - y;
    } else if (op == '*') {
        res = x * y;
    } else if (op == '/') {
        res =  x / y;
    } else {
        printf("Unknow operator!\n");
        return 0;
    }

    printf("%d %c %d = %d\n", x, op, y, res);
    return 0;
}
