#include "../../base/util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// heap property: each node <= then child
// so y -> -inf v -> +inf
typedef struct b_heap b_heap;

typedef struct b_heap {
    i64 count;
    i64 cap;
    i64* data;
} b_heap;

// left child: 2i+1
// right child 2i+2
// parent round(i-1/2)

void bh_init(b_heap* bh) {
    if (bh == NULL) {
        printf("wtf did u pass me?\n");
        return;
    }
    bh->cap = 0;
    bh->count = 0;
    bh->data = NULL;
}


void bh_print(const b_heap *bh) {
    if (bh->count == 0) {
        printf("(empty heap)\n");
        return;
    }

    printf("\n");
    size_t levels = 0;
    size_t n = bh->count;

    while (n > 0) {
        levels++;
        n >>= 1;
    }

    size_t index = 0;
    size_t nodes_in_level = 1;

    for (size_t level = 0; level < levels; level++) {
        size_t indent = ((size_t)1 << (levels - level - 1)) - 1;
        size_t gap = ((size_t)1 << (levels - level)) - 1;

        printf("%*s", (int)(indent * 2), "");

        for (size_t j = 0; j < nodes_in_level && index < bh->count; j++) {
            printf("%2" PRId64, bh->data[index++]);

            if (j + 1 < nodes_in_level && index < bh->count)
                printf("%*s", (int)(gap * 2), "");
        }

        printf("\n");
        nodes_in_level *= 2;
    }
}

bool bh_insert(b_heap* bh, i64 n) {
    debugf("inserting %lu\n", n);

    if (bh->cap == bh->count) {
        size_t new_cap = bh->cap == 0 ? 1 : bh->cap * 2;
        i64 *tmp = realloc(bh->data, new_cap * sizeof(*bh->data));
        if (tmp == NULL) {
            perror("realloc");
            return false;
        }

        bh->data = tmp;
        bh->cap = new_cap;
    }

    size_t i = bh->count; //save before inc
    // just insert at the end
    bh->data[i] = n;
    bh->count++;
    // now that its inserted we gotta check if tree maintains heap property which is, eavery parent is <= child
    // swap child-parent untill heap property
    #ifdef DEBUG
    print_64_arr(bh->data, bh->count);
    #endif

    while (i > 0) {
        // ok lets check parent
        size_t parentidx = (i-1)/2;
        if (bh->data[parentidx] <= bh->data[i]) {
            break; //all cool, hp is maintained
        }

        debugf("parent is at %lu, val=%lu\n", parentidx, bh->data[parentidx]);
        if (bh->data[parentidx] > bh->data[i]) {
            debugf("parent is bigger then child: %lu, %lu, swapping...\n", bh->data[parentidx], bh->data[i]);
            bh->data[parentidx] ^= bh->data[i];
            bh->data[i] ^= bh->data[parentidx];
            bh->data[parentidx] ^= bh->data[i];
            debugf("parent is bigger then child: %lu, %lu, swapped\n", bh->data[parentidx], bh->data[i]);
        }

        i = parentidx;
    }

    return true;
}

i64 bh_remove_min(b_heap* bh) { //because of binary heap property min is always root e.g bh->data[0]
    if (bh->count <= 0) {
        return 0;
    }
    i64 removed = bh->data[0];
    bh->count--;
    if (bh->count == 0) {
        return removed;
    }
    bh->data[0] = bh->data[bh->count];


    size_t parent_idx = 0;
    while (2*parent_idx+1 < bh->count) {
        int left = 2*parent_idx+1;
        int right = 2*parent_idx+2;

        size_t smaller_child_idx = left; //initialize to this cuz we dont kno if there's enough count for right
        if (right < bh->count && bh->data[right] < bh->data[left]) {
            smaller_child_idx = right;
        }

        if (bh->data[parent_idx] <= bh->data[smaller_child_idx]) {//all cool
            break;
        }

        bh->data[parent_idx] ^= bh->data[smaller_child_idx];
        bh->data[smaller_child_idx] ^= bh->data[parent_idx];
        bh->data[parent_idx] ^= bh->data[smaller_child_idx];

        parent_idx = smaller_child_idx;
    }

    return removed;
}

void bh_empty(b_heap* bh) {
    bh->cap = 0;
    bh->count = 0;
    free(bh->data);
}

// nlogn
void bh_sort(i64* input, size_t len) {
    b_heap *b = &(b_heap){0};
    bh_init(b);
    for (int i = 0; i < len; i++) {
        bh_insert(b, input[i]);
    }

    for (int i = 0; i < len; i++) {
        input[i] = bh_remove_min(b);
    }

    bh_empty(b);
}

int main(void) {
    b_heap *b = &(b_heap){0};
    bh_init(b);
    for (i64 i = 1; i < 24; i++) {
        bh_insert(b, i);
    }
    for (i64 i = 1; i < 24; i++) {
        assert(bh_remove_min(b) == i);
    }
    int64_t a[9] = {1, 42, 13, 11, 400, 42, 33, 5, 3};
    size_t len = sizeof(a) / sizeof(int64_t);
    bh_sort(a, len);
    print_64_arr(a, len);
    return 0;
}
