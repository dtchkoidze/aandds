#include <time.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../base/util.h"

static inline void swap_i64(i64 *a, i64 *b) {
    i64 tmp = *a;
    *a = *b;
    *b = tmp;
}

void merge_sort(i64* input, size_t len) {
    if (len <= 1) {
        return;
    }

    size_t pivot_idx = rand() % len;
    swap_i64(&input[pivot_idx], &input[len - 1]); //put pivot at the end
    i64 x = input[len-1]; //take that pivot
    size_t j = 0;
    printf("picked %lu at %lu\n", x, pivot_idx);
    for (int i = 0; i < len - 1; i++) {
        if (input[i] < x) {//if i[i] is less than x swap, can be done safely because we never touch pivot
            swap_i64(&input[i], &input[j]);
            j++;
        }
    }
    // once loop done all numbers BEFORE j are less then pivot so we can put pivot at j and put j at the and because it is guaranteed to be >= to pivot since it wasnt < pivot
    swap_i64(&input[j], &input[len - 1]);

    print_64_arr(input, len);
    merge_sort(input, j); //first half (nums that are < x)
    merge_sort(input + j + 1, len - j - 1); //nums that are >= x
}


int main(void) {
    srand(time(NULL));
    i64 a[11] = {1, 42, 13, 11, 11, 20, 42, 11, 33, 5, 3};
    i64 b[11] = {42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42};
    size_t len = sizeof(a) / sizeof(i64);
    merge_sort(a, len);
    merge_sort(b, len);
    print_64_arr(a, len);
    print_64_arr(b, len);
    return 0;
}
