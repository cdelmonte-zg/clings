// ub_lab2.c - Solution
//
// Fix: use memcpy() to copy bytes between float and uint32_t.
// This is well-defined, and the compiler optimizes it to a simple
// register move (zero overhead compared to the pointer cast).

#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t float_to_bits(float f) {
    uint32_t bits;
    memcpy(&bits, &f, sizeof(bits));
    return bits;
}

float bits_to_float(uint32_t bits) {
    float f;
    memcpy(&f, &bits, sizeof(f));
    return f;
}

int float_is_negative(float f) {
    uint32_t bits = float_to_bits(f);
    return (bits >> 31) & 1;
}

#ifndef TEST
int main(void) {
    float val = -3.14f;
    uint32_t bits = float_to_bits(val);

    printf("Float: %f\n", (double)val);
    printf("Bits:  0x%08X\n", bits);
    printf("Sign:  %s\n", float_is_negative(val) ? "negative" : "positive");

    float reconstructed = bits_to_float(bits);
    printf("Reconstructed: %f\n", (double)reconstructed);

    return 0;
}
#else
#include "clings_test.h"

TEST(test_zero_bits) {
    uint32_t bits = float_to_bits(0.0f);
    ASSERT_EQ(bits, 0u);
}

TEST(test_one_bits) {
    /* IEEE 754: 1.0f = 0x3F800000 */
    uint32_t bits = float_to_bits(1.0f);
    ASSERT_EQ(bits, 0x3F800000u);
}

TEST(test_roundtrip) {
    float original = 42.5f;
    uint32_t bits = float_to_bits(original);
    float result = bits_to_float(bits);
    ASSERT(original == result);
}

TEST(test_negative_roundtrip) {
    float original = -123.456f;
    uint32_t bits = float_to_bits(original);
    float result = bits_to_float(bits);
    ASSERT(original == result);
}

TEST(test_sign_positive) {
    ASSERT(!float_is_negative(1.0f));
    ASSERT(!float_is_negative(0.0f));
}

TEST(test_sign_negative) {
    ASSERT(float_is_negative(-1.0f));
    ASSERT(float_is_negative(-0.001f));
}

int main(void) {
    RUN_TEST(test_zero_bits);
    RUN_TEST(test_one_bits);
    RUN_TEST(test_roundtrip);
    RUN_TEST(test_negative_roundtrip);
    RUN_TEST(test_sign_positive);
    RUN_TEST(test_sign_negative);
    TEST_REPORT();
}
#endif
