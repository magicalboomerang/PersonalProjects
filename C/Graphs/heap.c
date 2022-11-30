#ifndef HEAP_C
#define HEAP_C

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/* Default attributes */
/* MUST BE POSITIVE NON-ZERO */
#define MIN_HEAP_CEILING 16
#define DEBUGMODE

/* Eventually may implement separate headers for 'release'
 * and 'development' but intend to keep a core .c file
 */
//#ifdef DEBUGMODE
//#include "debug_heap.h"
//#else
#include "heap.h"
//#endif

/* Private function prototypes */
void _heap_grow(heap_t *);
int _heap_has_member(heap_t *, int);
heap_node_t *_heap_node_construct(int, data_t);
void _heap_shrink(heap_t *);
void _heap_shuffle_down(heap_t *, int);
void _heap_shuffle_up(heap_t *, int);
void _heap_swap(heap_t *, int, int);
/* Only in debug/dev mode, prevent possible bad function calls */
#ifdef DEBUGMODE
void _debug_check_heap_rec(heap_t *, int);
void _heap_in_order_print(heap_t *);
void _heap_iop_rec(heap_t *, int, int);
#endif

/* Constructors & Destructors */
heap_t *heap_construct(int initial_ceiling, heap_priority_t heap_priority){
    int heap_ceiling = initial_ceiling > MIN_HEAP_CEILING ? initial_ceiling : MIN_HEAP_CEILING;
    /* Allocate all space for structs */
    heap_t *ret_ptr = (heap_t *)malloc(sizeof(heap_t));
    ret_ptr->attribs = (heap_header_t *)malloc(sizeof(heap_header_t));
    /* Offsetting address establishes first index of 1, for easy sequential access */
    ret_ptr->root = malloc(sizeof(heap_node_t *) * heap_ceiling);
    ret_ptr->data = (heap_node_t **)ret_ptr->root - 1;
    /* Set initial heap attributes */
    ret_ptr->attribs->size = 0;
    ret_ptr->attribs->ceiling = heap_ceiling;
    ret_ptr->attribs->heap_priority = heap_priority;
    
    return ret_ptr;
}

void heap_destruct(heap_t **trash_heap){
    int node_index;
    #ifdef DEBUGMODE
        assert(trash_heap);
    #endif
    if(trash_heap && *trash_heap){
        for(node_index = (*trash_heap)->attribs->size; node_index > 0; node_index--){
            free((*trash_heap)->data[node_index]->data_ptr);/*NOT zero inclusive, for sequential offset*/
            free((*trash_heap)->data[node_index]);
        }
        free((*trash_heap)->root);/* Reallign to true root */
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
    
    /*queue priority turns the standard high priority tree into a low priority tree
     * Will likely make this a heap attribute defined on construction.
     */    
    heap_ptr->data[index] = _heap_node_construct(priority  * heap_ptr->attribs->heap_priority, data_ptr);
    
    _heap_shuffle_up(heap_ptr, index);
}

data_t heap_pop(heap_t *heap_ptr){
    data_t return_value = NULL;
    if(heap_ptr && heap_ptr->attribs->size > 0){
        return_value = heap_ptr->data[1]->data_ptr;
        _heap_swap(heap_ptr, 1, heap_ptr->attribs->size);
        free(heap_ptr->data[heap_ptr->attribs->size--]);
        
        _heap_shuffle_down(heap_ptr, 1);
        
        /* Resize if necessary */
        if(heap_ptr->attribs->size * 4 < heap_ptr->attribs->ceiling && heap_ptr->attribs->ceiling > MIN_HEAP_CEILING)
            _heap_shrink(heap_ptr);
    }
    return return_value;
}

void _heap_shuffle_down(heap_t *heap_ptr, int index){
    int left_index = 2 * index, right_index = left_index + 1, swap_index = index;
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    if(heap_ptr){    
        if(_heap_has_member(heap_ptr, left_index))/* Left child exists */
            if(heap_ptr->data[left_index]->priority > heap_ptr->data[swap_index]->priority)
                swap_index = left_index;
        
        if(_heap_has_member(heap_ptr, right_index))/*Right Child exists*/
            if(heap_ptr->data[right_index]->priority > heap_ptr->data[swap_index]->priority)
                swap_index = right_index;
        
        if(index != swap_index){
            _heap_swap(heap_ptr, index, swap_index);
            _heap_shuffle_down(heap_ptr, swap_index);
        }
    }
}

void _heap_shuffle_up(heap_t *heap_ptr, int index){
    int parent_index;
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    if(heap_ptr && index > 1){
        parent_index = index / 2;
        if(heap_ptr->data[parent_index]->priority < heap_ptr->data[index]->priority)
            _heap_swap(heap_ptr, parent_index, index);
        _heap_shuffle_up(heap_ptr, parent_index);
    }
    /* Implied base case, index == 1, root of tree*/
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
        heap_ptr->root = (heap_node_t **)realloc((void *)heap_ptr->root, sizeof(heap_node_t *) * heap_ptr->attribs->ceiling);
        heap_ptr->data = (heap_node_t **)heap_ptr->root - 1;
    }
}

