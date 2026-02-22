// error_handling3.c - Solution
//
// Fix 1: error_set uses vsnprintf with proper buffer size.
// Fix 2: parse_config_line checks for NULL input.
// Fix 3: parse_config_line checks for missing '='.
// Fix 4: parse_config_line validates key/value buffer sizes.

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

struct error {
    int code;
    char message[256];
};

// Fixed: uses vsnprintf to prevent buffer overflow
void error_set(struct error *err, int code, const char *fmt, ...) {
    err->code = code;
    va_list args;
    va_start(args, fmt);
    vsnprintf(err->message, sizeof(err->message), fmt, args);
    va_end(args);
}

void error_clear(struct error *err) {
    err->code = 0;
    err->message[0] = '\0';
}

// Fixed: validates all inputs and checks buffer sizes
int parse_config_line(const char *line, char *key, size_t key_size,
                      char *value, size_t value_size, struct error *err) {
    error_clear(err);

    // Fixed: check for NULL input
    if (line == NULL) {
        error_set(err, 1, "input line is NULL");
        return -1;
    }

    // Find the '=' delimiter
    const char *eq = NULL;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '=') {
            eq = &line[i];
            break;
        }
    }

    // Fixed: check if '=' was found
    if (eq == NULL) {
        error_set(err, 2, "missing '=' delimiter in \"%s\"", line);
        return -1;
    }

    size_t key_len = (size_t)(eq - line);
    size_t val_len = strlen(eq + 1);

    // Fixed: check if key fits in buffer (need room for null terminator)
    if (key_len + 1 > key_size) {
        error_set(err, 3, "key length %zu exceeds buffer size %zu",
                  key_len, key_size);
        return -1;
    }

    // Fixed: check if value fits in buffer (need room for null terminator)
    if (val_len + 1 > value_size) {
        error_set(err, 4, "value length %zu exceeds buffer size %zu",
                  val_len, value_size);
        return -1;
    }

    memcpy(key, line, key_len);
    key[key_len] = '\0';

    memcpy(value, eq + 1, val_len);
    value[val_len] = '\0';

    return 0;
}

#ifndef TEST
int main(void) {
    struct error err;
    char key[32], value[32];

    int rc = parse_config_line("name=alice", key, sizeof(key),
                               value, sizeof(value), &err);
    if (rc == 0) {
        printf("key=\"%s\" value=\"%s\"\n", key, value);
    } else {
        printf("Error %d: %s\n", err.code, err.message);
    }

    rc = parse_config_line("bad line", key, sizeof(key),
                           value, sizeof(value), &err);
    if (rc != 0) {
        printf("Error %d: %s\n", err.code, err.message);
    }

    return 0;
}
#else
#include "clings_test.h"

TEST(test_parse_valid) {
    struct error err;
    char key[32], value[32];
    int rc = parse_config_line("name=alice", key, sizeof(key),
                               value, sizeof(value), &err);
    ASSERT_EQ(rc, 0);
    ASSERT_STR_EQ(key, "name");
    ASSERT_STR_EQ(value, "alice");
}

TEST(test_parse_numeric_value) {
    struct error err;
    char key[32], value[32];
    int rc = parse_config_line("port=8080", key, sizeof(key),
                               value, sizeof(value), &err);
    ASSERT_EQ(rc, 0);
    ASSERT_STR_EQ(key, "port");
    ASSERT_STR_EQ(value, "8080");
}

TEST(test_parse_empty_value) {
    struct error err;
    char key[32], value[32];
    int rc = parse_config_line("empty=", key, sizeof(key),
                               value, sizeof(value), &err);
    ASSERT_EQ(rc, 0);
    ASSERT_STR_EQ(key, "empty");
    ASSERT_STR_EQ(value, "");
}

TEST(test_parse_null_input) {
    struct error err;
    char key[32], value[32];
    int rc = parse_config_line(NULL, key, sizeof(key),
                               value, sizeof(value), &err);
    ASSERT_EQ(rc, -1);
    ASSERT_NE(err.code, 0);
}

TEST(test_parse_missing_equals) {
    struct error err;
    char key[32], value[32];
    int rc = parse_config_line("no-equals-here", key, sizeof(key),
                               value, sizeof(value), &err);
    ASSERT_EQ(rc, -1);
    ASSERT_NE(err.code, 0);
}

TEST(test_parse_key_too_long) {
    struct error err;
    char key[4], value[32];
    // Key is "longkey" (7 chars) but buffer is only 4
    int rc = parse_config_line("longkey=val", key, sizeof(key),
                               value, sizeof(value), &err);
    ASSERT_EQ(rc, -1);
    ASSERT_NE(err.code, 0);
}

TEST(test_parse_value_too_long) {
    struct error err;
    char key[32], value[4];
    // Value is "longvalue" (9 chars) but buffer is only 4
    int rc = parse_config_line("k=longvalue", key, sizeof(key),
                               value, sizeof(value), &err);
    ASSERT_EQ(rc, -1);
    ASSERT_NE(err.code, 0);
}

TEST(test_error_set_message) {
    struct error err;
    error_set(&err, 42, "failed at step %d: %s", 3, "bad input");
    ASSERT_EQ(err.code, 42);
    ASSERT_STR_EQ(err.message, "failed at step 3: bad input");
}

TEST(test_error_clear) {
    struct error err;
    error_set(&err, 1, "some error");
    error_clear(&err);
    ASSERT_EQ(err.code, 0);
    ASSERT_STR_EQ(err.message, "");
}

int main(void) {
    RUN_TEST(test_parse_valid);
    RUN_TEST(test_parse_numeric_value);
    RUN_TEST(test_parse_empty_value);
    RUN_TEST(test_parse_null_input);
    RUN_TEST(test_parse_missing_equals);
    RUN_TEST(test_parse_key_too_long);
    RUN_TEST(test_parse_value_too_long);
    RUN_TEST(test_error_set_message);
    RUN_TEST(test_error_clear);
    TEST_REPORT();
}
#endif
