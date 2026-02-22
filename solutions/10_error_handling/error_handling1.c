// error_handling1.c - Solution
//
// Fix: Check for division by zero (b == 0) and signed overflow
// (a == INT_MIN && b == -1) before performing the operation.
// Leave *result untouched on error.

#include <stdio.h>
#include <limits.h>

enum math_error {
    MATH_OK       =  0,
    MATH_DIV_ZERO = -1,
    MATH_OVERFLOW = -2
};

// Fixed: checks for div-by-zero and INT_MIN / -1 overflow
int safe_divide(int a, int b, int *result) {
    if (b == 0) {
        return MATH_DIV_ZERO;
    }
    if (a == INT_MIN && b == -1) {
        return MATH_OVERFLOW;
    }
    *result = a / b;
    return MATH_OK;
}

// Fixed: checks for mod-by-zero and INT_MIN % -1 overflow
int safe_modulo(int a, int b, int *result) {
    if (b == 0) {
        return MATH_DIV_ZERO;
    }
    if (a == INT_MIN && b == -1) {
        return MATH_OVERFLOW;
    }
    *result = a % b;
    return MATH_OK;
}

#ifndef TEST
int main(void) {
    int result;
    int err;

    err = safe_divide(10, 3, &result);
    printf("10 / 3 = %d (err=%d)\n", result, err);

    err = safe_divide(10, 0, &result);
    printf("10 / 0: err=%d\n", err);

    err = safe_modulo(10, 3, &result);
    printf("10 %% 3 = %d (err=%d)\n", result, err);

    return 0;
}
#else
#include "clings_test.h"

TEST(test_divide_basic) {
    int result;
    int err = safe_divide(10, 3, &result);
    ASSERT_EQ(err, MATH_OK);
    ASSERT_EQ(result, 3);
}

TEST(test_divide_exact) {
    int result;
    int err = safe_divide(21, 7, &result);
    ASSERT_EQ(err, MATH_OK);
    ASSERT_EQ(result, 3);
}

TEST(test_divide_negative) {
    int result;
    int err = safe_divide(-15, 5, &result);
    ASSERT_EQ(err, MATH_OK);
    ASSERT_EQ(result, -3);
}

TEST(test_divide_by_zero) {
    int result = 999;
    int err = safe_divide(10, 0, &result);
    ASSERT_EQ(err, MATH_DIV_ZERO);
    // result should be untouched
    ASSERT_EQ(result, 999);
}

TEST(test_divide_int_min_by_neg1) {
    int result = 999;
    int err = safe_divide(INT_MIN, -1, &result);
    ASSERT_EQ(err, MATH_OVERFLOW);
    // result should be untouched
    ASSERT_EQ(result, 999);
}

TEST(test_divide_zero_numerator) {
    int result;
    int err = safe_divide(0, 5, &result);
    ASSERT_EQ(err, MATH_OK);
    ASSERT_EQ(result, 0);
}

TEST(test_modulo_basic) {
    int result;
    int err = safe_modulo(10, 3, &result);
    ASSERT_EQ(err, MATH_OK);
    ASSERT_EQ(result, 1);
}

TEST(test_modulo_by_zero) {
    int result = 999;
    int err = safe_modulo(10, 0, &result);
    ASSERT_EQ(err, MATH_DIV_ZERO);
    ASSERT_EQ(result, 999);
}

TEST(test_modulo_int_min_by_neg1) {
    int result = 999;
    int err = safe_modulo(INT_MIN, -1, &result);
    ASSERT_EQ(err, MATH_OVERFLOW);
    ASSERT_EQ(result, 999);
}

TEST(test_modulo_no_remainder) {
    int result;
    int err = safe_modulo(12, 4, &result);
    ASSERT_EQ(err, MATH_OK);
    ASSERT_EQ(result, 0);
}

int main(void) {
    RUN_TEST(test_divide_basic);
    RUN_TEST(test_divide_exact);
    RUN_TEST(test_divide_negative);
    RUN_TEST(test_divide_by_zero);
    RUN_TEST(test_divide_int_min_by_neg1);
    RUN_TEST(test_divide_zero_numerator);
    RUN_TEST(test_modulo_basic);
    RUN_TEST(test_modulo_by_zero);
    RUN_TEST(test_modulo_int_min_by_neg1);
    RUN_TEST(test_modulo_no_remainder);
    TEST_REPORT();
}
#endif
