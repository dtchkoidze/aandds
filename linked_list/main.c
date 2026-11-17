#include "../base/util.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// ram model is when accessing stuff with indexing in backing array
// pointer machine is just ptr following


typedef struct ll_node ll_node;
typedef struct ll_node {
    ll_node* next;
    void* data;
} ll_node;

typedef struct ll {
    size_t elem_size;
    ll_node *first;
} ll;



bool ll_remove(ll* ll, void* x) {
    ll_node **curr = &ll->first; //ptr to ptr to first node
    while (*curr != NULL) {
        if (memcmp((*curr)->data, x, ll->elem_size) == 0)  { //shit to remove
            ll_node *removing = *curr;
            *curr = removing->next;
            free(removing->data);
            free(removing);
            return true;
         }

        curr = &(*curr)->next;
    }
    return false;
}

bool ll_add_after(ll* ll, void* x, void* y) {
    ll_node* y_node = malloc(sizeof(*y_node));
    if (y_node == NULL) {
        perror("malloc");
        return false;
    }

    *y_node = (ll_node){
        .data = y,
    };

    y_node->data = malloc(ll->elem_size);
    if (!y_node->data) {
        free(y_node);
        return false;
    }

    if (ll->first == NULL) {
        ll->first = y_node;
        return true;
    }

    ll_node *next = ll->first;

    while (next != NULL) {
        if (memcmp(next->data, x, ll->elem_size) == 0) {

            y_node->next = next->next;
            next->next = y_node;

            return true;
        }

        next = next->next;
    }

    free(y_node);
    return false;
}

int main(void) {
    return 0;
}
