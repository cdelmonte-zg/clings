// strings3.c - Solution
//
// Fixes:
// 1. sign = -1 (not 0) for negative numbers
// 2. Accumulate as long (to detect overflow before it wraps)
// 3. Check against INT_MAX / INT_MIN boundaries during accumulation

#include <stdio.h>
#include <limits.h>

int my_strtoi(const char *s, int *result) {
    if (s == NULL) {
        return -1;
    }

    // Skip leading whitespace
    while (*s == ' ' || *s == '\t' || *s == '\n') {
        s++;
    }

    // Handle optional sign
    int sign = 1;
    if (*s == '+' || *s == '-') {
        if (*s == '-') {
            sign = -1;
        }
        s++;
    }

    // Must have at least one digit
    if (*s < '0' || *s > '9') {
        return -1;
    }

    // Accumulate in a long to detect overflow safely.
    // On all platforms where clings targets, long is at least 64 bits
    // or at least wider than int, so this is safe for int-range detection.
    long value = 0;
    while (*s >= '0' && *s <= '9') {
        int digit = *s - '0';
        value = value * 10 + digit;

        // Early overflow check: if value exceeds INT_MAX range even before
        // applying sign, we know it's overflow (for positive). For negative,
        // INT_MIN magnitude is INT_MAX + 1.
        if (sign == 1 && value > (long)INT_MAX) {
            return -2;
        }
        if (sign == -1 && value > (long)INT_MAX + 1) {
            return -2;
        }

        s++;
    }

    *result = (int)(value * sign);
    return 0;
}

#ifndef TEST
int main(void) {
    int val;
    const char *tests[] = {"42", "-7", "  +123", "abc", "", "2147483648", NULL};

    for (int i = 0; tests[i] != NULL; i++) {
        int rc = my_strtoi(tests[i], &val);
        if (rc == 0) {
            printf("\"%s\" -> %d\n", tests[i], val);
        } else {
            printf("\"%s\" -> error %d\n", tests[i], rc);
        }
    }

    return 0;
}
#else
#include "clings_test.h"

TEST(test_simple_positive) {
    int val;
    ASSERT_EQ(my_strtoi("42", &val), 0);
    ASSERT_EQ(val, 42);
}

TEST(test_negative) {
    int val;
    ASSERT_EQ(my_strtoi("-7", &val), 0);
    ASSERT_EQ(val, -7);
}

TEST(test_leading_whitespace_and_plus) {
    int val;
    ASSERT_EQ(my_strtoi("  +123", &val), 0);
    ASSERT_EQ(val, 123);
}

TEST(test_zero) {
    int val;
    ASSERT_EQ(my_strtoi("0", &val), 0);
    ASSERT_EQ(val, 0);
}

TEST(test_invalid_no_digits) {
    int val;
    ASSERT_EQ(my_strtoi("abc", &val), -1);
}

TEST(test_invalid_empty) {
    int val;
    ASSERT_EQ(my_strtoi("", &val), -1);
}

TEST(test_invalid_null) {
    int val;
    ASSERT_EQ(my_strtoi(NULL, &val), -1);
}

TEST(test_invalid_sign_only) {
    int val;
    ASSERT_EQ(my_strtoi("-", &val), -1);
}

TEST(test_stops_at_non_digit) {
    int val;
    ASSERT_EQ(my_strtoi("99bottles", &val), 0);
    ASSERT_EQ(val, 99);
}

TEST(test_int_max) {
    int val;
    ASSERT_EQ(my_strtoi("2147483647", &val), 0);
    ASSERT_EQ(val, 2147483647);
}

TEST(test_int_min) {
    int val;
    ASSERT_EQ(my_strtoi("-2147483648", &val), 0);
    ASSERT_EQ(val, -2147483648);
}

TEST(test_overflow_positive) {
    int val;
    ASSERT_EQ(my_strtoi("2147483648", &val), -2);
}

TEST(test_overflow_negative) {
    int val;
    ASSERT_EQ(my_strtoi("-2147483649", &val), -2);
}

TEST(test_overflow_large) {
    int val;
    ASSERT_EQ(my_strtoi("99999999999", &val), -2);
}

int main(void) {
    RUN_TEST(test_simple_positive);
    RUN_TEST(test_negative);
    RUN_TEST(test_leading_whitespace_and_plus);
    RUN_TEST(test_zero);
    RUN_TEST(test_invalid_no_digits);
    RUN_TEST(test_invalid_empty);
    RUN_TEST(test_invalid_null);
    RUN_TEST(test_invalid_sign_only);
    RUN_TEST(test_stops_at_non_digit);
    RUN_TEST(test_int_max);
    RUN_TEST(test_int_min);
    RUN_TEST(test_overflow_positive);
    RUN_TEST(test_overflow_negative);
    RUN_TEST(test_overflow_large);
    TEST_REPORT();
}
#endif
