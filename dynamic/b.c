#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

static void bs() {
    uint64_t s = 0;
    for (int i = 0; i < UINT64_MAX; i++) {
        if ((i & 1) == 0) {
            s += i;
        }
    }
    printf("s=%" PRIu64 "\n", s);
}
