// ub_lab1.c - Solution
//
// Fix: use `1u << n` instead of `1 << n`.
// The `u` suffix makes the literal unsigned, so shifting into bit 31
// is well-defined (it just sets the high bit of an unsigned int).

#include <stdio.h>

void set_bit(unsigned int *flags, int n) {
    *flags |= (1u << n);
}

int check_bit(unsigned int flags, int n) {
    return (flags & (1u << n)) != 0;
}

void clear_bit(unsigned int *flags, int n) {
    *flags &= ~(1u << n);
}

#ifndef TEST
int main(void) {
    unsigned int flags = 0;

    set_bit(&flags, 0);
    set_bit(&flags, 15);
    set_bit(&flags, 31);

    printf("Bit  0: %s\n", check_bit(flags, 0)  ? "set" : "clear");
    printf("Bit 15: %s\n", check_bit(flags, 15) ? "set" : "clear");
    printf("Bit 31: %s\n", check_bit(flags, 31) ? "set" : "clear");
    printf("Bit  7: %s\n", check_bit(flags, 7)  ? "set" : "clear");

    clear_bit(&flags, 15);
    printf("Bit 15 after clear: %s\n", check_bit(flags, 15) ? "set" : "clear");

    return 0;
}
#else
#include "clings_test.h"

TEST(test_set_low_bit) {
    unsigned int flags = 0;
    set_bit(&flags, 0);
    ASSERT(check_bit(flags, 0));
}

TEST(test_set_middle_bit) {
    unsigned int flags = 0;
    set_bit(&flags, 15);
    ASSERT(check_bit(flags, 15));
    ASSERT(!check_bit(flags, 14));
}

TEST(test_set_high_bit) {
    unsigned int flags = 0;
    set_bit(&flags, 31);
    ASSERT(check_bit(flags, 31));
}

TEST(test_clear_bit) {
    unsigned int flags = 0;
    set_bit(&flags, 10);
    ASSERT(check_bit(flags, 10));
    clear_bit(&flags, 10);
    ASSERT(!check_bit(flags, 10));
}

TEST(test_multiple_bits) {
    unsigned int flags = 0;
    set_bit(&flags, 0);
    set_bit(&flags, 15);
    set_bit(&flags, 31);
    ASSERT(check_bit(flags, 0));
    ASSERT(check_bit(flags, 15));
    ASSERT(check_bit(flags, 31));
    ASSERT(!check_bit(flags, 7));
}

TEST(test_clear_preserves_others) {
    unsigned int flags = 0;
    set_bit(&flags, 5);
    set_bit(&flags, 10);
    clear_bit(&flags, 5);
    ASSERT(!check_bit(flags, 5));
    ASSERT(check_bit(flags, 10));
}

int main(void) {
    RUN_TEST(test_set_low_bit);
    RUN_TEST(test_set_middle_bit);
    RUN_TEST(test_set_high_bit);
    RUN_TEST(test_clear_bit);
    RUN_TEST(test_multiple_bits);
    RUN_TEST(test_clear_preserves_others);
    TEST_REPORT();
}
#endif
