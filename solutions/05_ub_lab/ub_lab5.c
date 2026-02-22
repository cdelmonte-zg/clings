// ub_lab5.c - Solution
//
// Fix: initialize all fields after malloc. We zero the entire struct
// with memset before populating the known fields, ensuring no field
// is left uninitialized.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    int age;
    int score;
    int active;
} Student;

Student *student_new(const char *name, int age) {
    Student *s = malloc(sizeof(Student));
    if (!s) return NULL;

    /* FIX: zero-initialize everything first */
    memset(s, 0, sizeof(*s));

    size_t len = strlen(name);
    if (len >= sizeof(s->name)) {
        len = sizeof(s->name) - 1;
    }
    memcpy(s->name, name, len);
    s->name[len] = '\0';

    s->age = age;

    return s;
}

void student_print(const Student *s) {
    printf("Name: %s, Age: %d, Score: %d, Active: %s\n",
           s->name, s->age, s->score,
           s->active ? "yes" : "no");
}

int student_effective_score(const Student *s) {
    if (s->active) {
        return s->score;
    }
    return 0;
}

void student_free(Student *s) {
    free(s);
}

#ifndef TEST
int main(void) {
    Student *s = student_new("Alice", 20);
    if (!s) return 1;

    student_print(s);
    printf("Effective score: %d\n", student_effective_score(s));

    student_free(s);
    return 0;
}
#else
#include "clings_test.h"

TEST(test_create) {
    Student *s = student_new("Bob", 25);
    ASSERT(s != NULL);
    ASSERT_STR_EQ(s->name, "Bob");
    ASSERT_EQ(s->age, 25);
    student_free(s);
}

TEST(test_default_score) {
    Student *s = student_new("Charlie", 30);
    ASSERT(s != NULL);
    ASSERT_EQ(s->score, 0);
    student_free(s);
}

TEST(test_default_active) {
    Student *s = student_new("Diana", 22);
    ASSERT(s != NULL);
    ASSERT_EQ(s->active, 0);
    student_free(s);
}

TEST(test_effective_score_inactive) {
    Student *s = student_new("Eve", 19);
    ASSERT(s != NULL);
    ASSERT_EQ(student_effective_score(s), 0);
    student_free(s);
}

TEST(test_effective_score_active) {
    Student *s = student_new("Frank", 21);
    ASSERT(s != NULL);
    s->active = 1;
    s->score = 95;
    ASSERT_EQ(student_effective_score(s), 95);
    student_free(s);
}

TEST(test_long_name_truncated) {
    Student *s = student_new("Abcdefghijklmnopqrstuvwxyz1234567890", 18);
    ASSERT(s != NULL);
    ASSERT_EQ(strlen(s->name), 31u);
    student_free(s);
}

int main(void) {
    RUN_TEST(test_create);
    RUN_TEST(test_default_score);
    RUN_TEST(test_default_active);
    RUN_TEST(test_effective_score_inactive);
    RUN_TEST(test_effective_score_active);
    RUN_TEST(test_long_name_truncated);
    TEST_REPORT();
}
#endif
