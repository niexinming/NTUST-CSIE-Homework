#include <stdio.h>

int main()
{
    char buffer[128];
    while (scanf("%s", buffer) != EOF) {
        if (buffer[0] == '9')
            break;
        printf("%s starts with the letter %c\n", buffer, buffer[0]);
    }
    return 0;
}
