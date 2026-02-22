/*
 * clings_test.h — Lightweight test framework for clings exercises
 *
 * Usage:
 *   #include "clings_test.h"
 *
 *   TEST(test_addition) {
 *       ASSERT_EQ(add(2, 3), 5);
 *       ASSERT_EQ(add(0, 0), 0);
 *   }
 *
 *   int main(void) {
 *       RUN_TEST(test_addition);
 *       TEST_REPORT();
 *   }
 */
#ifndef CLINGS_TEST_H
#define CLINGS_TEST_H

#include <stdio.h>
#include <string.h>

static int clings_tests_run    = 0;
static int clings_tests_passed = 0;
static int clings_tests_failed = 0;

/* Undefine TEST if it was set by -DTEST on the command line */
#ifdef TEST
#undef TEST
#endif

/* Define a test function */
#define TEST(name) static void name(void)

/* Run a test and track results */
#define RUN_TEST(name) do {                                         \
    clings_tests_run++;                                             \
    int _prev_failed = clings_tests_failed;                         \
    printf("  test %-40s ", #name);                                 \
    name();                                                         \
    if (clings_tests_failed == _prev_failed) {                      \
        clings_tests_passed++;                                      \
        printf("ok\n");                                             \
    }                                                               \
} while(0)

/* Basic assertion */
#define ASSERT(expr) do {                                           \
    if (!(expr)) {                                                  \
        printf("FAILED\n");                                         \
        printf("    assertion failed: %s\n", #expr);                \
        printf("    at %s:%d\n", __FILE__, __LINE__);               \
        clings_tests_failed++;                                      \
        return;                                                     \
    }                                                               \
} while(0)

/* Assert equality (integers / pointers) */
#define ASSERT_EQ(a, b) do {                                        \
    if ((a) != (b)) {                                               \
        printf("FAILED\n");                                         \
        printf("    expected: %s == %s\n", #a, #b);                 \
        printf("    at %s:%d\n", __FILE__, __LINE__);               \
        clings_tests_failed++;                                      \
        return;                                                     \
    }                                                               \
} while(0)

/* Assert string equality */
#define ASSERT_STR_EQ(a, b) do {                                    \
    if (strcmp((a), (b)) != 0) {                                    \
        printf("FAILED\n");                                         \
        printf("    expected: \"%s\" == \"%s\"\n", (a), (b));       \
        printf("    at %s:%d\n", __FILE__, __LINE__);               \
        clings_tests_failed++;                                      \
        return;                                                     \
    }                                                               \
} while(0)

/* Assert not equal */
#define ASSERT_NE(a, b) do {                                        \
    if ((a) == (b)) {                                               \
        printf("FAILED\n");                                         \
        printf("    expected: %s != %s\n", #a, #b);                 \
        printf("    at %s:%d\n", __FILE__, __LINE__);               \
        clings_tests_failed++;                                      \
        return;                                                     \
    }                                                               \
} while(0)

/* Print test summary and return appropriate exit code */
#define TEST_REPORT() do {                                          \
    printf("\n  %d tests, %d passed, %d failed\n",                  \
        clings_tests_run, clings_tests_passed, clings_tests_failed);\
    return clings_tests_failed > 0 ? 1 : 0;                        \
} while(0)

#endif /* CLINGS_TEST_H */
