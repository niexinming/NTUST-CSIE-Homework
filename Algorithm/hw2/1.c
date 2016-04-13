#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// table accessor, to deal with border
#define table(X, Y) raw_table[X + 1][Y + 1]

// linked-list to parent, depth and data
struct CELL {
    struct CELL * parent;
    uint16_t depth;
    char data;
} raw_table[1001][1001];

// utility functions for debugging
void dump_cell(struct CELL * cell, int p)
{
    if(!cell->data) {
        printf("%-6s", "X");
        return;
    }
    printf("(");
    //printf("%p, ", cell->parent);
    printf("%d, ", cell->depth);
    printf("%c)", cell->data);
    if(p > 0 && cell->parent) {
        printf(" -> ");
        dump_cell(cell->parent, p - 1);
    }
}

void dump_all(int h, int w)
{
    int i,j;
    for(i = 0; i < h; i++) {
        for(j = 0; j < w; j++) {
            dump_cell(&table(i, j), 0);
            printf(" ");
        }
        puts("");
    }
}

// return value is length of result
int find_longest_mirror_subsequance(const char * buffer, char * output)
{
    memset(raw_table, 0, sizeof(raw_table)); // clean table
    size_t len = strlen(buffer); // cache buffer length
    int i, j; // table index

    for(i = 0; i < len; i++) {
        for(j = 0; j < len; j++) {
            // forward-iteration and backward-iteration
            if(buffer[i] == buffer[len - j - 1]) {
                table(i, j).parent = &table(i-1, j-1);
                table(i, j).depth = table(i, j).parent->depth + 1;
                table(i, j).data = buffer[i];
            } else {
                if(table(i, j-1).depth > table(i-1, j).depth) {
                    table(i, j) = table(i, j-1);
                } else {
                    table(i, j) = table(i-1, j);
                }
            }
            // printf("\n(%d, %d) -> ", i, j);
            // dump_cell(&table(i, j), 999);
            // puts("");
            // dump_all(len, len);
            // dump_cell(&table(i, j), 9);
        }
    }

    struct CELL * p = &raw_table[len][len];

    while(p) {
        //dump_cell(p, 0);
        *output++ = p->data;
        p = p->parent;
    }
    *output = '\0';

    return raw_table[len][len].depth;
}

// find first breakline char and replace with null char
void rstrip(char *p)
{
    do {
        switch(*p) {
            case '\r':
            case '\n':
                *p = '\0';
                return;
        }
    } while(*p++);
}

int main()
{
    // read data buffer, result buffer
    char buffer[4096], output[4096];
    int N;

    freopen("test.txt", "r", stdin); // reopen stdin to read test.txt

    fgets(buffer, sizeof(buffer) - 1, stdin); // read a line from input and eat "\n"
    N = atoi(buffer); // string to interger

    while(N--) {
        fgets(buffer, sizeof(buffer) - 1, stdin); // read a test data from input
        rstrip(buffer);
        size_t length = find_longest_mirror_subsequance(buffer, output);
        printf("%d\n%s\n", length, output);
    }

    return 0;
}
