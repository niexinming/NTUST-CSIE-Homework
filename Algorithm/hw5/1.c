// kruskal

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a, b, weight;
} Edge;

Edge edges[10001];

// disjoint set
int ds[1001];

// disjoint set init
void ds_init(int n)
{
    int i;
    for(i = 0; i < n; i++) {
        ds[i] = i;
    }
}

// find root of member from disjoint set
int ds_root(int i)
{
    while(ds[i] != i)
        i = ds[i];
    return i;
}

// union two disjoint set
int ds_union(int which, int to) {
    return ds[ds_root(which)] = ds_root(to);
}

// test two member are in same disjoint set or not
int ds_same(int a, int b) {
    return ds_root(a) == ds_root(b);
}

// Verticies, Edges, Option
int V, E, O;

int cmp_edge(const void *a, const void *b)
{
    int r = ((Edge*)a)->weight - ((Edge*)b)->weight;
    if(r) return r;
    r = ((Edge*)a)->a - ((Edge*)b)->a;
    if(r) return r;
    return ((Edge*)a)->b - ((Edge*)b)->b;
}

void solve()
{
    // init disjoint set and sort edges by weight
    ds_init(E);
    qsort(edges, E, sizeof(Edge), cmp_edge);

    long long weight = 0;
    int i = 1, j = 0; // i from 1 to V-1, j from 0 to E-1

    // i >= V-1 means all the verticies are connected to each other
    // j >= E means all edges are processed
    while(i < V && j < E) {
        // skip rings
        while(ds_same(edges[j].a, edges[j].b)) {
            j++;
        }

        // take current edge, accumulate weight and merge these two MST
        weight += edges[j].weight;
        ds_union(edges[j].a, edges[j].b);

        if(O == 1) {
            printf("%d %d\n", edges[j].a, edges[j].b);
        }

        i++;
        j++;
    }

    printf("%lld\n", weight);
}

int main()
{
    freopen("test.txt", "r", stdin);

    int i, N;
    scanf("%d", &N); // how many test data

    while(N-- > 0) {
        scanf("%d%d%d", &V, &E, &O);

        // read edges data
        for(i = 0; i < E; i++) {
            scanf("%d%d%d", &edges[i].a, &edges[i].b, &edges[i].weight);
            if(edges[i].a > edges[i].b) {
                int t = edges[i].a;
                edges[i].a = edges[i].b;
                edges[i].b = t;
            }
        }

        solve();
    }

    return 0;
}
