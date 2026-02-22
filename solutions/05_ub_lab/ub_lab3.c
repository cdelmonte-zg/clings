// ub_lab3.c - Solution
//
// Fix: save the index instead of a pointer. After the realloc (triggered
// by intvec_push), recompute the pointer using the index.

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int count;
    int capacity;
} IntVec;

IntVec *intvec_new(int capacity) {
    IntVec *v = malloc(sizeof(IntVec));
    if (!v) return NULL;
    v->data = malloc(sizeof(int) * (size_t)capacity);
    if (!v->data) {
        free(v);
        return NULL;
    }
    v->count = 0;
    v->capacity = capacity;
    return v;
}

void intvec_push(IntVec *v, int value) {
    if (v->count >= v->capacity) {
        v->capacity *= 2;
        int *new_data = realloc(v->data, sizeof(int) * (size_t)v->capacity);
        if (new_data) {
            v->data = new_data;
        }
    }
    v->data[v->count] = value;
    v->count++;
}

void intvec_free(IntVec *v) {
    free(v->data);
    free(v);
}

int sum_with_saved_pointer(IntVec *v, int index, int num_extra) {
    /* FIX: save the index, not a pointer */
    int saved_index = index;

    int old_count = v->count;
    for (int i = 0; i < num_extra; i++) {
        intvec_push(v, (i + 1) * 10);
    }

    /* FIX: recompute the pointer from the (still-valid) index */
    int sum = v->data[saved_index];

    for (int i = old_count; i < v->count; i++) {
        sum += v->data[i];
    }
    return sum;
}

#ifndef TEST
int main(void) {
    IntVec *v = intvec_new(2);
    intvec_push(v, 100);
    intvec_push(v, 200);

    int result = sum_with_saved_pointer(v, 0, 4);
    printf("Sum: %d\n", result);

    intvec_free(v);
    return 0;
}
#else
#include "clings_test.h"

TEST(test_basic_push) {
    IntVec *v = intvec_new(4);
    intvec_push(v, 10);
    intvec_push(v, 20);
    ASSERT_EQ(v->count, 2);
    ASSERT_EQ(v->data[0], 10);
    ASSERT_EQ(v->data[1], 20);
    intvec_free(v);
}

TEST(test_push_triggers_realloc) {
    IntVec *v = intvec_new(2);
    intvec_push(v, 1);
    intvec_push(v, 2);
    intvec_push(v, 3);
    ASSERT_EQ(v->count, 3);
    ASSERT_EQ(v->data[0], 1);
    ASSERT_EQ(v->data[1], 2);
    ASSERT_EQ(v->data[2], 3);
    intvec_free(v);
}

TEST(test_sum_no_realloc) {
    IntVec *v = intvec_new(8);
    intvec_push(v, 100);
    intvec_push(v, 200);
    int result = sum_with_saved_pointer(v, 0, 2);
    ASSERT_EQ(result, 130);
    intvec_free(v);
}

TEST(test_sum_with_realloc) {
    IntVec *v = intvec_new(2);
    intvec_push(v, 100);
    intvec_push(v, 200);
    int result = sum_with_saved_pointer(v, 0, 4);
    ASSERT_EQ(result, 200);
    intvec_free(v);
}

TEST(test_sum_second_element) {
    IntVec *v = intvec_new(2);
    intvec_push(v, 100);
    intvec_push(v, 200);
    int result = sum_with_saved_pointer(v, 1, 4);
    ASSERT_EQ(result, 300);
    intvec_free(v);
}

int main(void) {
    RUN_TEST(test_basic_push);
    RUN_TEST(test_push_triggers_realloc);
    RUN_TEST(test_sum_no_realloc);
    RUN_TEST(test_sum_with_realloc);
    RUN_TEST(test_sum_second_element);
    TEST_REPORT();
}
#endif
