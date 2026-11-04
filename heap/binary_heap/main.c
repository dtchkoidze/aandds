#include "../../base/util.h"
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
    print_64_arr(bh->data, bh->count);

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

int main(void) {
    b_heap *b = &(b_heap){0};
    bh_init(b);

    bh_insert(b,4);
    bh_insert(b,10);
    bh_insert(b,2);
    bh_insert(b,11);
    bh_insert(b,5);
    bh_insert(b,13);
    bh_insert(b,254);
    bh_insert(b,15);
    bh_print(b);

    return 0;
}
