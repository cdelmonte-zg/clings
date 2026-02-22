// memory3.c - Double free and use-after-free
//
// This program implements a 2D matrix using dynamically allocated arrays.
// There are memory management bugs: incomplete cleanup in matrix_destroy
// leads to memory leaks, and failing to NULL out pointers after free
// creates use-after-free risks. Fix them!

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **data;
    int rows;
    int cols;
} Matrix;

Matrix *matrix_create(int rows, int cols) {
    Matrix *m = malloc(sizeof(Matrix));
    if (!m) return NULL;
    m->rows = rows;
    m->cols = cols;

    if (rows == 0) {
        m->data = NULL;
        return m;
    }

    m->data = malloc(sizeof(int *) * (size_t)rows);
    if (!m->data) {
        free(m);
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(sizeof(int) * (size_t)cols);
        if (!m->data[i]) {
            for (int j = 0; j < i; j++) {
                free(m->data[j]);
            }
            free(m->data);
            free(m);
            return NULL;
        }
        for (int c = 0; c < cols; c++) {
            m->data[i][c] = 0;
        }
    }

    return m;
}

void matrix_set(Matrix *m, int r, int c, int val) {
    if (r >= 0 && r < m->rows && c >= 0 && c < m->cols) {
        m->data[r][c] = val;
    }
}

int matrix_get(const Matrix *m, int r, int c) {
    if (r >= 0 && r < m->rows && c >= 0 && c < m->cols) {
        return m->data[r][c];
    }
    return -1;
}

void matrix_destroy(Matrix *m) {
    if (!m) return;
    // BUG: This only frees the array of row pointers, but not the
    // individual row arrays! Each m->data[i] was allocated with malloc
    // and needs its own free().
    free(m->data);
    free(m);
    // BUG: After freeing, the pointer fields still hold stale addresses.
    // If someone accidentally calls matrix_destroy again (double free)
    // or accesses the matrix after destroy (use-after-free), bad things
    // happen. NULL out pointers after freeing to make it safer.
}

#ifndef TEST
int main(void) {
    Matrix *m = matrix_create(3, 3);
    if (!m) {
        printf("Allocation failed!\n");
        return 1;
    }

    matrix_set(m, 0, 0, 1);
    matrix_set(m, 1, 1, 5);
    matrix_set(m, 2, 2, 9);

    printf("Matrix 3x3:\n");
    for (int r = 0; r < m->rows; r++) {
        for (int c = 0; c < m->cols; c++) {
            printf(" %3d", matrix_get(m, r, c));
        }
        printf("\n");
    }

    matrix_destroy(m);
    return 0;
}
#else
#include "clings_test.h"

TEST(test_create) {
    Matrix *m = matrix_create(3, 3);
    ASSERT(m != NULL);
    ASSERT(m->data != NULL);
    ASSERT_EQ(m->rows, 3);
    ASSERT_EQ(m->cols, 3);
    matrix_destroy(m);
}

TEST(test_set_and_get) {
    Matrix *m = matrix_create(3, 3);
    matrix_set(m, 0, 0, 1);
    matrix_set(m, 1, 1, 5);
    matrix_set(m, 2, 2, 9);
    ASSERT_EQ(matrix_get(m, 0, 0), 1);
    ASSERT_EQ(matrix_get(m, 1, 1), 5);
    ASSERT_EQ(matrix_get(m, 2, 2), 9);
    // Unset cells should be zero-initialized
    ASSERT_EQ(matrix_get(m, 0, 1), 0);
    ASSERT_EQ(matrix_get(m, 1, 0), 0);
    matrix_destroy(m);
}

TEST(test_bounds_check) {
    Matrix *m = matrix_create(3, 3);
    ASSERT_EQ(matrix_get(m, -1, 0), -1);
    ASSERT_EQ(matrix_get(m, 0, -1), -1);
    ASSERT_EQ(matrix_get(m, 3, 0), -1);
    ASSERT_EQ(matrix_get(m, 0, 3), -1);
    matrix_destroy(m);
}

TEST(test_empty_matrix) {
    Matrix *m = matrix_create(0, 0);
    ASSERT(m != NULL);
    ASSERT_EQ(m->rows, 0);
    ASSERT_EQ(m->data, NULL);
    matrix_destroy(m);
}

int main(void) {
    RUN_TEST(test_create);
    RUN_TEST(test_set_and_get);
    RUN_TEST(test_bounds_check);
    RUN_TEST(test_empty_matrix);
    TEST_REPORT();
}
#endif
