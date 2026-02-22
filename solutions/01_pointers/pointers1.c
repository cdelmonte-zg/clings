// pointers1.c - Solution
//
// The fix: pass the array length as a separate parameter, since
// sizeof(arr) in a function gives the pointer size, not the array size.

#include <stdio.h>

int array_sum(int arr[], int len) {
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += arr[i];
    }
    return sum;
}

#ifndef TEST
int main(void) {
    int numbers[] = {10, 20, 30, 40, 50};
    printf("Sum: %d\n", array_sum(numbers, sizeof(numbers) / sizeof(numbers[0])));
    return 0;
}
#else
#include "clings_test.h"

TEST(test_five_elements) {
    int a[] = {10, 20, 30, 40, 50};
    ASSERT_EQ(array_sum(a, sizeof(a) / sizeof(a[0])), 150);
}

TEST(test_single_element) {
    int b[] = {1};
    ASSERT_EQ(array_sum(b, sizeof(b) / sizeof(b[0])), 1);
}

TEST(test_negative_numbers) {
    int c[] = {-5, 5, -10, 10};
    ASSERT_EQ(array_sum(c, sizeof(c) / sizeof(c[0])), 0);
}

int main(void) {
    RUN_TEST(test_five_elements);
    RUN_TEST(test_single_element);
    RUN_TEST(test_negative_numbers);
    TEST_REPORT();
}
#endif
