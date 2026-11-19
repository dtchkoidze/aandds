#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>


// assuming perfectly balanced tree that
typedef struct segment_tree {
    uint64_t* data;
    size_t leaf_start;
    size_t len;
    size_t cap;
} segment_tree;

void st_print(segment_tree* st) {
    if (st->len == 0) {
        printf("(empty tree)\n");
        return;
    }

    printf("\n");
    size_t levels = 0;
    size_t n = st->len;

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

        for (size_t j = 0; j < nodes_in_level && index < st->cap; j++) {
            printf("%2" PRIu64, st->data[index++]);

            if (j + 1 < nodes_in_level && index < st->cap)
                printf("%*s", (int)(gap * 2), "");
        }

        printf("\n\n");
        nodes_in_level *= 2;
    }
}


// left child 2i+1 right child 2i+2
// parent
bool st_init(segment_tree* st, uint64_t* a, size_t len) {
    assert((len & (len - 1)) == 0);
    size_t required_cap = 2*len-1;
    st->data = malloc(required_cap*sizeof(uint64_t));
    if (st->data == NULL) {
        return false;
    }
    st->cap = required_cap;
    size_t leaf_start = required_cap / 2;
    st->leaf_start = leaf_start;
    memcpy(st->data + leaf_start, a, len * sizeof(*st->data));
    st->len = len;
    for (int i = leaf_start-1; i >= 0; i--) {
        st->data[i] = st->data[2*i+1] + st->data[2*i+2];
    }

    return true;
}

bool st_min_init(segment_tree* st, uint64_t* a, size_t len) {//this one maintains that each parent = min(lchild, rchild)
    assert((len & (len - 1)) == 0);
    size_t required_cap = 2*len-1;
    st->data = malloc(required_cap*sizeof(uint64_t));
    if (st->data == NULL) {
        return false;
    }
    st->cap = required_cap;
    size_t leaf_start = required_cap / 2;
    st->leaf_start = leaf_start;
    memcpy(st->data + leaf_start, a, len * sizeof(*st->data));
    st->len = len;
    for (int i = leaf_start-1; i >= 0; i--) {
        uint64_t lchild = st->data[2*i+1];
        uint64_t rchild = st->data[2*i+2];
        st->data[i] = lchild < rchild ? lchild : rchild;
    }

    return true;
}

bool st_min_set(segment_tree* st, size_t i, uint64_t v) {
    if (i >= st->len) {
        return false;
    }
    size_t real_idx = st->leaf_start + i;
    if (real_idx >= st->cap) {
        return false;
    }

    st->data[real_idx] = v;

    // parent = i-1/2 soo
    while (real_idx > 0) {
        real_idx = (real_idx - 1) / 2;
        uint64_t lchild = st->data[2*real_idx+1];
        uint64_t rchild = st->data[2*real_idx+2];
        st->data[real_idx] = lchild < rchild ? lchild : rchild;
    }


    return true;
}

// logn
bool st_set(segment_tree* st, size_t i, uint64_t v) {
    if (i >= st->len) {
        return false;
    }
    size_t real_idx = st->leaf_start + i;
    if (real_idx >= st->cap) {
        return false;
    }

    st->data[real_idx] = v;

    // parent = i-1/2 soo
    while (real_idx > 0) {
        real_idx = (real_idx - 1) / 2;
        st->data[real_idx] = st->data[2*real_idx+1] + st->data[2*real_idx+2];
    }


    return true;
}


uint64_t st_sum(segment_tree* st, size_t l, size_t r) {
    assert(l <= r && r <= st->len);

    size_t leaf_base = st->leaf_start + 1;
    l += leaf_base;
    r += leaf_base;

    uint64_t sum = 0;
    while (l < r) {
        if (l & 1) {
            sum += st->data[l - 1];
            ++l;
        }
        if (r & 1) {
            --r;
            sum += st->data[r - 1];
        }

        l /= 2;
        r /= 2;
    }

    return sum;
}

int main(void) {
    uint64_t a[8] = {5,1,3,2,6,5,4,7};
    segment_tree st = {0};
    st_init(&st, a, 8);
    st_print(&st);
    st_set(&st, 2, 4);
    st_print(&st);
    assert(st_sum(&st, 0, 2) == 6);
    assert(st_sum(&st, 1, 3) == 5);

    segment_tree st_min = {0};
    st_min_init(&st_min, a, 8);
    st_print(&st_min);



    return 0;
}
