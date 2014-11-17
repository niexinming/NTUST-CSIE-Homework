#include <stdio.h>
#include <string.h>

int main () { 
    int i, no;
    char buff[1024];
    while (scanf("%d %s", &no, buff) != EOF) {
        int len = strlen(buff);
        int H = 0, O = 0;
        for (i = 0; i < len; i++) {
            if (buff[i] == 'H') {
                H++;
            } else if (buff[i] == 'O') {
                O++;
            }
        }
        printf("Player %d's record: %s\n", no, buff);
        printf("Player %d's batting average: %.3f\n", no, (double)H / (H + O));
    }
    return 0;
}
