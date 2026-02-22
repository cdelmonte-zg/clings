// structs2.c - Solution
//
// Fixes:
// 1. stack_push: use a temporary pointer for realloc to avoid losing
//    the original data pointer on failure
// 2. stack_pop: check for empty stack before decrementing top

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

Stack *stack_create(int capacity) {
    Stack *s = malloc(sizeof(Stack));
    if (!s) return NULL;
    s->data = malloc(sizeof(int) * (size_t)capacity);
    if (!s->data) {
        free(s);
        return NULL;
    }
    s->top = 0;
    s->capacity = capacity;
    return s;
}

void stack_push(Stack *s, int value) {
    if (s->top >= s->capacity) {
        s->capacity *= 2;
        int *tmp = realloc(s->data, sizeof(int) * (size_t)s->capacity);
        if (!tmp) return;
        s->data = tmp;
    }
    s->data[s->top] = value;
    s->top++;
}

int stack_pop(Stack *s) {
    if (s->top == 0) return -1;
    s->top--;
    return s->data[s->top];
}

int stack_peek(const Stack *s) {
    if (s->top == 0) return -1;
    return s->data[s->top - 1];
}

int stack_size(const Stack *s) {
    return s->top;
}

void stack_destroy(Stack *s) {
    if (!s) return;
    free(s->data);
    free(s);
}

#ifndef TEST
int main(void) {
    Stack *s = stack_create(4);
    stack_push(s, 10);
    stack_push(s, 20);
    stack_push(s, 30);

    printf("size: %d\n", stack_size(s));
    printf("peek: %d\n", stack_peek(s));
    printf("pop:  %d\n", stack_pop(s));
    printf("pop:  %d\n", stack_pop(s));
    printf("size: %d\n", stack_size(s));

    stack_destroy(s);
    return 0;
}
#else
#include "clings_test.h"

TEST(test_create_and_destroy) {
    Stack *s = stack_create(4);
    ASSERT(s != NULL);
    ASSERT_EQ(stack_size(s), 0);
    stack_destroy(s);
}

TEST(test_push_pop) {
    Stack *s = stack_create(4);
    stack_push(s, 10);
    stack_push(s, 20);
    stack_push(s, 30);
    ASSERT_EQ(stack_size(s), 3);
    ASSERT_EQ(stack_pop(s), 30);
    ASSERT_EQ(stack_pop(s), 20);
    ASSERT_EQ(stack_pop(s), 10);
    ASSERT_EQ(stack_size(s), 0);
    stack_destroy(s);
}

TEST(test_peek) {
    Stack *s = stack_create(4);
    ASSERT_EQ(stack_peek(s), -1);
    stack_push(s, 42);
    ASSERT_EQ(stack_peek(s), 42);
    ASSERT_EQ(stack_size(s), 1);
    stack_destroy(s);
}

TEST(test_empty_pop) {
    Stack *s = stack_create(4);
    ASSERT_EQ(stack_pop(s), -1);
    ASSERT_EQ(stack_size(s), 0);
    stack_destroy(s);
}

TEST(test_growth) {
    Stack *s = stack_create(2);
    for (int i = 0; i < 20; i++) {
        stack_push(s, i * 10);
    }
    ASSERT_EQ(stack_size(s), 20);
    ASSERT_EQ(stack_pop(s), 190);
    ASSERT_EQ(stack_pop(s), 180);
    stack_destroy(s);
}

int main(void) {
    RUN_TEST(test_create_and_destroy);
    RUN_TEST(test_push_pop);
    RUN_TEST(test_peek);
    RUN_TEST(test_empty_pop);
    RUN_TEST(test_growth);
    TEST_REPORT();
}
#endif
