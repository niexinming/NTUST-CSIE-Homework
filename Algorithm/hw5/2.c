// prim

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char is_connected[1001][1001];
int weights[1001][1001];
char visited[1001];

// Verticies, Edges, Option
int V, E, O;

void init()
{
    memset(is_connected, 0, sizeof(is_connected));
    memset(weights, 0, sizeof(weights));
    memset(visited, 0, sizeof(visited));
}

void solve()
{
    visited[0] = 1;

    long long weight = 0;
    int i, j; // vertex index
    int l, lf, lv; // lowest index, lowest index from, lowest value

    while(1) {
        lv = INT_MAX;
        l = -1;

        // search a nearst vertex to current tree
        for(i = 0; i < V; i++) {
            // must start from a visited vertex
            if(visited[i]) {
                // search edge from V(i) to V(j)
                for(j = 0; j < V; j++) {
                    // V(i) to V(j) is connected and V(j) not visited and its weight is lower than current lv
                    if(is_connected[i][j] && !visited[j] && lv > weights[i][j]) {
                        // record lowest weight
                        lv = weights[i][j];
                        l = j;
                        lf = i;
                    }
                }
            }
        }

        // if no edge were found, it means we have visited all vertex
        if(l == -1) {
            break;
        }

        if(O == 1) {
            // we want the right order accroding to the spec
            if(lf < l) {
                printf("%d %d\n", lf, l);
            } else {
                printf("%d %d\n", l, lf);
            }
        }

        // drop visited edge, mark as visited, accumulate weight
        is_connected[lf][l] = 0;
        is_connected[l][lf] = 0;
        visited[l] = 1;
        weight += lv;
    }

    printf("%lld\n", weight);
}

int main()
{
    freopen("test.txt", "r", stdin);

    int i, N;
    int a, b, w;
    scanf("%d", &N); // how many test data

    while(N-- > 0) {
        scanf("%d%d%d", &V, &E, &O);
        init();

        // read edges data
        for(i = 0; i < E; i++) {
            scanf("%d%d%d", &a, &b, &w);
            if(!is_connected[a][b] || w < weights[a][b]) {
                is_connected[a][b] = 1;
                is_connected[b][a] = 1;
                weights[a][b] = w;
                weights[b][a] = w;
            }
        }

        solve();
    }

    return 0;
}
