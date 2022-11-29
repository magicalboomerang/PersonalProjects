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
    
    printf("Make new heap, with 0 value.\n");
    test_heap = heap_construct(0);
    printf("Insert single test item, print tree and destruct.\n");
    heap_push(test_heap, 0, 1);
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    printf("Make new heap with small positive value.\n");
    test_heap = heap_construct(8);
    printf("Insert 3 items such that they are inserted in sequential order.\n");
    heap_push(test_heap, 0, 0);
    heap_push(test_heap, 1, 1);
    heap_push(test_heap, 2, 2);
    heap_print(test_heap);
    heap_destruct(&test_heap);
    
    printf("Make new heap with extended initial size (>16).\n");
    test_heap = heap_construct(32);
    printf("Insert 32 items such that they are inserted in random order.\n");
    heap_push(test_heap, 213, 0);
    heap_push(test_heap, -734, 1);
    heap_push(test_heap, 3368, 2);
    heap_push(test_heap, -488, 3);
    heap_push(test_heap, 98742, 4);
    heap_push(test_heap, 586, 5);
    heap_push(test_heap, -3, 6);
    heap_push(test_heap, -79, 7);
    heap_push(test_heap, -240, 8);
    heap_push(test_heap, 23457, 9);
    heap_push(test_heap, 16556, 10);
    heap_push(test_heap, 543552, 11);
    heap_push(test_heap, 875200, 12);
    heap_push(test_heap, 2345661, 13);
    heap_push(test_heap, 2665, 14);
    heap_push(test_heap, 8087, 15);
    heap_push(test_heap, 232323, 16);
    heap_push(test_heap, 68965, 17);
    heap_push(test_heap, -234, 18);
    heap_push(test_heap, 74232, 19);
    heap_push(test_heap, -468, 20);
    heap_push(test_heap, 4568, 21);
    heap_push(test_heap, 232332358, 22);
    heap_push(test_heap, 8675309, 23);
    heap_push(test_heap, -563, 24);
    heap_push(test_heap, 5654, 25);
    heap_push(test_heap, 2348, 26);
    heap_push(test_heap, 598875, 27);
    heap_push(test_heap, -395, 28);
    heap_push(test_heap, 343456, 29);
    heap_push(test_heap, 109742, 30);
    heap_push(test_heap, 20987, 31);
    heap_print(test_heap);
    printf("Insert 1 more to ensure list expansion.\n");
    heap_push(test_heap, -1337, 32);
    heap_print(test_heap);
    heap_destruct(&test_heap);
}
