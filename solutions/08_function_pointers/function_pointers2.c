// function_pointers2.c - Solution
//
// The fix: cast base to (char*) instead of (int*) for byte-level arithmetic.
// When computing element addresses, we need byte offsets: (char*)base + j * size.
// Using (int*) would multiply the offset by sizeof(int) again, giving wrong addresses.

#include <stdio.h>
#include <string.h>
#include <stddef.h>

int cmp_int_asc(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

int cmp_int_desc(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ib > ia) - (ib < ia);
}

void bubble_sort(void *base, size_t count, size_t size,
                 int (*cmp)(const void *, const void *)) {
    unsigned char tmp[64];  /* enough for small types */
    for (size_t i = 0; i < count; i++) {
        for (size_t j = 0; j + 1 < count - i; j++) {
            void *elem_j   = (char *)base + j * size;
            void *elem_j1  = (char *)base + (j + 1) * size;
            if (cmp(elem_j, elem_j1) > 0) {
                memcpy(tmp, elem_j, size);
                memcpy(elem_j, elem_j1, size);
                memcpy(elem_j1, tmp, size);
            }
        }
    }
}

#ifndef TEST
int main(void) {
    int nums[] = {5, 3, 8, 1, 9, 2};
    int len = sizeof(nums) / sizeof(nums[0]);

    bubble_sort(nums, (size_t)len, sizeof(int), cmp_int_asc);

    printf("Sorted ascending: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    return 0;
}
#else
#include "clings_test.h"

TEST(test_sort_ascending) {
    int a[] = {5, 3, 8, 1, 9, 2};
    bubble_sort(a, 6, sizeof(int), cmp_int_asc);
    ASSERT_EQ(a[0], 1); ASSERT_EQ(a[1], 2); ASSERT_EQ(a[2], 3);
    ASSERT_EQ(a[3], 5); ASSERT_EQ(a[4], 8); ASSERT_EQ(a[5], 9);
}

TEST(test_sort_descending) {
    int a[] = {5, 3, 8, 1, 9, 2};
    bubble_sort(a, 6, sizeof(int), cmp_int_desc);
    ASSERT_EQ(a[0], 9); ASSERT_EQ(a[1], 8); ASSERT_EQ(a[2], 5);
    ASSERT_EQ(a[3], 3); ASSERT_EQ(a[4], 2); ASSERT_EQ(a[5], 1);
}

TEST(test_already_sorted) {
    int a[] = {1, 2, 3, 4, 5};
    bubble_sort(a, 5, sizeof(int), cmp_int_asc);
    ASSERT_EQ(a[0], 1); ASSERT_EQ(a[1], 2); ASSERT_EQ(a[2], 3);
    ASSERT_EQ(a[3], 4); ASSERT_EQ(a[4], 5);
}

TEST(test_single_element) {
    int a[] = {42};
    bubble_sort(a, 1, sizeof(int), cmp_int_asc);
    ASSERT_EQ(a[0], 42);
}

int main(void) {
    RUN_TEST(test_sort_ascending);
    RUN_TEST(test_sort_descending);
    RUN_TEST(test_already_sorted);
    RUN_TEST(test_single_element);
    TEST_REPORT();
}
#endif
