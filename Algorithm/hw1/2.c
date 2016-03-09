#include <stdio.h>
#include <stdint.h>

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

    // 每筆測資第一個數字代表接下來有多少輸入
    int32_t N;
    while(scanf("%d", &N) != EOF) {
        if(N <= 0) break;

        // variables for input data
        int32_t input_val;

        // max value and its range
        int64_t max_value = 0;
        int32_t max_left = 0, max_right = -1;

        // current value and range
        int64_t current_value = 0;
        int32_t current_left = 0, current_right = -1;

        // O(N) solution!
        for(int32_t i = 0; i < N; i++) {
            scanf("%d", &input_val);

            // 如果往右累加小於零則清掉current values（else做的事情）
            if(current_value + input_val > 0) {
                // 累加 current values
                current_value += input_val;
                current_right = i;

                // 刷新紀錄
                if(current_value > max_value) {
                    max_value = current_value;
                    max_left = current_left;
                    max_right = current_right;
                }
            } else {
                // 清除 current values
                current_left = i + 1;
                current_right = i;
                current_value = 0;
            }
        }

        printf("%d %d %lld\n", max_left, max_right, max_value);
    }
    return 0;
}

// vim: sw=4
