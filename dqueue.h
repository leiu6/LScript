#ifndef LS_DQUEUE_H_INCLUDED
#define LS_DQUEUE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

typedef struct LSDQueue LSDQueue;

// Initialize a queue
LSDQueue *lsdq_alloc();

// Free the queue object
void lsdq_free(LSDQueue *q);

// Add an item to the back of the queue
//
// Returns true if the item could be added
// Returns false otherwise
bool lsdq_enqueue(LSDQueue *q, double item);

// Remove an item from the front of the queue
//
// Returns true if there are items in the queue
// Returns false otherwise
bool lsdq_dequeue(LSDQueue *q, double *out_num);

// Get the current number of items in the queue
size_t lsdq_size(const LSDQueue *q);

// Determine if the queue is empty or not
//
// Returns true if the queue is empty
// Returns false otherwise
bool lsdq_isempty(const LSDQueue *q);

#endif // LS_QUEUE_H_INCLUDED
