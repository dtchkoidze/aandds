#include "../base/util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct stack {
    void *data;
    size_t elem_size;
    i64 count;
    i64 cap;
} stack;

typedef struct queue {
    void *data;
    size_t elem_size;
    i64 count;
    i64 cap;
    i64 head;
} queue;

static bool queue_add(queue *q, const void* elem) {
    if (q->cap == q->count) {
        const size_t new_cap = q->cap == 0 ? 1 : q->cap * 2;

        if (q->elem_size == 0 ||
            new_cap < q->cap ||
            new_cap > SIZE_MAX / q->elem_size) {
            return false;
        }

        void *tmp = malloc(new_cap * q->elem_size);
        if (tmp == NULL) {
            perror("malloc");
            return false;
        }

        for (size_t i = 0; i < q->count; i++) { //to normalize lay after realloc to allow modulo arithmetic
            const size_t old_idx = (q->head + i) % q->cap;

            memcpy(
                (char *)tmp + i * q->elem_size,
                (char *)q->data + old_idx * q->elem_size,
                q->elem_size
            );
        }

        free(q->data);
        q->data = tmp;
        q->cap = new_cap;
        q->head = 0;
    }
    size_t tail = (q->head + q->count) % q->cap;
    memcpy(
        (char *)q->data + tail * q->elem_size,
        elem,
        q->elem_size
    );
    q->count++;
    return true;
}

bool queue_rm(queue *q, void *out) {
    if (q->count == 0) {
        return false;
    }

    memcpy(
           out,
           (char *)q->data + q->head * q->elem_size,
           q->elem_size
    );

    q->head = (q->head + 1) % q->cap;
    q->count--;

    return false;
}

bool stack_push(stack *s, void *elem) {
    if (s->cap == s->count) {
        size_t new_cap = s->cap == 0 ? 1 : s->cap * 2;
        void *tmp = realloc(s->data, new_cap * s->elem_size);
        if (tmp == NULL) {
            perror("realloc");
            return false;
        }
        s->data = tmp;
        s->cap = new_cap;
    }
    memcpy(s->data + (s->count * s->elem_size), elem, s->elem_size);
    s->count++;
    return true;
}

static bool stack_pop(stack *s, void *out) {
    if (s->count == 0) {
        return false;
    }
    s->count--;
    memcpy(out, s->data + (s->count * s->elem_size), s->elem_size);
    return true;
}

int main(void) {
    stack *s = &(stack){
        .elem_size = sizeof(i64),
        // all others will be ZIed
    };
    i64 i = 0;
    while (stack_push(s, &i) && i < 100) {
        i++;
    }

    i64* scratch = malloc(1 * sizeof(i64));
    for (int j = i; j > 0; j--) {
        stack_pop(s, scratch);
        debugf("popped value = %" PRId64 "\n", *scratch);
        assert(*scratch == j);
    }

    queue *q = &(queue){
        .elem_size = sizeof(i64),
    };

    while(queue_add(q, &i) && i < 200) {
        i++;
    }

    i64 *scratchforq = malloc(1 * sizeof(i64));
    for (int k = 100; k <= 200; k++) {
        queue_rm(q, scratchforq);
        debugf("dequeued val=%" PRId64 "\n", *scratchforq);
        assert(*scratchforq == k);
    }

    return 0;
}
