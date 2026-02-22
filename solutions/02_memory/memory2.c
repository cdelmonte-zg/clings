// memory2.c - Solution
//
// Fixes:
// 1. Initialize count to 0 and capacity to initial_cap in dynarray_create
// 2. Use a temporary pointer for realloc to avoid losing data on failure

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    int count;
    int capacity;
} DynArray;

DynArray *dynarray_create(int initial_cap) {
    DynArray *da = malloc(sizeof(DynArray));
    if (!da) return NULL;
    da->items = malloc(sizeof(int) * (size_t)initial_cap);
    if (!da->items) {
        free(da);
        return NULL;
    }
    da->count = 0;
    da->capacity = initial_cap;
    return da;
}

void dynarray_push(DynArray *da, int value) {
    if (da->count >= da->capacity) {
        int new_cap = da->capacity * 2;
        int *tmp = realloc(da->items, sizeof(int) * (size_t)new_cap);
        if (!tmp) return;
        da->items = tmp;
        da->capacity = new_cap;
    }
    da->items[da->count] = value;
    da->count++;
}

int dynarray_get(const DynArray *da, int index) {
    if (index < 0 || index >= da->count) {
        return -1;
    }
    return da->items[index];
}

void dynarray_destroy(DynArray *da) {
    if (!da) return;
    free(da->items);
    free(da);
}

#ifndef TEST
int main(void) {
    DynArray *arr = dynarray_create(2);
    if (!arr) {
        printf("Allocation failed!\n");
        return 1;
    }

    dynarray_push(arr, 10);
    dynarray_push(arr, 20);
    dynarray_push(arr, 30);
    dynarray_push(arr, 40);
    dynarray_push(arr, 50);

    printf("DynArray contents:\n");
    for (int i = 0; i < arr->count; i++) {
        printf("  [%d] = %d\n", i, dynarray_get(arr, i));
    }

    dynarray_destroy(arr);
    return 0;
}
#else
#include "clings_test.h"

TEST(test_create) {
    DynArray *da = dynarray_create(4);
    ASSERT(da != NULL);
    ASSERT(da->items != NULL);
    ASSERT_EQ(da->count, 0);
    ASSERT_EQ(da->capacity, 4);
    dynarray_destroy(da);
}

TEST(test_push_and_get) {
    DynArray *da = dynarray_create(4);
    dynarray_push(da, 10);
    dynarray_push(da, 20);
    dynarray_push(da, 30);
    ASSERT_EQ(da->count, 3);
    ASSERT_EQ(dynarray_get(da, 0), 10);
    ASSERT_EQ(dynarray_get(da, 1), 20);
    ASSERT_EQ(dynarray_get(da, 2), 30);
    dynarray_destroy(da);
}

TEST(test_grow_beyond_capacity) {
    DynArray *da = dynarray_create(2);
    ASSERT_EQ(da->capacity, 2);
    dynarray_push(da, 100);
    dynarray_push(da, 200);
    // This push triggers a realloc
    dynarray_push(da, 300);
    ASSERT_EQ(da->count, 3);
    ASSERT(da->capacity >= 3);
    ASSERT_EQ(dynarray_get(da, 0), 100);
    ASSERT_EQ(dynarray_get(da, 1), 200);
    ASSERT_EQ(dynarray_get(da, 2), 300);
    dynarray_destroy(da);
}

TEST(test_get_out_of_bounds) {
    DynArray *da = dynarray_create(2);
    dynarray_push(da, 42);
    ASSERT_EQ(dynarray_get(da, 0), 42);
    ASSERT_EQ(dynarray_get(da, 1), -1);
    ASSERT_EQ(dynarray_get(da, -1), -1);
    dynarray_destroy(da);
}

int main(void) {
    RUN_TEST(test_create);
    RUN_TEST(test_push_and_get);
    RUN_TEST(test_grow_beyond_capacity);
    RUN_TEST(test_get_out_of_bounds);
    TEST_REPORT();
}
#endif
