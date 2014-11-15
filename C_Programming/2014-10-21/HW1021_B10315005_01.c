#include <stdio.h>

void draw_base() {
    puts("-------");
}

void draw_parallel() {
    int i;
    for (i = 0; i < 5; i++)
        puts("|     |");
}

int main () {
    draw_base();
    draw_parallel();
    draw_base();
    return 0;
}
