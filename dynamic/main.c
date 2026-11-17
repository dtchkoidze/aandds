#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>

#define REC_BOUND 1000000
#define MAX_RESULT 1000000

// exponential t complexity
// if n is 10 suppose, f8 as an example will be calculated twice by l and r rec calls and not only 8 ofc, so we do extra work for no reason
// f7 will be calced 3 times etc etc 6five times and so on
static uint64_t fib_nth(uint64_t n) {
    assert(n < 93);

    if (n <= 2) {
        return 1;
    }

    return fib_nth(n-1) + fib_nth(n-2);
}

static uint64_t fib_nth_saved(uint64_t n, uint64_t result[MAX_RESULT]) {
    assert(n < MAX_RESULT);

    // this is O(1) case
    if (result[n] != 0) {
        return result[n];
    }

    //everything below is O(n)
    // soo total time complexity of this bad boy is linear O(n)
    if (n <= 2) {
        result[n] = 1;
        return result[n];
    }

    result[n] = fib_nth_saved(n-1, result) + fib_nth_saved(n-2, result);
    return result[n];
}

static uint64_t fib_nth_proper_no_motherfucking_recursion_just_a_proper_fuckin_loop(uint64_t n) {
    uint64_t prev = 0;
    uint64_t fib = 1;
    for (uint64_t i = 0; i < n; i++) {
        uint64_t next = prev + fib;
        prev = fib;
        fib = next;
    }
    return prev;
}

static int factorial(int x) {
    int f = 1;
    for (int i = x; i > 0; i--) {
        f *= i;
    }
    return f;
}

static void fib_rec(int x, int y) {
    if (y > REC_BOUND) {
        return;
    }
    int z = x + y;
    printf("%d\n", z);

    return fib_rec(y, z);
}


// if files a.c and b.c
// add line  if ((i & 1) == 0) {
// add line    }
// static int diff() {
//     const char* a = "a.c";
//     const char* b = "b.c";
//     int fda, fdb;
//     fda = open(a, O_RDONLY);
//     if (fda == -1) {
//         perror("open");
//         return -1;
//     }
//     fdb = open(b, O_RDONLY);
//     if (fdb == -1) {
//         perror("open");
//         return -1;
//     }
//
//     off_t size_a = lseek(fda, 0, SEEK_END);
//     if (size_a == -1) {
//         return -1;
//     }
//
//     off_t size_b = lseek(fdb, 0, SEEK_END);
//     if (size_b == -1) {
//         return -1;
//     }
//
//     char* data_a = malloc(size_a);
//     if (data_a == NULL) {
//         perror("data_a malloc");
//         return -1;
//     }
//
//     char* data_b = malloc(size_b);
//     if (data_b == NULL) {
//         perror("data_b malloc");
//         return -1;
//     }
//
//     if (lseek(fda, 0, SEEK_SET) == -1) {
//         perror("set back fda");
//         return -1;
//     }
//
//     if (lseek(fdb, 0, SEEK_SET) == -1) {
//         perror("set back fdb");
//         return -1;
//     }
//
//     if (read(fda, data_a, size_a) == -1) {
//         perror("read a");
//         return -1;
//     }
//
//     if (read(fdb, data_b, size_b) == -1) {
//         perror("read b");
//         return -1;
//     }
//
//
//     return 0;
// }


int min_array(size_t count, const int *values) {
    assert(count > 0);

    int smallest = values[0];
    for (size_t i = 1; i < count; i++) {
        if (values[i] < smallest) {
            smallest = values[i];
        }
    }
    return smallest;
}

// to make b out a u either: ch letter or add letter or rm letter
// so min n of operations to go from a => b would be levenstein distance
int levenshtein(const char* a, const char* b) { //also called edit distance
    size_t size_a = strlen(a);
    size_t size_b = strlen(b);

    int d[size_a + 1][size_b + 1];

    for (int i = 0; i <= size_a; i++) {
        d[i][0] = i;
    }

    for (int j = 0; j <= size_b; j++) {
        d[0][j] = j;
    }


    // continue with smaller subset
    for (int i = 1; i <= size_a; i++) {
        for (int j = 1; j <= size_b; j++) {
            int substitution_cost = (a[i - 1] != b[j - 1]);
            d[i][j] = min_array(3, (int[]){
                d[i-1][j] + 1, //del
                d[i][j-1] + 1, //insert
                d[i-1][j-1] + substitution_cost, //match or sub
            });
        }
    }

    return d[size_a][size_b];
}


int main(void) {
    uint64_t result[MAX_RESULT];
    memset(&result, 0, MAX_RESULT);
    fib_rec(0, 1);
    printf("11th fib number: %" PRIu64 "\n", fib_nth_saved(11,result));
    memset(&result, 0, MAX_RESULT);
    printf("92th fib number: %" PRIu64 "\n", fib_nth_saved(92,result));

    memset(&result, 0, MAX_RESULT);
    assert(fib_nth_proper_no_motherfucking_recursion_just_a_proper_fuckin_loop(11) == fib_nth_saved(11, result));
    diff();
    int distance = levenshtein("apple", "alpine");
    return 0;
}
