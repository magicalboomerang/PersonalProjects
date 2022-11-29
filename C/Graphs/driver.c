#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

int main(int argc, char *argv[]){
    heap_t *test_heap = NULL;
    
    printf("\nTesting print functions.\nTesting NULL pointer:");
    heap_print(test_heap);
    
    printf("Constructing new heap with invalid value.\n");
    test_heap = heap_construct(-1);
    heap_print(test_heap);
    
    printf("Destroying empty heap.\n");
    heap_destruct(&test_heap);
    heap_print(test_heap);
    
    /* Insert empty object tests here */
    test_heap = heap_construct(0);
    heap_push(test_heap, 0, 1);
    heap_print(test_heap);
    heap_destruct(&test_heap);
}
