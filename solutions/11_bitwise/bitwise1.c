// bitwise1.c - Solution
//
// Fixes:
// 1. count_set_bits: iterate over all 32 bits and don't mutate n
//    while iterating with a bitmask (or shift n right instead).
// 2. is_power_of_two: add a check for n != 0, since 0 & (0-1) == 0
//    but 0 is not a power of two.

#include <stdio.h>

int count_set_bits(unsigned int n) {
    int count = 0;
    while (n) {
        count += n & 1u;
        n >>= 1;
    }
    return count;
}

int is_power_of_two(unsigned int n) {
    return n != 0 && (n & (n - 1)) == 0;
}

#ifndef TEST
int main(void) {
    printf("count_set_bits(0xFF) = %d\n", count_set_bits(0xFF));
    printf("count_set_bits(0xFFFFFFFF) = %d\n", count_set_bits(0xFFFFFFFF));
    printf("is_power_of_two(64) = %d\n", is_power_of_two(64));
    printf("is_power_of_two(0) = %d\n", is_power_of_two(0));
    return 0;
}
#else
#include "clings_test.h"

TEST(test_count_zero) {
    ASSERT_EQ(count_set_bits(0), 0);
}

TEST(test_count_0xff) {
    ASSERT_EQ(count_set_bits(0xFF), 8);
}

TEST(test_count_all_ones) {
    ASSERT_EQ(count_set_bits(0xFFFFFFFF), 32);
}

TEST(test_power_of_two_one) {
    ASSERT_EQ(is_power_of_two(1), 1);
}

TEST(test_power_of_two_64) {
    ASSERT_EQ(is_power_of_two(64), 1);
}

TEST(test_power_of_two_zero) {
    ASSERT_EQ(is_power_of_two(0), 0);
}

TEST(test_power_of_two_six) {
    ASSERT_EQ(is_power_of_two(6), 0);
}

int main(void) {
    RUN_TEST(test_count_zero);
    RUN_TEST(test_count_0xff);
    RUN_TEST(test_count_all_ones);
    RUN_TEST(test_power_of_two_one);
    RUN_TEST(test_power_of_two_64);
    RUN_TEST(test_power_of_two_zero);
    RUN_TEST(test_power_of_two_six);
    TEST_REPORT();
}
#endif
