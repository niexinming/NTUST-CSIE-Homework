#include <stdio.h>

#define TREE_INDENT_SIZE 2

typedef struct BINNODE_ {
    int value;
    short left_size;
    char used;
} BINNODE;

int data_count;
int array[1<<20];
BINNODE bstree[1<<20];

#define ROOT 1
#define PARENT(X) ((X)/2)
#define RIGHT(X) ((X) * 2 + 1)
#define LEFT(X) ((X) * 2)

int DEBUG_FLAG = 1;

void hang(char *reason)
{
    if(!reason) reason = "unknow error";
    fprintf(stderr, "[!] error: %s\n", reason);
    fprintf(stderr, "[+] attach debugger or Ctrl-C to exit...\n");
    fflush(stderr);
    while(DEBUG_FLAG);
}

int read_number()
{
    int n; scanf("%d", &n); return n;
}

void sort_array()
{
    int i, j, t;
    for(i = 0; i < data_count; i++) {
        for(j = i + 1; j < data_count; j++) {
            if(array[i] > array[j]) {
                t = array[i];
                array[i] = array[j];
                array[j] = t;
            }
        }
    }
}

void sorted_array_to_bst(int current, int l, int r)
{
    // range: [l, r]
    int m = (l + r) / 2;
    bstree[current].value = array[m];
    bstree[current].used = 1;
    bstree[current].left_size = 1;
    if(r - l < 2) return;
    sorted_array_to_bst(LEFT(current), l, m - 1);
    sorted_array_to_bst(RIGHT(current), m + 1, r);
}

int update_left_size(int current)
{
    if(bstree[RIGHT(current)].used)
        update_left_size(RIGHT(current));
    if(bstree[LEFT(current)].used) {
        bstree[current].left_size = 1 + update_left_size(LEFT(current));
        return bstree[current].left_size;
    } else {
        bstree[current].left_size = 1;
        return 1;
    }
}

void dump_tree(int node, int depth)
{
    if(!bstree[node].used)
        return;
    printf("%*snode(%d, %d)\n", depth * TREE_INDENT_SIZE, "", bstree[node].value, bstree[node].left_size);
    if(bstree[LEFT(node)].used || bstree[RIGHT(node)].used) {
        if(bstree[LEFT(node)].used)
            dump_tree(LEFT(node), depth + 1);
        else
            printf("%*sNULL\n", depth * TREE_INDENT_SIZE + TREE_INDENT_SIZE, "");
        if(bstree[RIGHT(node)].used)
            dump_tree(RIGHT(node), depth + 1);
        else
            printf("%*sNULL\n", depth * TREE_INDENT_SIZE + TREE_INDENT_SIZE, "");
    }
}

void print_in_level_order()
{
    int i = ROOT;
    int count = sizeof(bstree) / sizeof(bstree[0]);
    while(i < count) {
        if(bstree[i].used) {
            printf("%d ", bstree[i].left_size);
        }
        i++;
    }
    puts("");
}

void calc_top_n()
{
    while(1) {
        puts("top n average, n?");
        int n = read_number();
        if(n > data_count) {
            puts("-1");
        } else {
            int i = data_count - 1;
            int s = 0;
            int c = n;
            while(c > 0) {
                s += array[i];
                i--; c--;
            }
            printf("%d\n", s / n);
        }
    }
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    puts("how many?");
    int n = read_number();

    for(data_count = 0; data_count < n; data_count++) {
        array[data_count] = read_number();
    }

    sort_array();
    sorted_array_to_bst(ROOT, 0, data_count - 1);
    update_left_size(ROOT);
    dump_tree(ROOT, 0);
    print_in_level_order();
    calc_top_n();

    return 0;
}
