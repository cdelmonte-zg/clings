// pointers2.c - Solution
//
// Two bugs to fix:
// 1. swap() didn't use a temp variable, so it lost the original value of *a
// 2. reverse_array() had an off-by-one: right should be arr + len - 1

#include <stdio.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverse_array(int *arr, int len) {
    int *left = arr;
    int *right = arr + len - 1;
    while (left < right) {
        swap(left, right);
        left++;
        right--;
    }
}

#ifndef TEST
int main(void) {
    int arr[] = {1, 2, 3, 4, 5};
    int len = sizeof(arr) / sizeof(arr[0]);

    reverse_array(arr, len);

    printf("Reversed: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
#else
#include "clings_test.h"

TEST(test_reverse_five) {
    int a[] = {1, 2, 3, 4, 5};
    reverse_array(a, 5);
    ASSERT_EQ(a[0], 5); ASSERT_EQ(a[1], 4); ASSERT_EQ(a[2], 3);
    ASSERT_EQ(a[3], 2); ASSERT_EQ(a[4], 1);
}

TEST(test_reverse_two) {
    int b[] = {10, 20};
    reverse_array(b, 2);
    ASSERT_EQ(b[0], 20); ASSERT_EQ(b[1], 10);
}

TEST(test_reverse_one) {
    int c[] = {42};
    reverse_array(c, 1);
    ASSERT_EQ(c[0], 42);
}

int main(void) {
    RUN_TEST(test_reverse_five);
    RUN_TEST(test_reverse_two);
    RUN_TEST(test_reverse_one);
    TEST_REPORT();
}
#endif
