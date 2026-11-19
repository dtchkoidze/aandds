#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// assuming perfectly balanced tree that
typedef struct segment_tree {
    uint64_t* data;
    size_t len;
    size_t cap;
} segment_tree;

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
    memcpy(st->data + leaf_start, a, len * sizeof(*st->data));
    st->len = len;
    for (int i = leaf_start-1; i >= 0; i--) {
        st->data[i] = st->data[2*i+1] + st->data[2*i+2];
    }

    return true;
}

void st_set(segment_tree* st, size_t i, uint64_t v) {

}

void st_sum(segment_tree* st, size_t l, size_t r) {

}

int main(void) {
    uint64_t a[8] = {5,1,3,2,6,5,4,7};
    segment_tree st = {0};
    st_init(&st, a, 8);

    return 0;
}
