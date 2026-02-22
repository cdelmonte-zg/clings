// structs1.c - Solution
//
// Fix: Reorder padded_record_t fields from largest alignment to smallest:
//   double (8) + int (4) + char (1) + 3 padding = 16 bytes total
// instead of the original 24 bytes.

#include <stdio.h>
#include <stddef.h>

typedef struct {
    int x;
    int y;
} packed_point_t;

// Fixed: fields ordered from largest to smallest alignment
typedef struct {
    double value;
    int count;
    char flag;
} padded_record_t;

size_t record_size(void) {
    return sizeof(padded_record_t);
}

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
