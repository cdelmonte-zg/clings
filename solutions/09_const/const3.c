// const3.c - Solution
//
// Fix 1: config_create copies the full title string with snprintf.
// Fix 2: config_describe takes `const struct config *c` and does NOT
//         modify the struct — it just reads the fields.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct config {
    int width;
    int height;
    char title[64];
};

// Fixed: copies the full title string into the struct's buffer
struct config *config_create(int w, int h, const char *title) {
    struct config *c = malloc(sizeof(struct config));
    if (!c) return NULL;
    c->width = w;
    c->height = h;
    snprintf(c->title, sizeof(c->title), "%s", title);
    return c;
}

int config_width(const struct config *c) {
    return c->width;
}

int config_height(const struct config *c) {
    return c->height;
}

// Fixed: takes const pointer, does not modify the config
void config_describe(const struct config *c, char *buf, size_t buf_size) {
    snprintf(buf, buf_size, "%s: %dx%d", c->title, c->width, c->height);
}

void config_destroy(struct config *c) {
    free(c);
}

#ifndef TEST
int main(void) {
    struct config *cfg = config_create(1920, 1080, "Main Window");
    if (!cfg) return 1;

    char buf[128];
    config_describe(cfg, buf, sizeof(buf));
    printf("%s\n", buf);
    printf("Width: %d, Height: %d\n", config_width(cfg), config_height(cfg));

    config_destroy(cfg);
    return 0;
}
#else
#include "clings_test.h"

TEST(test_config_create) {
    struct config *c = config_create(800, 600, "Test");
    ASSERT(c != NULL);
    ASSERT_EQ(config_width(c), 800);
    ASSERT_EQ(config_height(c), 600);
    config_destroy(c);
}

TEST(test_config_title_copied) {
    char title[32];
    snprintf(title, sizeof(title), "Dynamic");
    struct config *c = config_create(100, 200, title);
    // Modify the original — config should have its own copy
    title[0] = 'X';
    char buf[128];
    config_describe(c, buf, sizeof(buf));
    ASSERT_STR_EQ(buf, "Dynamic: 100x200");
    config_destroy(c);
}

TEST(test_config_describe_format) {
    struct config *c = config_create(1920, 1080, "Main");
    char buf[128];
    config_describe(c, buf, sizeof(buf));
    ASSERT_STR_EQ(buf, "Main: 1920x1080");
    config_destroy(c);
}

TEST(test_config_describe_does_not_modify) {
    struct config *c = config_create(640, 480, "Window");
    char buf[128];
    config_describe(c, buf, sizeof(buf));
    // After describe, the config should be unchanged
    ASSERT_EQ(config_width(c), 640);
    ASSERT_EQ(config_height(c), 480);
    config_destroy(c);
}

TEST(test_config_small_buffer) {
    struct config *c = config_create(1920, 1080, "Main");
    char buf[8];
    config_describe(c, buf, sizeof(buf));
    // Should be truncated but not overflow
    ASSERT(strlen(buf) == 7);
    config_destroy(c);
}

int main(void) {
    RUN_TEST(test_config_create);
    RUN_TEST(test_config_title_copied);
    RUN_TEST(test_config_describe_format);
    RUN_TEST(test_config_describe_does_not_modify);
    RUN_TEST(test_config_small_buffer);
    TEST_REPORT();
}
#endif
