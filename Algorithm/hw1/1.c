#include <stdint.h>
#include <stdio.h>
#include <string.h>

int data[100000];

// 用來回傳結果的 structure
typedef struct SEARCH_RESULT_ {
    int left;
    int right;
    int64_t value;
} SEARCH_RESULT;

// 實作投影片上的程式
void find_max_corssing_subarray(int low, int mid, int high, SEARCH_RESULT *result)
{
    int64_t left_sum = INT64_MIN, right_sum = INT64_MIN, sum = 0;
    int i, max_left = mid, max_right = mid;

    // search left part
    for(i = mid; i >= low; i--) {
        sum += data[i];
        if(sum > left_sum) {
            left_sum = sum;
            max_left = i;
        }
    }

    // search right part
    sum = 0;
    for(i = mid + 1; i <= high; i++) {
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

void find_max_subarray(int low, int high, SEARCH_RESULT *result)
{
    // 只剩一個的時候，最大值就是自己
    if(low == high) {
        result->left = low;
        result->right = high;
        result->value = data[low];
        return;
    }

    int mid = (low + high) / 2;
    SEARCH_RESULT left_result, right_result, cross_result;

    // search left to mid
    find_max_subarray(low, mid, &left_result);
    // search mid+1 to right
    find_max_subarray(mid+1, high, &right_result);
    // search cross mid result
    find_max_corssing_subarray(low, mid, high, &cross_result);

    // return best result
    if(left_result.value >= right_result.value && left_result.value >= cross_result.value) {
        *result = left_result;
    } else if(right_result.value >= left_result.value && right_result.value >= cross_result.value) {
        *result = right_result;
    } else {
        *result = cross_result;
    }
}

int main(int argc, const char *argv[])
{
    if(argc > 1) {
        // 如果有提供執行參數
        if(freopen(argv[1], "r", stdin) == NULL) {
            // 如果檔案開啟失敗
            fprintf(stderr, "Can not open input file\n");
            return 1;
        }
    } else {
        // 如果沒有提供讀檔參數，則顯示提示訊息至 stderr
        fprintf(stderr, "** %s input-file.txt to read file, using stdin now.\n", argv[0]);
    }

    int N;
    while(scanf("%d", &N) != EOF) {
        if(N <= 0)
            break;

        SEARCH_RESULT result;

        // 依序讀入所有資料
        for(int32_t i = 0; i < N; i++)
            scanf("%d", &data[i]);

        find_max_subarray(0, N-1, &result);

        // 如果結果小於零，最佳解是全不取，等於零
        if(result.value < 0) {
            result.left = -1;
            result.right = -1;
            result.value = 0;
        }
        printf("%d %d %lld\n", result.left, result.right, result.value);
    }
    return 0;
}

// vim: sw=4
