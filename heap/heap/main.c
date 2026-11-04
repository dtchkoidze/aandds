#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdint.h>

typedef int (*heap_cmp)(const void *, const void *);
typedef void (*print_func)(const void *);

typedef struct simple_heap {
    size_t count;
    size_t cap;
    size_t elem_size;
    void *data;
    heap_cmp comp_func;
    print_func print_func;
} simple_heap;

bool sh_init(simple_heap *sh, size_t elem_size, heap_cmp comp_func, print_func print_func) {
    if (!sh || !elem_size || !comp_func) {
        return false;
    }

    sh->count = 0;
    sh->cap = 0;
    sh->elem_size = elem_size;
    sh->data = NULL;
    sh->comp_func = comp_func;
    sh->print_func = print_func;
    return true;
}

static void *sh_at(simple_heap *sh, size_t i) {
    return (char *)sh->data + i * sh->elem_size;
}

static void sh_swap(simple_heap *sh, size_t a, size_t b, void *tmp) {
    if (a == b) return;

    memcpy(tmp, sh_at(sh, a), sh->elem_size);
    memcpy(sh_at(sh, a), sh_at(sh, b), sh->elem_size);
    memcpy(sh_at(sh, b), tmp, sh->elem_size);
}

bool sh_insert(simple_heap *sh, const void *el) {
    if (!sh || !el) return false;

    if (sh->count == sh->cap) {
        size_t max_cap = SIZE_MAX / sh->elem_size;
        if (sh->cap >= max_cap) return false;

        size_t new_cap = sh->cap
            ? (sh->cap > max_cap / 2 ? max_cap : sh->cap * 2)
            : (max_cap < 8 ? max_cap : 8);

        void *tmp = realloc(sh->data, new_cap * sh->elem_size);
        if (!tmp) {
            perror("realloc");
            return false;
        }

        sh->data = tmp;
        sh->cap = new_cap;
    }

    memcpy(sh_at(sh, sh->count), el, sh->elem_size);

    size_t i = sh->count++;
    void *tmp = malloc(sh->elem_size);
    if (!tmp) {
        sh->count--;
        return false;
    }

    while (i > 0) {
        size_t parent = (i - 1) / 2;

        if (sh->comp_func(sh_at(sh, i), sh_at(sh, parent)) >= 0)
            break;

        sh_swap(sh, i, parent, tmp);
        i = parent;
    }

    free(tmp);
    return true;
}

bool sh_remove_min(simple_heap *sh, void *out) {
    if (!sh || sh->count == 0) return false;
    void *tmp = malloc(sh->elem_size);
    if (!tmp) return false;
    if (out) memcpy(out, sh_at(sh, 0), sh->elem_size);
    sh->count--;

    if (sh->count == 0) {
        free(tmp);
        return true;
    }

    memcpy(sh_at(sh, 0), sh_at(sh, sh->count), sh->elem_size);
    size_t i = 0;

    while (i < sh->count / 2) {
        size_t left = 2 * i + 1;
        size_t right = left + 1;
        size_t smallest = left;

        if (right < sh->count &&
            sh->comp_func(sh_at(sh, right), sh_at(sh, left)) < 0) {
            smallest = right;
        }

        if (sh->comp_func(sh_at(sh, i), sh_at(sh, smallest)) <= 0)
            break;

        sh_swap(sh, i, smallest, tmp);
        i = smallest;
    }

    free(tmp);
    return true;
}

void sh_print(simple_heap *sh) {
    if (!sh || !sh->print_func) return;

    printf("count=%zu, capacity=%zu\n", sh->count, sh->cap);

    for (size_t i = 0; i < sh->count; i++) {
        sh->print_func(sh_at(sh, i));
    }

    printf("\n");
}

void sh_destroy(simple_heap *sh) {
    if (!sh) return;

    free(sh->data);
    sh->data = NULL;
    sh->count = 0;
    sh->cap = 0;
}

void print_i64(const void *el) {
    printf("%" PRId64 " ", *(const int64_t *)el);
}

int cmp_i64(const void *a, const void *b) {
    int64_t x = *(const int64_t *)a;
    int64_t y = *(const int64_t *)b;
    return (x > y) - (x < y);
}

int main(void) {
    simple_heap sh = {0};

    if (!sh_init(&sh, sizeof(int64_t), cmp_i64, print_i64)) {
        return EXIT_FAILURE;
    }

    int64_t values[] = {114, 122, 3, 8, 2, 36, 17, 1, 84};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        if (!sh_insert(&sh, &values[i])) {
            sh_destroy(&sh);
            return EXIT_FAILURE;
        }
    }

    sh_print(&sh);
    printf("Extracting in sorted order:\n");
    int64_t min;
    while (sh_remove_min(&sh, &min)) {
        printf("min %" PRId64 " ", min);
    }
    printf("\n");

    sh_destroy(&sh);
    return 0;
}
