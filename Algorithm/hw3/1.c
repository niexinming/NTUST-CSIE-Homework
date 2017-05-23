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

int order_by_finish(const void *a, const void *b)
{
    TASK *t1 = (TASK*)a, *t2 = (TASK*)b;
    return t1->finish - t2->finish;
}

void solve(TASK* tasks, size_t count)
{
    qsort(tasks, count, sizeof(TASK), order_by_finish);
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
        int i, N;
        memset(tasks, 0, sizeof(tasks));
        scanf("%d", &N); // task count
        for(i = 0; i < N; i++) {
            read_task(&tasks[i]);
        }
        solve(tasks, N);
    }
    return 0;
}
