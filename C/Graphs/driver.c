#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

int main(int argc, char *argv[]){
    heap_t *test_heap = NULL;
    int *check1, *check2, index;
    
    printf("Test Group 1: Try to use empty/uninitialized heaps.\nTesting NULL pointer:");
    debug_check_heap(test_heap);
    heap_print(test_heap);
    
    printf("Constructing new heap with invalid value.\n");
    test_heap = heap_construct(-1,LOW_PRIORITY);
    debug_check_heap(test_heap);
    heap_print(test_heap);
    
    printf("Destroying empty heap.\n");
    heap_destruct(&test_heap);
    heap_print(test_heap);
    
    printf("\n\nTest Group 2: Heap insertions.\nMake new heap, with 0 size.\n");
    test_heap = heap_construct(0,LOW_PRIORITY);
    printf("Insert single test item, print tree and destruct.\n");
    heap_push(test_heap, 0, NULL);
    debug_check_heap(test_heap);
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    printf("Make new heap with small positive value.\n");
    test_heap = heap_construct(8,LOW_PRIORITY);
    printf("Insert 3 items such that they are inserted in sequential order.\n");
    heap_push(test_heap, 0, NULL);
    heap_push(test_heap, 1, NULL);
    heap_push(test_heap, 2, NULL);
    debug_check_heap(test_heap);
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    printf("Make new heap with extended initial size (>16).\n");
    test_heap = heap_construct(32,LOW_PRIORITY);
    heap_print(test_heap);
    printf("Insert 32 items such that they are inserted in random order.\n");
    heap_push(test_heap, 213, NULL);
    heap_push(test_heap, -734, NULL);
    heap_push(test_heap, 3368, NULL);
    heap_push(test_heap, -488, NULL);
    heap_push(test_heap, 98742, NULL);
    heap_push(test_heap, 586, NULL);
    heap_push(test_heap, -3, NULL);
    heap_push(test_heap, -79, NULL);
    heap_push(test_heap, -240, NULL);
    heap_push(test_heap, 23457, NULL);
    heap_push(test_heap, 16556, NULL);
    heap_push(test_heap, 543552, NULL);
    heap_push(test_heap, 875200, NULL);
    heap_push(test_heap, 2345661, NULL);
    heap_push(test_heap, 2665, NULL);
    heap_push(test_heap, 8087, NULL);
    heap_push(test_heap, 232323, NULL);
    heap_push(test_heap, 68965, NULL);
    heap_push(test_heap, -234, NULL);
    heap_push(test_heap, 74232, NULL);
    heap_push(test_heap, -468, NULL);
    heap_push(test_heap, 4568, NULL);
    heap_push(test_heap, 232332358, NULL);
    heap_push(test_heap, 8675309, NULL);
    heap_push(test_heap, -563, NULL);
    heap_push(test_heap, 5654, NULL);
    heap_push(test_heap, 2348, NULL);
    heap_push(test_heap, 598875, NULL);
    heap_push(test_heap, -395, NULL);
    heap_push(test_heap, 343456, NULL);
    heap_push(test_heap, 109742, NULL);
    heap_push(test_heap, 20987, NULL);
    heap_print(test_heap);
    printf("Ensure they are a heap.");
    debug_check_heap(test_heap);
    printf("Insert 1 more to ensure list expansion.\n");
    heap_push(test_heap, -1337, NULL);
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    
    printf("Insert 5 items with equal priorites.\n");
    test_heap = heap_construct(-1,LOW_PRIORITY);
    for(index = 0; index < 5; index++)
        heap_push(test_heap, -1, NULL);
    debug_check_heap(test_heap);
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    printf("Test function for building a quick heap.\n");
    test_heap = debug_quick_heap(1, 15,LOW_PRIORITY);
    debug_check_heap(test_heap);
    heap_print(test_heap);
    
    printf("Test function to check for bad trees.\n");
    debug_spoof_node(test_heap, 1, -16, NULL);
    heap_print(test_heap);
    printf("(Check removed. Manually reinsert to verify.)\n\n");
    /* Removed since the assertion intentionally crashes. Reinsert to test.
    debug_check_heap(test_heap);
    */
    heap_destruct(&test_heap);
    
    printf("Test Group 3: Heap removals\nFail to removing objects from an empty list...");
    assert(!heap_pop(test_heap));
    test_heap = heap_construct(-1,LOW_PRIORITY);
    assert(!heap_pop(test_heap));
    printf("test passed.\n\n");
    printf("Insert and remove single object.\n");
    heap_push(test_heap, 0, NULL);
    heap_print(test_heap);
    printf("Removing only element.\n");
    check1 = heap_pop(test_heap);
    printf("Removal was %ssuccessful.\n", check1 ? "un" : "");
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    printf("Insert+Remove 127 items with matching key/data pairs, and verify the final heap order after each removal.\n");
    test_heap = debug_quick_heap(0,126,LOW_PRIORITY);/*key = data for easy varification*/
    heap_print(test_heap);
    check2 = malloc(sizeof(int));
    *check2 = -1;
    index = 0;
    while(!heap_is_empty(test_heap)){
        check1 = heap_pop(test_heap);
        debug_check_heap(test_heap);
        assert(*check2 < *check1);/* Remember it's a LOW priority heap */
        free(check2);
        check2 = check1;
        printf("Item %d popped: %d\n",index++, *check2);
        if(!(index % 10))/*Only print a tree every 10 removals*/
            heap_print(test_heap);
    }
    free(check2);
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    printf("Test Group 4: High-Priority queue tests.\n\n");
    printf("Making high priority test tree from -100 to 100 (201 items).\n");
    test_heap = debug_quick_heap(-100, 100, HIGH_PRIORITY);
    debug_check_heap(test_heap);
    index = test_heap->attribs->size / 2;
    while(!heap_is_empty(test_heap)){
        printf("Popping items %d to %d.\n", index, test_heap->attribs->size);
        while(0 < test_heap->attribs->size && index <= test_heap->attribs->size)
            free(heap_pop(test_heap));
        debug_check_heap(test_heap);
        heap_print(test_heap);
        index = test_heap->attribs->size / 2;
    }
    heap_destruct(&test_heap);
}

