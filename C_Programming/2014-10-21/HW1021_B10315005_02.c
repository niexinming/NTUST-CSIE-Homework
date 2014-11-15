#include <stdio.h>

void draw_base() {
    puts("-------");
}

void draw_parallel() {
    int i;
    for (i = 0; i < 5; i++)
        puts("|     |");
}

void display_rectangle() {
    draw_base();
    draw_parallel();
    draw_base();
}

void display_circle() {
    puts("  *  ");
    puts("*   *");
    puts(" *** ");
}

int main () {
    display_circle();
    display_rectangle();
    return 0;
}
