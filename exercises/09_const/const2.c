// const2.c - Pointer to const vs const pointer
//
// There are three ways to use const with pointers:
//   const int *p       — pointer to const int (can't modify *p, can reassign p)
//   int *const p       — const pointer to int (can modify *p, can't reassign p)
//   const int *const p — const pointer to const int (can't modify either)
//
// Implement two functions:
//   find_first()  — searches a const array for a target value, returns pointer
//   array_fill()  — fills every element of an array with a given value
//
// BUG 1: find_first takes `int *arr` and returns `int *`. Since the test
//         passes a `const int[]` array, it won't compile. Fix the parameter
//         to `const int *arr` and the return type to `const int *`.
//
// BUG 2: array_fill modifies the pointer `arr` by incrementing it in the loop.
//         After the loop, it writes a sentinel that goes past the buffer.
//         Fix: declare arr as `int *const arr` and use index-based access.
//         The const on the pointer would have caught this bug at compile time.

#include <stdio.h>
#include <stddef.h>

// BUG: Should return `const int *` and take `const int *arr`
// since we only read the array, never write to it.
int *find_first(int *arr, int len, int target) {
    for (int i = 0; i < len; i++) {
        if (arr[i] == target) {
            return &arr[i];
        }
    }
    return NULL;
}

// BUG: Modifying `arr` directly and then writing past the end.
// Fix by using `int *const arr` with index-based access.
// The compiler would catch the arr++ if arr were declared const.
void array_fill(int *arr, int len, int value) {
    for (int i = 0; i < len; i++) {
        *arr = value;
        arr++;
    }
    // BUG: arr now points one past the end — this writes out of bounds!
    // If arr were `int *const`, the arr++ above wouldn't compile,
    // forcing index-based access and avoiding this bug entirely.
    *arr = 0;
}

#ifndef TEST
int main(void) {
    int data[] = {10, 20, 30, 40, 50};
    int *found = find_first(data, 5, 30);
    if (found) {
        printf("Found 30 at offset %td\n", found - data);
    }

    // Using a larger buffer so the out-of-bounds write doesn't crash here
    int buf[8] = {0};
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
