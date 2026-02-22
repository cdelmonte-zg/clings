// preprocessor1.c - Solution
//
// The fix:
// - STRINGIFY uses # to turn argument into a string literal
// - CONCAT uses ## to paste two tokens together
// - DEBUG_VAR uses STRINGIFY(var) to get the variable name as a string

#include <stdio.h>
#include <string.h>

#define STRINGIFY(x) #x

#define CONCAT(a, b) a##b

#define DEBUG_VAR(var) printf("%s = %d\n", STRINGIFY(var), var)

#ifndef TEST
int main(void) {
    int my_value = 42;
    DEBUG_VAR(my_value);

    printf("STRINGIFY test: %s\n", STRINGIFY(hello_world));

    int foobar = 99;
    printf("CONCAT test: %d\n", CONCAT(foo, bar));

    return 0;
}
#else
#include "clings_test.h"

TEST(test_stringify) {
    ASSERT_STR_EQ(STRINGIFY(hello), "hello");
    ASSERT_STR_EQ(STRINGIFY(foo_bar), "foo_bar");
}

TEST(test_concat) {
    int foobar = 42;
    ASSERT_EQ(CONCAT(foo, bar), 42);

    int test123 = 99;
    ASSERT_EQ(CONCAT(test, 123), 99);
}

TEST(test_debug_var) {
    int some_var = 7;
    DEBUG_VAR(some_var);
}

int main(void) {
    RUN_TEST(test_stringify);
    RUN_TEST(test_concat);
    RUN_TEST(test_debug_var);
    TEST_REPORT();
}
#endif
