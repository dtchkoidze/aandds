#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
//max size of actual set elements
#define MAX_N 1000000
//max value that set elem can have
#define MAX_M 1000000


typedef struct sparse_set {
    uint64_t dense[MAX_N];
    uint64_t sparse[MAX_M];
    uint64_t n;
    uint64_t iter_idx;
}  sparse_set;

void ss_iter_clear(sparse_set* ss) {
    ss->iter_idx = 0;
}

void ss_clear(sparse_set* ss) {
    ss->n = 0;
    ss->iter_idx = 0;
}

bool ss_is_member(sparse_set* ss, uint64_t i) {
    return ss->sparse[i] < ss->n && ss->dense[ss->sparse[i]] == i;
}

bool ss_add_member(sparse_set* ss, uint64_t i) {
    assert(i < MAX_M);

    if (ss_is_member(ss, i)) {
        return false;
    }

    assert(ss->n < MAX_N);

    ss->dense[ss->n] = i;
    ss->sparse[i] = ss->n;
    ss->n++;
    return true;
}


bool ss_next(sparse_set* ss, uint64_t* val) {
    if (ss->iter_idx >= ss->n) {
        return false;
    }
    *val = ss->dense[ss->iter_idx++];
    return true;
}

// https://research.swtch.com/sparse
int main(void) {
    static sparse_set sss = {0};
    sparse_set* ss = &sss;

    ss_clear(ss);
    for (int i = 0; i < MAX_M; i++) {
        assert(ss_add_member(ss, i));
        assert(ss_is_member(ss, i));
    }

    uint64_t val = 0;
    uint64_t expected_val = 0;
    while (ss_next(ss, &val)) {
        assert(val == expected_val);
        expected_val++;
    }
    assert(expected_val == ss->n);

    return 0;
}
