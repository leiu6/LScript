#ifndef LS_MAT_H_INCLUDED
#define LS_MAT_H_INCLUDED

#include <stdlib.h>

typedef struct {
    size_t rows, cols;
    double *data;
} LSMat;

LSMat *mat_alloc(size_t rows, size_t cols);

LSMat *mat_init(size_t rows, size_t cols, double fill);

LSMat *mat_eye(size_t rows, size_t cols);

void mat_free(LSMat *m);

/* Basic Accessors */
void mat_set(LSMat *m, size_t row, size_t col, double value);
double mat_get(const LSMat *m, size_t row, size_t col);
double *mat_get_ptr(LSMat *m, size_t row, size_t col);

/* Mathematical Operations */
LSMat *mat_add(LSMat *a, LSMat *b);
LSMat *mat_subtract(LSMat *a, LSMat *b);
void mat_negate(LSMat *m);
LSMat *mat_mult(LSMat *a, LSMat *b);

/* Debug */
void mat_print(const LSMat *m);

#endif // LS_MAT_H_INCLUDED
