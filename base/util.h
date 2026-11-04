#pragma once
#include <stdint.h>
#include <stdio.h>

#ifdef DEBUG
#define debugf(...) do {                                           \
    fprintf(stderr, "%s:%s:%d: ", __FILE__, __func__, __LINE__);  \
    fprintf(stderr, __VA_ARGS__);                                 \
    fflush(stderr);                                               \
} while (0)
#else
#define debugf(...) ((void)0)
#endif

#define swap(a,b)       \
do {                    \
        a ^= b;         \
        b ^= a;         \
        a^= b ;         \
} while (0)             \

#define print_64_arr(input, len)                                                                   \
    do {                                                                                           \
        printf("i64 arr:\t");                                                                      \
        for (int i = 0; i < len; i++) {                                                            \
            printf("%lu ", input[i]);                                                              \
        }                                                                                          \
        printf("\n");                                                                              \
    } while (0)

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a > b ? b : a)

// nums
#define i64 int64_t
