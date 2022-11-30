#ifndef HEAP_H
#define HEAP_H

/*  heap.h
 *  Timothy Feusse
 *  Created: Nov, 28, 2022
 *  Updated: Nov, 29, 2022
 *
 *  This is a sequential heap storage data type. It is capable of prioritizing
 *  low or high keys as selected by the user upon construction. The ADT
 *  dynamically manages memory, and provides a settable minimum size to 
 *  optimize performance. An initial memory size can be chosen upon creation
 *  as a further optimization.
 *
 *  The internal structure should gaurantee better than O(n) performance
 *  for all operations other than 'heapification', destruction, and possibly
 * insertions&removals that dynamically resize the structure (realloc calls).
 *
 *  Five core functions are taken from Standish's model for a priority queue:
 *      construction (+destruction), push, pop, check if full, check if empty
 *  These provide a simple interface for a self-managing heap ADT.
 *
 *  Original Description:
 *  This is the header file for a heap data type.
 *  <XXInitially this will only work for a low-priority sequential heap, but
 *  more complex heap structures may be added in the future.XX>
 *  Nov, 29, 2022: This now works for both high and low priority sequential heaps.
 *
*/

/* Defines the minimum size of newly made heaps */
#define MIN_HEAP_CEILING 16

/* Data storage definition. Must be some object pointer */
typedef int *data_t;

/* Used for determining sort/priority type */
typedef enum heap_priority_enum {LOW_PRIORITY = -1, HIGH_PRIORITY = 1} heap_priority_t;

typedef struct heap_header_struct{
    int size;
    int ceiling; /* The maximum size of seq. array / allocated memory */
    heap_priority_t heap_priority;
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
heap_t *heap_construct(int, heap_priority_t);

void heap_destruct(heap_t **);

/* Insert/Remove functions */
void heap_push(heap_t *, int, data_t);
data_t heap_pop(heap_t *);

/* Organizational Functions */
void heapify(heap_t *); /* Currently unimplemented */

/* Status functions */
int heap_is_empty(heap_t *);
int heap_is_full(heap_t *);

/* Debug Functions - Remove this section for 'release' */
void heap_print(heap_t *);
void debug_check_heap(heap_t *);
heap_t *debug_quick_heap(int, int, heap_priority_t);
void debug_spoof_node(heap_t *, int, int, data_t);
/* */

#endif
