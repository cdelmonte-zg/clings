// ub1.c - Solution
//
// The fix: check for overflow BEFORE performing the addition.
// Compare against INT_MAX and INT_MIN limits.

#include <stdio.h>
#include <limits.h>

int safe_add(int a, int b, int *result) {
    if (b > 0 && a > INT_MAX - b) {
        return -1;
    }
    if (b < 0 && a < INT_MIN - b) {
        return -1;
    }
    *result = a + b;
    return 0;
}

#ifndef TEST
int main(void) {
    int result;

    if (safe_add(INT_MAX, 1, &result) != 0) {
        printf("Overflow detected! INT_MAX + 1 would overflow.\n");
    } else {
        printf("Result: %d\n", result);
    }

    if (safe_add(100, 200, &result) == 0) {
        printf("100 + 200 = %d\n", result);
    }

    return 0;
}
#else
#include "clings_test.h"

TEST(test_normal_add) {
    int result;
    ASSERT_EQ(safe_add(1, 2, &result), 0);
    ASSERT_EQ(result, 3);
}

TEST(test_negative_add) {
    int result;
    ASSERT_EQ(safe_add(-5, -3, &result), 0);
    ASSERT_EQ(result, -8);
}

TEST(test_zero) {
    int result;
    ASSERT_EQ(safe_add(0, 0, &result), 0);
    ASSERT_EQ(result, 0);
}

TEST(test_overflow_detected) {
    int result;
    ASSERT_NE(safe_add(INT_MAX, 1, &result), 0);
    ASSERT_NE(safe_add(INT_MAX, INT_MAX, &result), 0);
}

TEST(test_underflow_detected) {
    int result;
    ASSERT_NE(safe_add(INT_MIN, -1, &result), 0);
    ASSERT_NE(safe_add(INT_MIN, INT_MIN, &result), 0);
}

TEST(test_edge_cases_no_overflow) {
    int result;
    ASSERT_EQ(safe_add(INT_MAX, 0, &result), 0);
    ASSERT_EQ(result, INT_MAX);
    ASSERT_EQ(safe_add(INT_MIN, 0, &result), 0);
    ASSERT_EQ(result, INT_MIN);
    ASSERT_EQ(safe_add(INT_MAX, -1, &result), 0);
    ASSERT_EQ(result, INT_MAX - 1);
}

int main(void) {
    RUN_TEST(test_normal_add);
    RUN_TEST(test_negative_add);
    RUN_TEST(test_zero);
    RUN_TEST(test_overflow_detected);
    RUN_TEST(test_underflow_detected);
    RUN_TEST(test_edge_cases_no_overflow);
    TEST_REPORT();
}
#endif
