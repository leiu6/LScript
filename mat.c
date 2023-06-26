#include "mat.h"

#include <assert.h>
#include <stdio.h>

LSMat *mat_alloc(size_t rows, size_t cols) {
    LSMat *m = malloc(sizeof(LSMat));
    if (!m)
        return NULL;

    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    if (!m->data) {
        free(m);
        return NULL;
    }

    return m;
}

LSMat *mat_init(size_t rows, size_t cols, double fill) {
    LSMat *m = mat_alloc(rows, cols);
    if (!m) {
        return NULL;
    }

    for (size_t i = 0; i < rows * cols; i++)
        m->data[i] = fill;

    return m;
}

LSMat *mat_eye(size_t rows, size_t cols) {
    size_t min = rows < cols ? rows : cols;
    LSMat *m = mat_init(rows, cols, 0.0);
    if (!m) return NULL;

    for (size_t i = 0; i < min; i++)
        mat_set(m, i, i, 1.0);
    
    return m;
}

void mat_free(LSMat *m) {
    assert(m != NULL);
    assert(m->data != NULL);

    free(m);
    free(m->data);
}

void mat_set(LSMat *m, size_t row, size_t col, double value) {
    assert(m != NULL);
    assert(row < m->rows);
    assert(col < m->cols);

    m->data[m->rows * col + row] = value;
}

double mat_get(const LSMat *m, size_t row, size_t col) {
    assert(m != NULL);
    assert(row < m->rows);
    assert(col < m->cols);

    return m->data[m->rows * col + row];
}

double *mat_get_ptr(LSMat *m, size_t row, size_t col) {
    assert(m != NULL);
    assert(row < m->rows);
    assert(col < m->cols);

    return &m->data[m->rows * col + row];
}

LSMat *mat_add(LSMat *a, LSMat *b) {
    assert(a != NULL);
    assert(b != NULL);
    assert(a->rows == b->rows);
    assert(a->cols == b->cols);

    LSMat *m = mat_alloc(a->rows, a->cols);
    if (!m) return NULL;

    for (size_t i = 0; i < a->rows * a->cols; i++)
        m->data[i] = a->data[i] + b->data[i];
    
    return m;
}

LSMat *mat_subtract(LSMat *a, LSMat *b) {
    assert(a != NULL);
    assert(b != NULL);
    assert(a->rows == b->rows);
    assert(a->cols == b->cols);

    LSMat *m = mat_alloc(a->rows, a->cols);
    if (!m) return NULL;

    for (size_t i = 0; i < a->rows * a->cols; i++)
        m->data[i] = a->data[i] - b->data[i];
    
    return m;
}

void mat_negate(LSMat *m) {
    assert(m != NULL);

    for (size_t i = 0; i < m->rows * m->cols; i++)
        m->data[i] = -m->data[i];
}

LSMat *mat_mult(LSMat *a, LSMat *b) {
    assert(a != NULL);
    assert(b != NULL);
    assert(a->cols == b->rows);

    LSMat *m = mat_alloc(a->rows, b->cols);
    if (!m) return NULL;

    for (size_t row = 0; row < a->rows; row++) {
        for (size_t col = 0; col < b->cols; col++) {
            double dot_product = 0.0;

            for (size_t i = 0; i < a->cols; i++) {
                dot_product += mat_get(a, row, i) * mat_get(b, i, col);
            }

            mat_set(m, row, col, dot_product);
        }
    }

    return m;
}

void mat_print(const LSMat *m) {
    assert(m != NULL);

    printf("LSMat: %zux%zu\n", m->rows, m->cols);
    
    printf("[");
    for (size_t row = 0; row < m->rows; row++) {
        if (row != 0)
            printf(" ");

        printf("[");
        for (size_t col = 0; col < m->cols; col++) {
            printf("%f", mat_get(m, row, col));

            if (col != m->cols - 1)
                printf(", ");
        }
        printf("]");

        if (row != m->rows - 1)
            printf(",\n");
    }
    printf("]\n");
}
