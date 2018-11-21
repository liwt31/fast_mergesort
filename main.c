/* Adopted from Christopher Swenson (https://github.com/swenson/sort) in MIT license */

#include <sys/time.h>

#define SORT_NAME int64_type
#define SORT_TYPE int64_t
#include "sort.h"
#undef SORT_NAME
#undef SORT_TYPE

#define SORT_NAME float64_type
#define SORT_TYPE double
#include "sort.h"
#undef SORT_NAME
#undef SORT_TYPE

/* Used to control the test */
#define SEED 123
#define MAXSIZE 10000
#define TESTS 100

#define RAND_RANGE(__n, __min, __max) \
    (__n) = (__min) + (long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / (RAND_MAX + 1.0)))


static __inline double utime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (1000000.0 * t.tv_sec + t.tv_usec);
}

/* helper functions */
int verify_int(int64_t *dst, const int size) {
    int i;

    for (i = 1; i < size; i++) {
        if (dst[i - 1] > dst[i]) {
            printf("Verify failed! at %d", i);
            return 0;
        }
    }

    return 1;
}

int verify_double(double *dst, const int size) {
    int i;

    for (i = 1; i < size; i++) {
        if (dst[i - 1] > dst[i]) {
            printf("Verify failed! at %d", i);
            return 0;
        }
    }

    return 1;
}

static void fill_int(int64_t *dst, const int size) {
    int i;

    for (i = 0; i < size; i++) {
        dst[i] = lrand48();
    }
}

static void fill_double(double *dst, const int size) {
    int i;

    for (i = 0; i < size; i++) {
        dst[i] = drand48();
    }
}

#define TEST_SORT_INT(name) do { \
  res = 0; \
  diff = 0; \
  printf("%-29s", #name); \
  for (test = 0; test < sizes_cnt; test++) { \
    int64_t size = sizes[test]; \
    int64_t dst[MAXSIZE]; \
    fill_int(dst, size); \
    usec1 = utime(); \
    name(dst, size); \
    usec2 = utime(); \
    res = verify_int(dst, size); \
    if (!res) { \
      break; \
    } \
    diff += usec2 - usec1; \
  } \
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff); \
  if (!res) return 0; \
} while (0)

#define TEST_SORT_DOUBLE(name) do { \
  res = 0; \
  diff = 0; \
  printf("%-29s", #name); \
  for (test = 0; test < sizes_cnt; test++) { \
    int64_t size = sizes[test]; \
    double dst[MAXSIZE]; \
    fill_double(dst, size); \
    usec1 = utime(); \
    name(dst, size); \
    usec2 = utime(); \
    res = verify_double(dst, size); \
    if (!res) { \
      break; \
    } \
    diff += usec2 - usec1; \
  } \
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff); \
  if (!res) return 0; \
} while (0)

int run_tests(int64_t *sizes, int sizes_cnt) {
    int test, res;
    double usec1, usec2, diff;
    TEST_SORT_INT(int64_type_quick_sort);
    TEST_SORT_INT(int64_type_merge_sort);
    TEST_SORT_DOUBLE(float64_type_quick_sort);
    TEST_SORT_DOUBLE(float64_type_merge_sort);
    return 0;
}


int main(void) {
    int result, i = 0;
    int64_t sizes[TESTS];
    srand48(SEED);
    fill_int(sizes, TESTS);

    for (i = 0; i < TESTS; i++) {
        RAND_RANGE(sizes[i], 0, MAXSIZE);
    }

    result = run_tests(sizes, TESTS);

    if (result) {
        return 1;
    }

    return 0;
}
