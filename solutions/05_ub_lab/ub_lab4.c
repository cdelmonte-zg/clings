// ub_lab4.c - Solution
//
// Fix: before each multiplication, check if `fact > INT_MAX / i`.
// If so, the multiplication would overflow, so return -1.

#include <stdio.h>
#include <limits.h>

int factorial(int n, int *result) {
    if (n < 0) {
        return -1;
    }

    int fact = 1;
    for (int i = 2; i <= n; i++) {
        /* FIX: check for overflow before multiplying */
        if (fact > INT_MAX / i) {
            return -1;
        }
        fact *= i;
    }

    *result = fact;
    return 0;
}

#ifndef TEST
int main(void) {
    for (int i = 0; i <= 15; i++) {
        int result;
        if (factorial(i, &result) == 0) {
            printf("%2d! = %d\n", i, result);
        } else {
            printf("%2d! = OVERFLOW\n", i);
        }
    }
    return 0;
}
#else
#include "clings_test.h"

TEST(test_zero) {
    int result;
    ASSERT_EQ(factorial(0, &result), 0);
    ASSERT_EQ(result, 1);
}

TEST(test_one) {
    int result;
    ASSERT_EQ(factorial(1, &result), 0);
    ASSERT_EQ(result, 1);
}

TEST(test_small_values) {
    int result;
    ASSERT_EQ(factorial(5, &result), 0);
    ASSERT_EQ(result, 120);

    ASSERT_EQ(factorial(6, &result), 0);
    ASSERT_EQ(result, 720);
}

TEST(test_twelve) {
    int result;
    ASSERT_EQ(factorial(12, &result), 0);
    ASSERT_EQ(result, 479001600);
}

TEST(test_overflow_detected) {
    int result;
    ASSERT_EQ(factorial(13, &result), -1);
    ASSERT_EQ(factorial(20, &result), -1);
}

TEST(test_negative_input) {
    int result;
    ASSERT_EQ(factorial(-1, &result), -1);
}

int main(void) {
    RUN_TEST(test_zero);
    RUN_TEST(test_one);
    RUN_TEST(test_small_values);
    RUN_TEST(test_twelve);
    RUN_TEST(test_overflow_detected);
    RUN_TEST(test_negative_input);
    TEST_REPORT();
}
#endif
