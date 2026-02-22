// ub_lab6.c - Solution
//
// Fixes:
// 1. find_last_index: use `i < size` instead of `i <= size`
// 2. my_strdup: allocate `len + 1` bytes to include the null terminator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_char(const char *str, char c) {
    int len = (int)strlen(str);
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] == c) {
            count++;
        }
    }
    return count;
}

int find_last_index(const int *buf, int size, int c) {
    int last = -1;
    /* FIX: `i < size` instead of `i <= size` */
    for (int i = 0; i < size; i++) {
        if (buf[i] == c) {
            last = i;
        }
    }
    return last;
}

char *my_strdup(const char *src) {
    size_t len = strlen(src);
    /* FIX: allocate len + 1 for the null terminator */
    char *copy = malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, src, len + 1);
    return copy;
}

#ifndef TEST
int main(void) {
    const char *msg = "hello world";
    printf("'l' appears %d times in \"%s\"\n", count_char(msg, 'l'), msg);

    int data[] = {10, 20, 30, 20, 40};
    int idx = find_last_index(data, 5, 20);
    printf("Last index of 20: %d\n", idx);

    char *dup = my_strdup("test");
    if (dup) {
        printf("Duplicated: %s\n", dup);
        free(dup);
    }

    return 0;
}
#else
#include "clings_test.h"

TEST(test_count_char_basic) {
    ASSERT_EQ(count_char("hello", 'l'), 2);
    ASSERT_EQ(count_char("hello", 'h'), 1);
    ASSERT_EQ(count_char("hello", 'z'), 0);
}

TEST(test_count_char_empty) {
    ASSERT_EQ(count_char("", 'a'), 0);
}

TEST(test_find_last_basic) {
    int data[] = {10, 20, 30, 20, 40};
    ASSERT_EQ(find_last_index(data, 5, 20), 3);
    ASSERT_EQ(find_last_index(data, 5, 10), 0);
    ASSERT_EQ(find_last_index(data, 5, 40), 4);
}

TEST(test_find_last_not_found) {
    int data[] = {1, 2, 3};
    ASSERT_EQ(find_last_index(data, 3, 99), -1);
}

TEST(test_my_strdup_basic) {
    char *s = my_strdup("hello");
    ASSERT(s != NULL);
    ASSERT_STR_EQ(s, "hello");
    free(s);
}

TEST(test_my_strdup_empty) {
    char *s = my_strdup("");
    ASSERT(s != NULL);
    ASSERT_STR_EQ(s, "");
    free(s);
}

int main(void) {
    RUN_TEST(test_count_char_basic);
    RUN_TEST(test_count_char_empty);
    RUN_TEST(test_find_last_basic);
    RUN_TEST(test_find_last_not_found);
    RUN_TEST(test_my_strdup_basic);
    RUN_TEST(test_my_strdup_empty);
    TEST_REPORT();
}
#endif
