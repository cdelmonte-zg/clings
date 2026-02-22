// const1.c - Solution
//
// Fix: Add `const` to all parameters that should not be modified:
//   - string_length takes `const char *s`
//   - array_max takes `const int *arr`
//   - print_person takes `const struct person *p`

#include <stdio.h>
#include <string.h>

struct person {
    char name[64];
    int age;
};

// Fixed: const char *s — we only read the string
size_t string_length(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Fixed: const int *arr — we only read the array
int array_max(const int *arr, int len) {
    int max = arr[0];
    for (int i = 1; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Fixed: const struct person *p — we only read the struct
void print_person(const struct person *p, char *buf, size_t buf_size) {
    snprintf(buf, buf_size, "%s is %d years old", p->name, p->age);
}

#ifndef TEST
int main(void) {
    // With const fixed, these can safely use const-qualified variables.
    char greeting[] = "Hello, world!";
    printf("Length of \"%s\" = %zu\n", greeting, string_length(greeting));

    int nums[] = {3, 7, 1, 9, 4};
    printf("Max = %d\n", array_max(nums, 5));

    struct person alice = {"Alice", 30};
    char buf[128];
    print_person(&alice, buf, sizeof(buf));
    printf("%s\n", buf);

    return 0;
}
#else
#include "clings_test.h"

TEST(test_string_length_basic) {
    const char *s = "hello";
    ASSERT(string_length(s) == 5);
}

TEST(test_string_length_empty) {
    const char *s = "";
    ASSERT(string_length(s) == 0);
}

TEST(test_string_length_literal) {
    ASSERT(string_length("abcdef") == 6);
}

TEST(test_array_max_basic) {
    const int arr[] = {3, 7, 1, 9, 4};
    ASSERT_EQ(array_max(arr, 5), 9);
}

TEST(test_array_max_single) {
    const int arr[] = {42};
    ASSERT_EQ(array_max(arr, 1), 42);
}

TEST(test_array_max_negative) {
    const int arr[] = {-5, -2, -8, -1};
    ASSERT_EQ(array_max(arr, 4), -1);
}

TEST(test_print_person) {
    const struct person bob = {"Bob", 25};
    char buf[128];
    print_person(&bob, buf, sizeof(buf));
    ASSERT_STR_EQ(buf, "Bob is 25 years old");
}

TEST(test_print_person_const) {
    const struct person eve = {"Eve", 99};
    char buf[128];
    print_person(&eve, buf, sizeof(buf));
    ASSERT_STR_EQ(buf, "Eve is 99 years old");
}

int main(void) {
    RUN_TEST(test_string_length_basic);
    RUN_TEST(test_string_length_empty);
    RUN_TEST(test_string_length_literal);
    RUN_TEST(test_array_max_basic);
    RUN_TEST(test_array_max_single);
    RUN_TEST(test_array_max_negative);
    RUN_TEST(test_print_person);
    RUN_TEST(test_print_person_const);
    TEST_REPORT();
}
#endif
