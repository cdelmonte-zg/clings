// structs1.c - Struct layout and sizeof
//
// The compiler inserts padding bytes between struct fields to satisfy
// alignment requirements. The order of fields affects how much padding
// is added, and therefore the total size of the struct.
//
// Fix the padded struct by reordering its fields to minimize padding.

#include <stdio.h>
#include <stddef.h>

// A tightly packed 2D point: two ints, no padding needed.
typedef struct {
    int x;
    int y;
} packed_point_t;

// BUG: These fields are ordered poorly, causing excessive padding.
// On most 64-bit systems:
//   char (1 byte) + 7 padding + double (8 bytes) + int (4 bytes) + 4 padding = 24 bytes
//
// TODO: Reorder the fields so the struct uses minimal space.
// The optimal layout on a 64-bit system should be 16 bytes.
// Hint: place fields from largest alignment to smallest.
typedef struct {
    char flag;
    double value;
    int count;
} padded_record_t;

// Returns the size of padded_record_t
size_t record_size(void) {
    return sizeof(padded_record_t);
}

// Fills in a padded_record_t and returns it
padded_record_t make_record(double value, int count, char flag) {
    padded_record_t r;
    r.value = value;
    r.count = count;
    r.flag = flag;
    return r;
}

#ifndef TEST
int main(void) {
    printf("sizeof(packed_point_t)  = %zu\n", sizeof(packed_point_t));
    printf("sizeof(padded_record_t) = %zu\n", sizeof(padded_record_t));

    padded_record_t r = make_record(3.14, 42, 'A');
    printf("record: value=%.2f count=%d flag=%c\n", r.value, r.count, r.flag);

    return 0;
}
#else
#include "clings_test.h"

TEST(test_packed_point_size) {
    // Two ints should be exactly 8 bytes (no padding needed)
    ASSERT_EQ(sizeof(packed_point_t), 2 * sizeof(int));
}

TEST(test_record_optimized_size) {
    // After reordering: double(8) + int(4) + char(1) + 3 padding = 16
    // The struct must be at most 16 bytes when fields are optimally ordered
    ASSERT(record_size() <= 16);
}

TEST(test_record_field_values) {
    padded_record_t r = make_record(2.718, 100, 'Z');
    ASSERT(r.value > 2.71 && r.value < 2.72);
    ASSERT_EQ(r.count, 100);
    ASSERT_EQ(r.flag, 'Z');
}

TEST(test_point_fields) {
    packed_point_t p;
    p.x = -5;
    p.y = 10;
    ASSERT_EQ(p.x, -5);
    ASSERT_EQ(p.y, 10);
}

int main(void) {
    RUN_TEST(test_packed_point_size);
    RUN_TEST(test_record_optimized_size);
    RUN_TEST(test_record_field_values);
    RUN_TEST(test_point_fields);
    TEST_REPORT();
}
#endif
