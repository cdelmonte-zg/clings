// error_handling2.c - Solution
//
// Fix: Add proper validation at each step:
//   1. Check for NULL or empty input (return -1)
//   2. Check that comma exists (return -2)
//   3. Validate first number (return -3)
//   4. Validate second number (return -4)
// Never modify output parameters on error.

#include <stdio.h>
#include <string.h>

// Helper: returns 1 if s contains only digits (with optional leading '-'), 0 otherwise
static int is_valid_int(const char *s, int len) {
    if (len <= 0) return 0;
    int start = 0;
    if (s[0] == '-') {
        if (len == 1) return 0;  // just "-" is not valid
        start = 1;
    }
    for (int i = start; i < len; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }
    return 1;
}

// Helper: simple string-to-int (no error checking — caller uses is_valid_int first)
static int parse_int(const char *s, int len) {
    int result = 0;
    int negative = 0;
    int start = 0;
    if (s[0] == '-') {
        negative = 1;
        start = 1;
    }
    for (int i = start; i < len; i++) {
        result = result * 10 + (s[i] - '0');
    }
    return negative ? -result : result;
}

int read_pair(const char *input, int *a, int *b) {
    // Fixed: check for NULL or empty input
    if (input == NULL || input[0] == '\0') {
        return -1;
    }

    // Find the comma
    const char *comma = NULL;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ',') {
            comma = &input[i];
            break;
        }
    }

    // Fixed: check if comma was found
    if (comma == NULL) {
        return -2;
    }

    int first_len = (int)(comma - input);
    int second_len = (int)strlen(comma + 1);

    // Fixed: validate the first number before parsing
    if (!is_valid_int(input, first_len)) {
        return -3;
    }

    // Fixed: validate the second number before parsing
    if (!is_valid_int(comma + 1, second_len)) {
        return -4;
    }

    *a = parse_int(input, first_len);
    *b = parse_int(comma + 1, second_len);

    return 0;
}

#ifndef TEST
int main(void) {
    int a, b;
    int err;

    err = read_pair("42,17", &a, &b);
    printf("\"42,17\" -> a=%d, b=%d (err=%d)\n", a, b, err);

    err = read_pair("hello", &a, &b);
    printf("\"hello\" -> err=%d\n", err);

    err = read_pair("", &a, &b);
    printf("\"\" -> err=%d\n", err);

    return 0;
}
#else
#include "clings_test.h"

TEST(test_valid_pair) {
    int a = 0, b = 0;
    int err = read_pair("42,17", &a, &b);
    ASSERT_EQ(err, 0);
    ASSERT_EQ(a, 42);
    ASSERT_EQ(b, 17);
}

TEST(test_negative_numbers) {
    int a = 0, b = 0;
    int err = read_pair("-5,10", &a, &b);
    ASSERT_EQ(err, 0);
    ASSERT_EQ(a, -5);
    ASSERT_EQ(b, 10);
}

TEST(test_both_negative) {
    int a = 0, b = 0;
    int err = read_pair("-3,-7", &a, &b);
    ASSERT_EQ(err, 0);
    ASSERT_EQ(a, -3);
    ASSERT_EQ(b, -7);
}

TEST(test_zeros) {
    int a = 99, b = 99;
    int err = read_pair("0,0", &a, &b);
    ASSERT_EQ(err, 0);
    ASSERT_EQ(a, 0);
    ASSERT_EQ(b, 0);
}

TEST(test_null_input) {
    int a = 99, b = 99;
    int err = read_pair(NULL, &a, &b);
    ASSERT_EQ(err, -1);
    // a and b should be untouched
    ASSERT_EQ(a, 99);
    ASSERT_EQ(b, 99);
}

TEST(test_empty_input) {
    int a = 99, b = 99;
    int err = read_pair("", &a, &b);
    ASSERT_EQ(err, -1);
    ASSERT_EQ(a, 99);
    ASSERT_EQ(b, 99);
}

TEST(test_missing_comma) {
    int a = 99, b = 99;
    int err = read_pair("12345", &a, &b);
    ASSERT_EQ(err, -2);
    ASSERT_EQ(a, 99);
    ASSERT_EQ(b, 99);
}

TEST(test_invalid_first_number) {
    int a = 99, b = 99;
    int err = read_pair("abc,5", &a, &b);
    ASSERT_EQ(err, -3);
    ASSERT_EQ(a, 99);
    ASSERT_EQ(b, 99);
}

TEST(test_invalid_second_number) {
    int a = 99, b = 99;
    int err = read_pair("5,xyz", &a, &b);
    ASSERT_EQ(err, -4);
    ASSERT_EQ(a, 99);
    ASSERT_EQ(b, 99);
}

TEST(test_empty_before_comma) {
    int a = 99, b = 99;
    int err = read_pair(",5", &a, &b);
    ASSERT_EQ(err, -3);
    ASSERT_EQ(a, 99);
    ASSERT_EQ(b, 99);
}

TEST(test_empty_after_comma) {
    int a = 99, b = 99;
    int err = read_pair("5,", &a, &b);
    ASSERT_EQ(err, -4);
    ASSERT_EQ(a, 99);
    ASSERT_EQ(b, 99);
}

int main(void) {
    RUN_TEST(test_valid_pair);
    RUN_TEST(test_negative_numbers);
    RUN_TEST(test_both_negative);
    RUN_TEST(test_zeros);
    RUN_TEST(test_null_input);
    RUN_TEST(test_empty_input);
    RUN_TEST(test_missing_comma);
    RUN_TEST(test_invalid_first_number);
    RUN_TEST(test_invalid_second_number);
    RUN_TEST(test_empty_before_comma);
    RUN_TEST(test_empty_after_comma);
    TEST_REPORT();
}
#endif
