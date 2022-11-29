#ifndef HEAP_C
#define HEAP_C

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

/* Default attributes */
/* MUST BE POSITIVE NON-ZERO */
#define MIN_HEAP_CEILING 16
#define DEBUGMODE

/* Private function prototypes */
void _heap_grow(heap_t *);
int _heap_has_member(heap_t *, int);
void _heap_in_order_print(heap_t *);
void _heap_iop_rec(heap_t *, int, int);
heap_node_t *_heap_node_construct(int, data_t);
void _heap_shuffle_down(heap_t *, int);
void _heap_shuffle_up(heap_t *, int);
void _heap_swap(heap_t *, int, int);

/* Constructors & Destructors */
heap_t *heap_construct(int initial_ceiling){
    int heap_ceiling = initial_ceiling > MIN_HEAP_CEILING ? initial_ceiling : MIN_HEAP_CEILING;
    /* Allocate all space for structs */
    heap_t *ret_ptr = (heap_t *)malloc(sizeof(heap_t));
    ret_ptr->attribs = (heap_header_t *)malloc(sizeof(heap_header_t));
    /* Offsetting address establishes first index of 1, for easy sequential access */
    ret_ptr->root = (heap_node_t **)malloc(sizeof(heap_node_t *) * heap_ceiling) - 1;
    
    /* Set initial heap attributes */
    ret_ptr->attribs->size = 0;
    ret_ptr->attribs->ceiling = heap_ceiling;
    
    return ret_ptr;
}

void heap_destruct(heap_t **trash_heap){
    int node_index;
    if(*trash_heap){
        for(node_index = (*trash_heap)->attribs->size; node_index > 0; node_index--)
            free((*trash_heap)->root[node_index]);/*NOT zero inclusive, for sequential offset*/
        free((*trash_heap)->root + 1);/* Reallign to true root */
        free((*trash_heap)->attribs);
        free(*trash_heap);
        *trash_heap = NULL;
    }
}

heap_node_t *_heap_node_construct(int priority, data_t data_ptr){
    heap_node_t *return_ptr = (heap_node_t *)malloc(sizeof(heap_node_t));
    return_ptr->priority = priority;
    return_ptr->data_ptr = data_ptr;
    return return_ptr;
}


/* Insert/Remove functions */
void heap_push(heap_t *heap_ptr, int priority, data_t data_ptr){
    int index;
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    
    if(heap_is_full(heap_ptr)){
        _heap_grow(heap_ptr);
    }
    
    index = ++heap_ptr->attribs->size;
    heap_ptr->root[index] = _heap_node_construct(priority, data_ptr);
    
    _heap_shuffle_up(heap_ptr, index);
}

void _heap_shuffle_up(heap_t *heap_ptr, int index){
    int parent_index;
    if(index > 1){
        parent_index = index / 2;
        if(heap_ptr->root[parent_index]->priority < heap_ptr->root[index]->priority)
            _heap_swap(heap_ptr, parent_index, index);
        _heap_shuffle_up(heap_ptr, parent_index);
    }
    /* Implied base case, index == 1, root of tree*/
}

data_t *heap_pop(heap_t *heap_ptr){
    return NULL;
}


/* Organizational Functions */
/* My end goal is to implement a graph path finding algorithm.
 * This function will likely be unneccessary. It may be
 * on hold for a long time.
 *
 * Idea for implementation, start with last addition, work back
 * until out of leaves (2i <= n). For each leaf, 'bubble up'
*/
void heapify(heap_t *heap_ptr){
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif    
}

void _heap_grow(heap_t *heap_ptr){
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    if(heap_ptr){
        heap_ptr->attribs->ceiling <<= 1;/*fast x2*/
        heap_ptr->root = (heap_node_t **)realloc((void *)(heap_ptr->root[1]), sizeof(heap_node_t *) * heap_ptr->attribs->ceiling);
    }
}

void _heap_swap(heap_t *heap_ptr, int index1, int index2){
    heap_node_t *hold_node;

    if(heap_ptr){
        hold_node = heap_ptr->root[index1];
        heap_ptr->root[index1] = heap_ptr->root[index2];
        heap_ptr->root[index2] = hold_node;
    }
}

/* Status functions */
int heap_is_empty(heap_t *heap_ptr){
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    return heap_ptr && heap_ptr->attribs->size <= 0;/*just in case a negative gets through*/
}

int heap_is_full(heap_t *heap_ptr){
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    return heap_ptr && heap_ptr->attribs->size >= heap_ptr->attribs->ceiling;/* >=, just in case */
}

/* Return a boolean true if a node with that index [1:n] exists in the heap
 * Prevents OOB memory access when traversing sequential tree. Used to validate
 * child's existence, like any good parent should.
*/
int _heap_has_member(heap_t *heap_ptr, int inorder_node_index){
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    return heap_ptr && 0 < inorder_node_index && inorder_node_index <= heap_ptr->attribs->size;
}


/* Debug Functions */
void heap_print(heap_t *heap_ptr){
    if(heap_ptr){
        if(heap_ptr->attribs->size > 0){
            printf("\nHeap has %d / %d members.\n\n", heap_ptr->attribs->size, heap_ptr->attribs->ceiling);
            _heap_in_order_print(heap_ptr);
        } else{
            printf("\nHeap empty. Space for %d node links (%d bytes).\n\n", heap_ptr->attribs->ceiling, heap_ptr->attribs->ceiling * (int)sizeof(heap_node_t *));
        }
    } else {
        printf("\nTree not found (NULL pointer).\n\n");
    }
}

void _heap_in_order_print(heap_t *heap_ptr){
    _heap_iop_rec(heap_ptr, 1, 0);
    printf("\n\n");
}

void _heap_iop_rec(heap_t *heap_ptr, int parent_index, int level){
    int left_index, right_index;
    
    left_index = 2 * parent_index;
    right_index = left_index + 1;
    
    /* right */
    if(_heap_has_member(heap_ptr, right_index))
        _heap_iop_rec(heap_ptr, right_index, level + 1);
    
    /* Print this node's value */
    for(int i = 0; i <= level; i++)
        printf("\t");
    printf("%d%s\n", (heap_ptr->root[parent_index])->priority, level == 0 ? " (Root)" : "");
    
    /* left */
    if(_heap_has_member(heap_ptr, left_index))
        _heap_iop_rec(heap_ptr, left_index, level + 1);
    
}

#endif
