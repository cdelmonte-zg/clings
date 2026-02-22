// const2.c - Solution
//
// Fix 1: find_first takes `const int *arr` and returns `const int *`.
// Fix 2: array_fill takes `int *const arr` and uses index-based access,
//         removing the out-of-bounds write that occurred from incrementing arr.

#include <stdio.h>
#include <stddef.h>

// Fixed: takes const int *, returns const int *
const int *find_first(const int *arr, int len, int target) {
    for (int i = 0; i < len; i++) {
        if (arr[i] == target) {
            return &arr[i];
        }
    }
    return NULL;
}

// Fixed: int *const arr — pointer is const so it can't be incremented.
// Uses index-based access. Removed the out-of-bounds `*arr = 0` write.
void array_fill(int *const arr, int len, int value) {
    for (int i = 0; i < len; i++) {
        arr[i] = value;
    }
}

#ifndef TEST
int main(void) {
    int data[] = {10, 20, 30, 40, 50};
    const int *found = find_first(data, 5, 30);
    if (found) {
        printf("Found 30 at offset %td\n", found - data);
    }

    int buf[4];
    array_fill(buf, 4, 7);
    for (int i = 0; i < 4; i++) {
        printf("buf[%d] = %d\n", i, buf[i]);
    }

    return 0;
}
#else
#include "clings_test.h"

TEST(test_find_first_found) {
    const int arr[] = {10, 20, 30, 40, 50};
    const int *p = find_first(arr, 5, 30);
    ASSERT(p != NULL);
    ASSERT_EQ(*p, 30);
    ASSERT_EQ(p - arr, 2);
}

TEST(test_find_first_not_found) {
    const int arr[] = {1, 2, 3};
    const int *p = find_first(arr, 3, 99);
    ASSERT(p == NULL);
}

TEST(test_find_first_returns_first) {
    const int arr[] = {5, 5, 5};
    const int *p = find_first(arr, 3, 5);
    ASSERT(p != NULL);
    ASSERT_EQ(p - arr, 0);
}

TEST(test_array_fill_basic) {
    int arr[5] = {0};
    array_fill(arr, 5, 42);
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(arr[i], 42);
    }
}

TEST(test_array_fill_zero) {
    int arr[3] = {1, 2, 3};
    array_fill(arr, 3, 0);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(arr[i], 0);
    }
}

TEST(test_array_fill_single) {
    int arr[1] = {99};
    array_fill(arr, 1, -1);
    ASSERT_EQ(arr[0], -1);
}

int main(void) {
    RUN_TEST(test_find_first_found);
    RUN_TEST(test_find_first_not_found);
    RUN_TEST(test_find_first_returns_first);
    RUN_TEST(test_array_fill_basic);
    RUN_TEST(test_array_fill_zero);
    RUN_TEST(test_array_fill_single);
    TEST_REPORT();
}
#endif
