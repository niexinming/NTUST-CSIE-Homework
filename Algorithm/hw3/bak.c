#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TASK_ {
    uint32_t number;
    uint32_t start;
    uint32_t finish;
    uint32_t value;
} TASK;

TASK tasks[1024];
uint32_t timemap[1048];

int bisect(uint32_t *array, size_t size, uint32_t v)
{
    int l = 0, r = size - 1, m;
    while(l <= r) {
        m = (l + r) / 2;
        if(array[m] > v)      r = m - 1;
        else if(array[m] < v) l = m + 1;
        else return m;
    }
    return -1;
}

int cmp_int(const void *a, const void *b)
{
    int *v1 = a, *v2 = b;
    return *v1 - *v2;
}

int order_by_finish(const void *a, const void *b)
{
    TASK *t1 = a, *t2 = b;
    return t1->finish - t2->finish;
}

int order_by_start(const void *a, const void *b)
{
    TASK *t1 = a, *t2 = b;
    return t1->finish - t2->finish;
}

size_t compact(uint32_t *arr, size_t count)
{
    ptrdiff_t i = 0, j = 0;
    while(j < count) {
        arr[i] = arr[j];
        while(arr[i] == arr[j]) j++;
        i++;
    }
    return i;
}

void create_timemap(TASK *tasks, size_t count, uint32_t *map)
{
    int i;
    for(i = 0; i < count; i++) {
        map[i] = tasks[i].start;
        map[i+count] = tasks[i].finish;
    }
    qsort(map, count * 2, sizeof(uint32_t), cmp_int);
    size_t timemap_size = compact(map, count * 2);
    for(i = 0; i < count; i++) {
        map[i].start = bisect(map, count, map[i].start);
        map[i].finish = bisect(map, count, map[i].finish);
    }
}

void solve(TASK* tasks, size_t count)
{
    qsort(tasks, count, sizeof(TASK), order_by_finish);
    create_timemap(tasks, count, timemap);
}

// read data from stdin to given pointer to TASK object
void read_task(TASK *t)
{
    scanf("%d", &t->number);
    scanf("%d", &t->start);
    scanf("%d", &t->finish);
    scanf("%d", &t->value);
}

int main()
{
    int32_t test_count;
    scanf("%d", &test_count); // how many test data
    while(test_count-- > 0) {
        size_t i, N;
        memset(tasks, 0, sizeof(tasks));
        scanf("%d", &N); // task count
        for(i = 0; i < N; i++) {
            read_task(&tasks[i]);
        }
        solve(tasks, N);
    }
    return 0;
}
