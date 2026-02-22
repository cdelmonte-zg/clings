// memory1.c - Solution
//
// Fixes:
// 1. Added NULL checks after malloc
// 2. Free each individual string before freeing the items array

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    int count;
    int capacity;
} StringList;

char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

StringList *stringlist_new(int capacity) {
    StringList *list = malloc(sizeof(StringList));
    if (!list) return NULL;
    list->items = malloc(sizeof(char *) * (size_t)capacity);
    if (!list->items) {
        free(list);
        return NULL;
    }
    list->count = 0;
    list->capacity = capacity;
    return list;
}

void stringlist_add(StringList *list, const char *str) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->items = realloc(list->items, sizeof(char *) * (size_t)list->capacity);
    }
    list->items[list->count] = my_strdup(str);
    list->count++;
}

void stringlist_print(const StringList *list) {
    for (int i = 0; i < list->count; i++) {
        printf("  [%d] %s\n", i, list->items[i]);
    }
}

void stringlist_free(StringList *list) {
    for (int i = 0; i < list->count; i++) {
        free(list->items[i]);
    }
    free(list->items);
    free(list);
}

#ifndef TEST
int main(void) {
    StringList *list = stringlist_new(2);
    stringlist_add(list, "hello");
    stringlist_add(list, "advanced");
    stringlist_add(list, "C");

    stringlist_print(list);
    stringlist_free(list);

    return 0;
}
#else
#include "clings_test.h"

TEST(test_create) {
    StringList *list = stringlist_new(2);
    ASSERT(list != NULL);
    ASSERT(list->items != NULL);
    stringlist_free(list);
}

TEST(test_add_and_grow) {
    StringList *list = stringlist_new(2);
    stringlist_add(list, "alpha");
    stringlist_add(list, "beta");
    ASSERT_EQ(list->count, 2);

    stringlist_add(list, "gamma");
    ASSERT_EQ(list->count, 3);

    ASSERT_STR_EQ(list->items[0], "alpha");
    ASSERT_STR_EQ(list->items[1], "beta");
    ASSERT_STR_EQ(list->items[2], "gamma");

    stringlist_free(list);
}

int main(void) {
    RUN_TEST(test_create);
    RUN_TEST(test_add_and_grow);
    TEST_REPORT();
}
#endif
