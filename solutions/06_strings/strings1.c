// strings1.c - Solution
//
// Fix: The remaining space must subtract 1 for the null terminator.
// remaining = dst_size - dst_len - 1
// This ensures we always leave room for '\0'.

#include <stdio.h>
#include <string.h>

int safe_strcat(char *dst, size_t dst_size, const char *src) {
    size_t dst_len = strlen(dst);
    size_t src_len = strlen(src);

    // Correct: subtract 1 to reserve space for the null terminator
    if (dst_len + 1 >= dst_size) {
        // No room at all (buffer already full)
        return (src_len == 0) ? 0 : -1;
    }

    size_t remaining = dst_size - dst_len - 1;

    if (src_len > remaining) {
        // Would overflow — truncate to fit
        memcpy(dst + dst_len, src, remaining);
        dst[dst_size - 1] = '\0';
        return -1;
    }

    memcpy(dst + dst_len, src, src_len + 1);
    return 0;
}

#ifndef TEST
int main(void) {
    char buf[16] = "Hello";

    printf("Before: \"%s\"\n", buf);

    int rc = safe_strcat(buf, sizeof(buf), ", world!");
    printf("After:  \"%s\" (rc=%d)\n", buf, rc);

    rc = safe_strcat(buf, sizeof(buf), " This is way too long to fit.");
    printf("Trunc:  \"%s\" (rc=%d)\n", buf, rc);

    return 0;
}
#else
#include "clings_test.h"

TEST(test_normal_concat) {
    char buf[32] = "Hello";
    int rc = safe_strcat(buf, sizeof(buf), ", world!");
    ASSERT_EQ(rc, 0);
    ASSERT_STR_EQ(buf, "Hello, world!");
}

TEST(test_empty_src) {
    char buf[16] = "Hello";
    int rc = safe_strcat(buf, sizeof(buf), "");
    ASSERT_EQ(rc, 0);
    ASSERT_STR_EQ(buf, "Hello");
}

TEST(test_empty_dst) {
    char buf[8] = "";
    int rc = safe_strcat(buf, sizeof(buf), "Hi");
    ASSERT_EQ(rc, 0);
    ASSERT_STR_EQ(buf, "Hi");
}

TEST(test_exact_fit) {
    // buf is 6 bytes: room for "Hello" (5 chars + null)
    char buf[6] = "He";
    // Appending "llo" should exactly fill: "Hello" + '\0' = 6 bytes
    int rc = safe_strcat(buf, sizeof(buf), "llo");
    ASSERT_EQ(rc, 0);
    ASSERT_STR_EQ(buf, "Hello");
}

TEST(test_overflow_truncates) {
    char buf[8] = "Hello";
    // buf has 8 bytes, "Hello" uses 5, so 2 chars + null can fit
    int rc = safe_strcat(buf, sizeof(buf), "!!!");
    ASSERT_EQ(rc, -1);
    // Should truncate: "Hello" + "!!" = "Hello!!" (7 chars + null = 8)
    ASSERT_STR_EQ(buf, "Hello!!");
}

TEST(test_no_room_at_all) {
    char buf[6] = "Hello";
    // Buffer is exactly full already (5 chars + null = 6 bytes)
    int rc = safe_strcat(buf, sizeof(buf), "X");
    ASSERT_EQ(rc, -1);
    ASSERT_STR_EQ(buf, "Hello");
}

int main(void) {
    RUN_TEST(test_normal_concat);
    RUN_TEST(test_empty_src);
    RUN_TEST(test_empty_dst);
    RUN_TEST(test_exact_fit);
    RUN_TEST(test_overflow_truncates);
    RUN_TEST(test_no_room_at_all);
    TEST_REPORT();
}
#endif
