#include "dqueue.h"

#include <assert.h>
#include <string.h>

#define INITIAL_SIZE 64
#define SIZE_INCREMENT INITIAL_SIZE

typedef char byte;

struct LSDQueue {
    size_t num_elements;
    size_t capacity;
    double *data;
};

static bool lsdq_expand(LSDQueue *q) {
    assert(q != NULL);

    double *tptr = realloc(q->data, q->capacity + SIZE_INCREMENT);
    if (!tptr) return false;
    q->data = tptr;

    return true;
}

LSDQueue *lsdq_alloc() {
    LSDQueue *q = malloc(sizeof(LSDQueue));
    if (!q) return NULL;

    q->capacity = INITIAL_SIZE;
    q->num_elements = 0;
    q->data = malloc(sizeof(double) * INITIAL_SIZE);
    if (!q->data) {
        free(q);
        return NULL;
    }

    return q;
}

void lsdq_free(LSDQueue *q) {
    assert(q != NULL);

    free(q->data);
    free(q);
}

bool lsdq_enqueue(LSDQueue *q, double item) {
    assert(q != NULL);

    if (q->num_elements + 1 > q->capacity) {
        bool expanded = lsdq_expand(q);
        if (!expanded) return false;
    }

    q->data[q->num_elements] = item;
    q->num_elements++;

    return true;
}

bool lsdq_dequeue(LSDQueue *q, double *out_num) {
    assert(q != NULL);
    
    if (q->num_elements == 0)
        return false;

    if (out_num != NULL) {
        double num = q->data[0];
        *out_num = num;
    }
    
    for (size_t i = 1; i < q->num_elements; i++)
        q->data[i - 1] = q->data[i];
    
    q->num_elements--;

    return true;
}

size_t lsdq_size(const LSDQueue *q) {
    assert(q != NULL);

    return q->num_elements;
}

bool lsdq_isempty(const LSDQueue *q) {
    assert(q != NULL);

    size_t size = lsdq_size(q);

    return size == 0 ? true : false;
}
