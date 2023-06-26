#ifndef LS_OBJ_H_INCLUDED
#define lS_OBJ_H_INCLUDED

#include "defs.h"
#include "mat.h"

#include <stdbool.h>

#define LSOBJTYPES                  \
    LSOBJ(LSOBJ_NULL),              \
    LSOBJ(LSOBJ_NUMBER),            \
    LSOBJ(LSOBJ_BOOL),              \
    LSOBJ(LSOBJ_STRING),            \
    LSOBJ(LSOBJ_MATRIX)

#define LSOBJ(name) name
typedef enum {
    LSOBJTYPES
} LSObjType;
#undef LSOBJ

// This is the base type for an object in LScript
typedef struct {
    LSObjType type;
    union {
        double number;
        bool boolean;
        char *string;
        LSMat *matrix;
    } value;
} LSObj;

// Allocate a null LSObj
LSObj *lsobj_alloc_null();

// Allocate a numerical LSObj
LSObj *lsobj_alloc_number(double number);

// Allocate a string LSObj
LSObj *lsobj_alloc_string(const char *str);

// Allocate a boolean LSObj
LSObj *lsobj_alloc_bool(bool boolean);

// Allocate a matrix LSObj
LSObj *lsobj_alloc_matrix(LSMat *m);

// Free an LSObj
void lsobj_free(LSObj *o);

// Print an LSObj to the terminal
void lsobj_print(const LSObj *o);

#endif // LS_OBJ_H_INCLUDED
