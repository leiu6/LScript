#include "ls-obj.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

static const char *lsobj_type_as_string(LSObjType type) {
#define LSOBJ(name) #name
    const char *typestrings[] = {LSOBJTYPES};
#undef LSOBJ

    return typestrings[type];
}

LSObj *lsobj_alloc_null() {
    LSObj *o = malloc(sizeof(LSObj));
    if (!o) return NULL;

    o->type = LSOBJ_NULL;
    
    return o;
}

LSObj *lsobj_alloc_number(double number) {
    LSObj *o = malloc(sizeof(LSObj));
    if (!o) return NULL;

    o->type = LSOBJ_NUMBER;
    o->value.number = number;

    return o;
}

LSObj *lsobj_alloc_string(const char *str) {
    LSObj *o = malloc(sizeof(LSObj));
    if (!o) return NULL;

    o->type = LSOBJ_STRING;
    o->value.string = malloc(sizeof(str) + 1);
    if (!o->value.string) return NULL;
    strncpy(o->value.string, str, strlen(str) + 1);

    return o;
}

LSObj *lsobj_alloc_bool(bool boolean) {
    LSObj *o = malloc(sizeof(LSObj));
    if (!o) return NULL;

    o->type = LSOBJ_BOOL;
    o->value.boolean = boolean;

    return o;
}

LSObj *lsobj_alloc_matrix(LSMat *m) {
    assert(m != NULL);

    LSObj *o = malloc(sizeof(LSObj));
    if (!o) return NULL;

    o->type = LSOBJ_MATRIX;
    o->value.matrix = m;

    return o;
}

void lsobj_free(LSObj *o) {
    assert(o != NULL);

    switch (o->type) {
    case LSOBJ_NULL:
    case LSOBJ_BOOL:
    case LSOBJ_NUMBER:
        free(o);
        break;
    case LSOBJ_STRING:
        free(o->value.string);
        free(o);
        break;
    case LSOBJ_MATRIX:
        mat_free(o->value.matrix);
        free(o);
        break;
    }
}

void lsobj_print(const LSObj *o) {
    printf("LSObj: \n");
    printf("Type: %s\n", lsobj_type_as_string(o->type));

    switch (o->type) {
    case LSOBJ_NULL:
        printf("Value: NULL\n");
        break;
    case LSOBJ_NUMBER:
        printf("Value: %f\n", o->value.number);
        break;
    case LSOBJ_BOOL:
        if (o->value.boolean)
            printf("Value: true\n");
        else
            printf("Value: false\n");
        break;
    case LSOBJ_STRING:
        printf("Value: %s\n", o->value.string);
        break;
    case LSOBJ_MATRIX:
        mat_print(o->value.matrix);
        break;
    }
}
