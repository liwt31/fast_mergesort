/* Adopted from Christopher Swenson (https://github.com/swenson/sort) in MIT license */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifndef SORT_NAME
#error "Must declare SORT_NAME"
#endif

#ifndef SORT_TYPE
#error "Must declare SORT_TYPE"
#endif

#ifndef SORT_SWAP
#define SORT_SWAP(x,y) {SORT_TYPE _sort_swap_temp = (x); (x) = (y); (y) = _sort_swap_temp;}
#endif

#define SORT_CONCAT(x, y) x ## _ ## y
#define SORT_MAKE_STR1(x, y) SORT_CONCAT(x,y)
#define SORT_MAKE_STR(x) SORT_MAKE_STR1(SORT_NAME,x)

#define INSERTION_SORT                 SORT_MAKE_STR(insertion_sort)
#define QUICK_SORT                     SORT_MAKE_STR(quick_sort)
#define MERGE_SORT                     SORT_MAKE_STR(merge_sort)
#define MERGE_SORT_REC                 SORT_MAKE_STR(merge_sort_rec)

/* minimal insertion threshold */
#ifndef MIN_INSERTION
#define MIN_INSERTION 16
#endif


void INSERTION_SORT(SORT_TYPE *dst, const size_t size);
void QUICK_SORT(SORT_TYPE *dst, const size_t size);
void MERGE_SORT(SORT_TYPE *dst, const size_t size);


void INSERTION_SORT(SORT_TYPE *dst, const size_t size) {
    SORT_TYPE *pl, *pr, *pi, *pj, v;
    pl = dst;
    pr = pl + size;

    for (pi = pl + 1; pi < pr; ++pi) {
        v = *pi;
        pj = pi;

        while ((v < * (pj - 1)) && pl < pj) {
            *pj = *(pj - 1);
            --pj;
        }

        *pj = v;
    }
}


void QUICK_SORT(SORT_TYPE *dst, const size_t size) {
    SORT_TYPE *pl, *pr, *pm, pivot;

    if (size < MIN_INSERTION) {
        INSERTION_SORT(dst, size);
        return;
    }

    pl = dst;
    pr = pl + size;
    pm = pl + ((pr - pl) >> 1);
    --pr;

    /* median of 3 */
    if (*pm < *pl) {
        SORT_SWAP(*pm, *pl);
    }

    if (*pr < *pm) {
        SORT_SWAP(*pr, *pm);
    }

    if (*pm < *pl) {
        SORT_SWAP(*pm, *pl);
    }

    SORT_SWAP(*pl, *pm);
    pivot = *pl;

    for (;;) {
        do { --pr; }
        while (pivot < *pr);

        if (pr <= pl) {
            break;
        }

        SORT_SWAP(*pl, *pr);

        do { ++pl; }
        while (*pl < pivot);

        if (pr <= pl) {
            break;
        }

        SORT_SWAP(*pl, *pr);
    }

    *pl = pivot;
    QUICK_SORT(dst, pl - dst);
    QUICK_SORT(pl + 1, dst + size - pl - 1);
}


void MERGE_SORT_REC(SORT_TYPE *dst, const size_t size, SORT_TYPE *pw) {
    SORT_TYPE *pl, *pr, *pm;

    if (size < MIN_INSERTION) {
        INSERTION_SORT(dst, size);
        return;
    }

    pl = dst;
    pr = pl + size;
    pm = pl + ((pr - pl) >> 1);
    MERGE_SORT_REC(pl, pm - pl, pw);
    MERGE_SORT_REC(pm, pr - pm, pw);
    memcpy(pw, pl, (pm - pl) * sizeof(SORT_TYPE));

    while (pm < pr && pl < pm) {
        if (*pw <= *pm) {
            *pl++ = *pw++;
        } else {
            *pl++ = *pm++;
        }
    }

    if (pl < pm) {
        memcpy(pl, pw, (pm - pl)*sizeof(SORT_TYPE));
    }

    return;
}

void MERGE_SORT(SORT_TYPE *dst, const size_t size) {
    SORT_TYPE *pw;

    pw = (SORT_TYPE *) malloc(size * sizeof(SORT_TYPE));
    MERGE_SORT_REC(dst, size, pw);
    free(pw);
}