void _heap_shrink(heap_t *heap_ptr){
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    if(heap_ptr){
        heap_ptr->attribs->ceiling >>= 1;/*fast x2*/
        heap_ptr->root = (heap_node_t **)realloc((void *)heap_ptr->root, sizeof(heap_node_t *) * heap_ptr->attribs->ceiling);
        heap_ptr->data = (heap_node_t **)heap_ptr->root - 1;
    }
}

void _heap_swap(heap_t *heap_ptr, int index1, int index2){
    heap_node_t *hold_node;
    #ifdef DEBUGMODE
        assert(heap_ptr);
    #endif
    if(heap_ptr){
        hold_node = heap_ptr->data[index1];
        heap_ptr->data[index1] = heap_ptr->data[index2];
        heap_ptr->data[index2] = hold_node;
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


#ifdef DEBUGMODE
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
    /* Correct the priority according to low/high priority before print */
    printf("%d", heap_ptr->data[parent_index]->priority * heap_ptr->attribs->heap_priority);
    printf(":%p%s\n", heap_ptr->data[parent_index]->data_ptr, level == 0 ? " (Root)" : "");
    
    /* left */
    if(_heap_has_member(heap_ptr, left_index))
        _heap_iop_rec(heap_ptr, left_index, level + 1);
    
}

void debug_check_heap(heap_t *test_heap){
    if(test_heap)
        _debug_check_heap_rec(test_heap, 1);
}

void _debug_check_heap_rec(heap_t *test_heap, int index){
    int left_index = 2 * index, right_index = left_index + 1;
    if(_heap_has_member(test_heap, left_index)){
        assert(test_heap->data[index]->priority >= test_heap->data[left_index]->priority);
        _debug_check_heap_rec(test_heap, left_index);
    }
    
    if(_heap_has_member(test_heap, right_index)){
        assert(test_heap->data[index]->priority >= test_heap->data[right_index]->priority);
        _debug_check_heap_rec(test_heap, right_index);
    }
}


/* Only works in debug-mode. A public release should not have this functionality */

heap_t *debug_quick_heap(int floor, int ceiling, heap_priority_t heap_priority){
    heap_t *return_heap = heap_construct(-1, heap_priority);
    data_t temp;
    
    /* Good optimization for large auto-generated heaps (O(n logn) becomes O(n)).
     * By inserting according to low/high priority, we eliminate the need for an
     * upward sort (O(logn)) for the new addition. Notably, the opposite is the
     * worst case scenario for heap additions.
     */
    if(heap_priority == HIGH_PRIORITY){
        /* Decreasing order favors high-priority queues */
        for(int index = ceiling; index >= floor; index--){ 
            temp = (data_t)malloc(sizeof(int));
            *temp = index;
            heap_push(return_heap, index, temp);
        }
    } else {
        /* Increasing order favors low-priority queues */
        for(int index = floor; index <= ceiling; index++){ 
            temp = (data_t)malloc(sizeof(int));
            *temp = index;
            heap_push(return_heap, index, temp);
        }

    }
    return return_heap;
}

void debug_spoof_node(heap_t *heap_ptr, int index, int priority, data_t data_ptr){
    free(heap_ptr->data[index]->data_ptr);
    heap_ptr->data[index]->priority = priority;
    heap_ptr->data[index]->data_ptr = data_ptr;
}
#endif
#endif
