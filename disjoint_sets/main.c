#include "../base/util.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct dsetarr dsetarr;
typedef struct dsetarr {
    i64* data; //indexes are set elements and values are set special objects they belong to
    i64 count;
    i64 cap;
} dsetarr;

// O(1) constant
i64 dset_find(dsetarr* set, i64 x) {
    if (set->count == 0) {
        return -1;
    }
    return set->data[x];
}

// O(n) max n-1 OPS TOTAL WILL BE nsquare
bool dset_union(dsetarr* set, i64 x, i64 y) {
    i64 xrep = dset_find(set, x);
    i64 yrep = dset_find(set, y);
    if ((xrep == -1) || (yrep == -1)) {
        return false;
    }

    // choose 1 representative for both, this case i choose xrep for y too
    for (int i = 0; i < set->count - 1; i++) {
        if (set->data[i] == yrep) {
            set->data[i] = xrep;
        }
    }

    return true;
}

i64 dset_make(dsetarr* set) {
    if (set->count == set->cap) {
        i64 new_cap = set->cap ? set->cap * 2 : 8;

        i64 *new_data = realloc(
            set->data,
            (size_t)new_cap * sizeof(*set->data)
        );

        if (!new_data) return -1;

        set->data = new_data;
        set->cap = new_cap;
    }

    i64 x = set->count;
    set->data[x] = x;
    set->count++;

    return x;
}

int main(void) {


    return 0;
}
