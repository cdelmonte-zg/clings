// bitwise3.c - Solution
//
// Fixes:
// 1. next_power_of_two: decrement n before bit-smearing so that exact
//    powers of two don't get doubled (e.g., 8 -> 7 -> smear -> 7 -> +1 = 8).
// 2. highest_set_bit: remove the spurious +1 at the end; the while loop
//    already counts correctly.

#include <stdio.h>

unsigned int next_power_of_two(unsigned int n) {
    if (n == 0) {
        return 1;
    }
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

int highest_set_bit(unsigned int n) {
    if (n == 0) {
        return -1;
    }
    int pos = 0;
    while (n >>= 1) {
        pos++;
    }
    return pos;
}

unsigned int swap_nibbles(unsigned int byte) {
    unsigned int low = byte & 0x0F;
    unsigned int high = (byte >> 4) & 0x0F;
    return (low << 4) | high;
}

#ifndef TEST
int main(void) {
    printf("next_power_of_two(5) = %u\n", next_power_of_two(5));
    printf("next_power_of_two(8) = %u\n", next_power_of_two(8));
    printf("next_power_of_two(1) = %u\n", next_power_of_two(1));
    printf("highest_set_bit(1) = %d\n", highest_set_bit(1));
    printf("highest_set_bit(128) = %d\n", highest_set_bit(128));
    printf("swap_nibbles(0xAB) = 0x%X\n", swap_nibbles(0xAB));
    return 0;
}
#else
#include "clings_test.h"

TEST(test_next_pow2_five) {
    ASSERT_EQ(next_power_of_two(5), 8u);
}

TEST(test_next_pow2_eight) {
    ASSERT_EQ(next_power_of_two(8), 8u);
}

TEST(test_next_pow2_one) {
    ASSERT_EQ(next_power_of_two(1), 1u);
}

TEST(test_next_pow2_zero) {
    ASSERT_EQ(next_power_of_two(0), 1u);
}

TEST(test_highest_bit_one) {
    ASSERT_EQ(highest_set_bit(1), 0);
}

TEST(test_highest_bit_128) {
    ASSERT_EQ(highest_set_bit(128), 7);
}

TEST(test_highest_bit_zero) {
    ASSERT_EQ(highest_set_bit(0), -1);
}

TEST(test_highest_bit_255) {
    ASSERT_EQ(highest_set_bit(255), 7);
}

TEST(test_swap_nibbles_ab) {
    ASSERT_EQ(swap_nibbles(0xAB), 0xBAu);
}

TEST(test_swap_nibbles_12) {
    ASSERT_EQ(swap_nibbles(0x12), 0x21u);
}

TEST(test_swap_nibbles_f0) {
    ASSERT_EQ(swap_nibbles(0xF0), 0x0Fu);
}

int main(void) {
    RUN_TEST(test_next_pow2_five);
    RUN_TEST(test_next_pow2_eight);
    RUN_TEST(test_next_pow2_one);
    RUN_TEST(test_next_pow2_zero);
    RUN_TEST(test_highest_bit_one);
    RUN_TEST(test_highest_bit_128);
    RUN_TEST(test_highest_bit_zero);
    RUN_TEST(test_highest_bit_255);
    RUN_TEST(test_swap_nibbles_ab);
    RUN_TEST(test_swap_nibbles_12);
    RUN_TEST(test_swap_nibbles_f0);
    TEST_REPORT();
}
#endif
