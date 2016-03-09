#include <stdint.h>
#include <stdio.h>
#include <string.h>

int data[100000];

typedef struct SEARCH_RESULT_ {
    int left;
    int right;
    int64_t value;
} SEARCH_RESULT;

void find_max_corssing_subarray(int l, int m, int h, SEARCH_RESULT *result)
{
    int64_t left_sum = INT64_MIN, right_sum = INT64_MIN, sum = 0;
    int i, max_left = m, max_right = m;
    for(i = m; i >= l; i--) {
        sum += data[i];
        if(sum > left_sum) {
            left_sum = sum;
            max_left = i;
        }
    }

    sum = 0;
    for(i = m + 1; i <= h; i++) {
        sum += data[i];
        if(sum > right_sum) {
            right_sum = sum;
            max_right = i;
        }
    }
    result->left = max_left;
    result->right = max_right;
    result->value = left_sum + right_sum;
}

void find_max_subarray(int l, int h, SEARCH_RESULT *result)
{
    if(l == h) {
        result->left = l;
        result->right = h;
        result->value = data[l];
        return;
    }
    int m = (l + h) / 2;
    SEARCH_RESULT left_result, right_result, cross_result;
    // search left to mid
    find_max_subarray(l, m, &left_result);
    // search mid+1 to right
    find_max_subarray(m+1, h, &right_result);
    // search cross mid result
    find_max_corssing_subarray(l, m, h, &cross_result);
    if(left_result.value >= right_result.value && left_result.value >= cross_result.value) {
        memcpy(result, &left_result, sizeof(SEARCH_RESULT));
    } else if(right_result.value >= left_result.value && right_result.value >= cross_result.value) {
        memcpy(result, &right_result, sizeof(SEARCH_RESULT));
    } else {
        memcpy(result, &cross_result, sizeof(SEARCH_RESULT));
    }
}

int main(int argc, const char *argv[])
{
    if(argc > 1) {
        if(freopen(argv[1], "r", stdin) == NULL) {
            fprintf(stderr, "Can not open input file\n");
            return 1;
        }
    } else {
        fprintf(stderr, "** %s input-file.txt to read file, using stdin now.\n", argv[0]);
    }

    int N;
    while(scanf("%d", &N) != EOF) {
        if(N <= 0)
            break;

        SEARCH_RESULT result;

        for(int32_t i = 0; i < N; i++)
            scanf("%d", &data[i]);

        find_max_subarray(0, N-1, &result);

        if(result.value < 0) {
            result.left = -1;
            result.right = -1;
            result.value = 0;
        }
        printf("%d %d %lld\n", result.left, result.right, result.value);
    }
    return 0;
}
