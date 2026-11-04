#include "../base/util.h"
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// in best case it's O(n) that is if its sorted already
// worst case will be O(n2) that is desc sorted
// hence algo is O(n2)
void insertion_sort(int64_t *input, size_t len) {
    for (int i = 0; i < len; i++) { // go from left to right
        debugf("number at: %d=%lu\n", i, input[i]);
        int j = i; // say u at 42, take idx of 42 that is 1 and start walking backwards below
        while (j > 0 && input[j] < input[j-1]) { // walk backwards until idx 1, comparing curr idx val to prev and if prev (left val) if more than curr, swap
            debugf("swapping input[j]: %lu, and input[j-1]: %lu\n", input[j], input[j-1]);
            input[j] ^= input[j-1];
            input[j-1] ^= input[j];
            input[j] ^= input[j-1];
            debugf("swapped input[j]: %lu, and input[j-1]: %lu\n", input[j], input[j-1]);
            j--; // to go left eg. j is 4 at val:400 when starting, then j-- gives u 11 then j-- 13, etc
        }
    }
}

// O(n+m)
void merge(int64_t *a, int64_t *b, size_t a_len, size_t b_len, int64_t *c) {
    // merge: a,b two sorted arrays, output bigass c which is fully sorted
    #ifdef DEBUG
        printf("input array a:");
        print_64_arr(a, a_len);
        printf("input array b:");
        print_64_arr(b, b_len);
    #endif
    size_t c_len = a_len + b_len;
    size_t i = 0; size_t j = 0;
    for (size_t k = 0; k < c_len; k++) {
        if (i == a_len) {
            c[k] = b[j];
            j++;
            continue;
        }

        if (j == b_len) {
            c[k] = a[i];
            i++;
            continue;
        }

        if (a[i] <= b[j]) {
            c[k] = a[i];
            i++;
        } else {
            c[k] = b[j];
            j++;
        }
    }
}

void merge_sort(int64_t *input, int64_t *scratch, size_t len) {
    if (len < 2 ) {
        return;
    }
    size_t midpoint = len / 2;

    merge_sort(input, scratch, midpoint);
    merge_sort(input + midpoint, scratch, len - midpoint);

    merge(input, input + midpoint, midpoint, len - midpoint, scratch);
    memcpy(input, scratch, len * sizeof(*input));
}

int main(void) {
    int64_t a[9] = {1, 42, 13, 11, 400, 42, 33, 5, 3};
    size_t a_len = sizeof(a) / sizeof(int64_t);
    insertion_sort(a, a_len);
    print_64_arr(a, a_len);
    // at this point a is sorted, lets write up b
    int64_t b[9] = {114, 122, 3, 8, 2, 36, 17, 1, 84};
    size_t b_len = sizeof(b) / sizeof(int64_t);
    int64_t *scratch = calloc(b_len, sizeof(int64_t));
    if (scratch == NULL) {
        perror("calloc");
        return 1;
    }
    merge_sort(b, scratch, b_len);
    print_64_arr(b, b_len);
    return 0;
}
