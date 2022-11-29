#ifndef HEAP_H
#define HEAP_H

/*  heap.h
 *  Timothy Feusse
 *  Created: Nov, 28, 2022
 *
 *  This is the header file for a heap data type.
 *  Initially this will only work for a low-priority sequential heap, but
 *  more complex heap structures may be added in the future.
 *
 *  Five core functions are taken from Standish's model for a PQ:
 *      construct (+destruct), push, pop, check full, check empty
*/

/* Data storage definition */
typedef int data_t;
 
typedef struct heap_header_struct{
    int size;
    int ceiling; /* The maximum size of seq. array */
} heap_header_t;
 
typedef struct heap_node_struct{
    int priority;
    data_t data_ptr;
} heap_node_t;

typedef struct heap_struct{
    heap_header_t *attribs;
    void *root; /* This will point to the memory allocation */
    heap_node_t **data; /* The 1 initial indexed data pointer */
} heap_t;


/* Constructors & Destructors */
heap_t *heap_construct(int);
void heap_destruct(heap_t **);

/* Insert/Remove functions */
void heap_push(heap_t *, int, data_t);
data_t *heap_pop(heap_t *);

/* Organizational Functions */
void heapify(heap_t *);

/* Status functions */
int heap_is_empty(heap_t *);
int heap_is_full(heap_t *);

/* Debug Functions */
void heap_print(heap_t *);

#endif
