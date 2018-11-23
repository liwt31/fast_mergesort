/* Adopted from Christopher Swenson (https://github.com/swenson/sort) in MIT license */

#include <string.h>
#include <stdint.h>


/* minimal insertion threshold */
#ifndef MIN_INSERTION
#define MIN_INSERTION 16
#endif


void asm_int64_insertion_sort(int64_t *dst, const size_t size);
void asm_int64_merge_sort(int64_t *dst, const size_t size);


void asm_int64_insertion_sort(int64_t *dst, const size_t size) {
    int64_t *pl, *pr, *pi, *pj, v;
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


void asm_int64_merge_sort_rec(int64_t *dst, const size_t size, int64_t *pw) {
    int64_t *pl, *pr, *pm;

    if (size < 16) {
        asm_int64_insertion_sort(dst, size);
        return;
    }

    pl = dst;
    pr = pl + size;
    pm = pl + ((pr - pl) >> 1);
    asm_int64_merge_sort_rec(pl, pm - pl, pw);
    asm_int64_merge_sort_rec(pm, pr - pm, pw);
    memcpy(pw, pl, (pm - pl) * sizeof(int64_t));
    asm (
        "%=:\n\t"
        "cmp    %1,%0\n\t"
        "jae    %=f\n\t"
        "mov    (%3),%%rax\n\t"
        "mov    (%1),%%rdx\n\t"
        "lea    0x8(%3),%%rsi\n\t"
        "lea    0x8(%1),%%rcx\n\t"
        "cmp    %%rdx,%%rax\n\t"
        "cmovle %%rsi,%3\n\t"
        "add    $0x8,%0\n\t"
        "cmp    %%rdx,%%rax\n\t"
        "cmovg  %%rcx,%1\n\t"
        "cmovg  %%rdx,%%rax\n\t"
        "cmp    %1,%2\n\t"
        "mov    %%rax,-0x8(%0)\n\t"
        "ja     %=b\n\t"
        "%=:\n\t"
        : "+r"(pl), "+r"(pm), "+r"(pr), "+r"(pw)
        : /* no input registers */
        : "%rax", "%rdx", "%rsi", "%rcx", "memory"
    );

    if (pl < pm) {
        memcpy(pl, pw, (pm - pl)*sizeof(int64_t));
    }

    return;
}

void asm_int64_merge_sort(int64_t *dst, const size_t size) {
    int64_t *pw;
    pw = (int64_t *) malloc(size * sizeof(int64_t));
    asm_int64_merge_sort_rec(dst, size, pw);
    free(pw);
}
