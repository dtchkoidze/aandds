#include "../base/util.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <inttypes.h>

typedef enum bi_cond {
    gte,
    lte,
} bi_cond;

typedef bool(*pred_func)(i64 val, void *ctx);

// take any valid predicate, this is sort of a general case for which gte and lte or == are specific cases, monotonic predicate or whateva
// so for this input is something like a: ------------++++++++++++++++++++++++ where first + to the len-1 all are good or vice versa if !true_on_rightside
i64 binary_search_pred(i64* h, size_t len, pred_func pred, void *ctx, bool true_on_rightside) {
    i64 l = -1;
    i64 r = len;

    while (r > l+1) {
        i64 mid = (l+r)/2;
        bool result = pred(h[mid], ctx);
        if (result == true_on_rightside) {
            r = mid;
        } else {
            l = mid;
        }
    }

    return true_on_rightside ? r : l;
}

i64 binary_search_approx(i64* h, size_t len, i64 x, bi_cond cond) {//find closest element to lte or gte
    i64 l = -1; //such that h[l] < 0 - inf
    i64 r = len; //such that [r] >= 0 + inf
    // these indices are just imaginary to guarantee the properties for l and r
    while (r > l+1) {//untill they are side by side e.g
        //                    l <>  r
        // i64 a[9] = {1, 34, 53, 66, 78, 88, 89, 104, 146};
        i64 mid = (l+r)/2;
        debugf("midpoint=%" PRId64 " value=%" PRId64 "\n", mid, h[mid]);
        bool ifcond = (cond == gte)
                  ? h[mid] >= x
                  : h[mid] <= x;

        if (ifcond) {
            // bring right idx here
            if (cond == gte) r = mid; else l = mid;
        } else {
            // bring left idx here
            if (cond == gte) l = mid; else r = mid;
        }
    }

    if (cond == gte) {
      return r;
    }
    return l; //cuz at the end of iteration u'll have two nums say 1 and 2 and 1 will be < x and 2 >= x
}

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

i64 binary_search_lte(i64* h, size_t len, i64 x) {//find closest element such that n <= x
    i64 l = -1; //such that h[l] < 0 - inf
    i64 r = len; //such that [r] >= 0 + inf
    // these indices are just imaginary to guarantee the properties for l and r

    while (r > l+1) {//untill they are side by side e.g
        //                    l <>  r
        // i64 a[9] = {1, 34, 53, 66, 78, 88, 89, 104, 146};
        i64 mid = (l+r)/2;
        debugf("midpoint=%" PRId64 " value=%" PRId64 "\n", mid, h[mid]);
        if (h[mid] <= x) {
            // bring left idx here
            l = mid;
        } else {
            // bring right idx here
            r = mid;
        }
    }

    return l; //cuz at the end of iteration u'll have two nums say 1 and 2 and 1 will be < x and 2 >= x
}

i64 binary_search_gte(i64* h, size_t len, i64 x) {//find closest element such that n >= x
    i64 l = -1; //such that h[l] < 0 - inf
    i64 r = len; //such that [r] >= 0 + inf
    // these indices are just imaginary to guarantee the properties for l and r

    while (r > l+1) {//untill they are side by side e.g
        //                    l <>  r
        // i64 a[9] = {1, 34, 53, 66, 78, 88, 89, 104, 146};
        i64 mid = (l+r)/2;
        debugf("midpoint=%" PRId64 " value=%" PRId64 "\n", mid, h[mid]);
        if (h[mid] >= x) {
            // bring right idx here
            r = mid;
        } else {
            // bring left idx here
            l = mid;
        }
    }

    return r; //cuz at the end of iteration u'll have two nums say 1 and 2 and 1 will be < x and 2 >= x
}

bool is_gte(i64 value, void *ctx) {
    i64 x = *(i64 *)ctx;
    return value >= x;
}

bool is_lte(i64 value, void *ctx) {
    i64 x = *(i64 *)ctx;
    return value <= x;
}

//                                        MAX
//                                         ^
//                                         |
//                                         |
// got f(x) for which y:  0 <- +++++++++++++------------- ->0
// if u can check if at some points f(x) is increasing or decreasing u can do binary search, if u cant check then cant do bin saerch

typedef i64(*fnx) (i64 val);

i64 ternary_search(fnx fn, i64 l, i64 r) {
    // pick 2 middle points m1 and m2

    while (r - l > 2) {
        i64 m1 = l + (r - l) / 3;
        i64 m2 = r - (r - l) / 3;

        if (fn(m1) > fn(m2)) {//we know that in this range func was decreasing and m2 stands on the decreasing side
            r = m2;
        } else {
            l = m1;
        }
    }

    i64 best = l;

    for (i64 i = l + 1; i <= r; i++) {
        if (fn(i) < fn(best)) {
            best = i;
        }
    }

    return best;
}


int main(void) {
                    //  l   r
    i64 a[9] = {1, 34, 53, 66, 78, 88, 89, 104, 146};
    size_t len = sizeof(a) / sizeof(i64);
    i64 x = 89;
    size_t idx = binary_search(x, a, len);
    printf("idx of x=%lu\n", idx);

    x = 52;
    idx = binary_search_gte(a, len,  x); //should be 53 [<= to x]
    assert(idx == 2);
    printf("idx of leftmost num >= to x is=%" PRId64 "\n", idx);

    idx = binary_search_lte(a, len,  x); //should be 34 [<= to x]
    assert(idx == 1);
    printf("idx of leftmost num <= to x is=%" PRId64 "\n", idx);


    assert(binary_search_lte(a, len,  x) == binary_search_approx(a, len, x, lte));
    assert(binary_search_gte(a, len,  x) == binary_search_approx(a, len, x, gte));

    // not found case
    i64 b[9] = {1, 2, 3, 13, 15, 18, 23, 44, 50};
    idx = binary_search_gte(b, len,  x); //should be BULLSHIT? [<= to x]
    printf("idx of leftmost num >= to x is=%" PRId64 "\n", idx);


    debugf("-----------------------------------------------------------------\n");
    idx = binary_search_pred(a, len, is_gte, &x, true);
    debugf("idx = %" PRId64 "\n", idx);
    assert(idx == 2);

    idx = binary_search_pred(a, len, is_lte, &x, false);
    debugf("idx = %" PRId64 "\n", idx);
    assert(idx == 1);


    return 0;
}
