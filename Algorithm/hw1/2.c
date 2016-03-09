#include <stdio.h>
#include <stdint.h>

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

        for(int32_t i = 0; i < N; i++) {
            scanf("%d", &input_val);

            if(current_value + input_val > 0) {
                current_value += input_val;
                current_right = i;

                if(current_value > max_value) {
                    max_value = current_value;
                    max_left = current_left;
                    max_right = current_right;
                }
            } else {
                current_left = i + 1;
                current_right = i;
                current_value = 0;
            }
        }

        printf("%d %d %lld\n", max_left, max_right, max_value);
    }
    return 0;
}
