#include "../base/util.h"
#include <stddef.h>
#include <stdio.h>
#include <inttypes.h>

i64 binary_search(i64 n, i64* h, size_t len) {
    if (len == 0) {
        return -1;
    }

    i64 mid = len/2;
    debugf("h starts at: %zu, and up to +size: %lu\n", *h, len);
    if (h[mid] == n) {
        debugf("found it: %lu %lu \n", h[mid], n);
        return mid;
    }

    if (h[mid] < n) {
        // we know we only gotta search in right part
        debugf("is less\n");
        i64 rel_idx =  binary_search(n, h + mid + 1, len - mid - 1); //mid + 1 to account for the dragging of idx
        return rel_idx == -1  ? -1 : mid + 1 + rel_idx;
    } else {
        debugf("is more\n");
        return binary_search(n, h, mid);
    }

    return -1;
}


int main(void) {
    i64 a[9] = {1, 34, 53, 66, 78, 88, 89, 104, 146};
    size_t len = sizeof(a) / sizeof(i64);
    i64 x = 89;
    size_t idx = binary_search(x, a, len);
    printf("idx of x=%lu\n", idx);

    x = 1;
    idx = binary_search(x, a, len);
    printf("idx of x=%lu\n", idx);

    x = 17;
    idx = binary_search(x, a, len);
    printf("idx of x=%" PRId64 "\n", idx);

    return 0;
}
