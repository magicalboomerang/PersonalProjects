#include <stdio.h>
#include <assert.h>
#include "lint.h"

void PrintList(lint *list);

int main(int argc, char **argv){
    lint *lTester = NULL, *lHolder = NULL;
    int index;
    
    // Test print on empty list
    PrintList(lTester);
    
    // Test constructor
    lTester = lint_Construct(0);
    lHolder = lint_Construct(1);
    
    // Test item append
    lint_Append(&lTester, &lHolder);
    assert(lHolder == NULL); // lTester handles both items now
    
    // Literal append on a list
    lint_Append_int(&lTester, 2);
    PrintList(lTester);
    
    lint_Set_Indexed_Value(lTester, 1, 4);
    PrintList(lTester);
    
    // Test reverse
    lint_Reverse(&lTester);
    PrintList(lTester);
    
    // Test trim on last item
    lint_Trim(&lTester, 2);
    PrintList(lTester);
    
    // Test Destructor
    lint_Destruct(&lTester);
    assert(lTester == NULL);
    
    // Literal append on an empty list
    lint_Append_int(&lHolder, 42);
    PrintList(lHolder);
    
    // Append an item into an empty list
    lint_Append(&lTester, &lHolder);
    PrintList(lTester);
    PrintList(lHolder);
    
    // Test trim 0 full deletes
    lint_Trim(&lTester, 0);
    assert(lTester == NULL);
    
    // Create longer lists for testing
    for(index = 0; index <= 10; index++)
        lint_Append_int(&lTester, index * index);
    
    for(index = 0; index <= 10; index++)
        lint_Append_int(&lHolder, 100 - (index * index));

    PrintList(lTester);
    PrintList(lHolder);
    
    // Insert OOB
    lint_Insert(&lTester, &lHolder, 500);
    assert(lHolder != NULL);
    
    // Insert in bounds
    lint_Insert(&lTester, &lHolder, 5);
    PrintList(lTester);
    assert(lHolder == NULL);
    
    // Split OOB
    lHolder = lint_Split(&lTester, 500);
    assert(lHolder == NULL);
    
    // Split in bounds
    lHolder = lint_Split(&lTester, 11);

    PrintList(lTester);
    PrintList(lHolder);
    
    // ----------------------------------------------------------------------
    // Clean up program. Delete/Add with care!
    lint_Destruct(&lTester);
    assert(lTester == NULL);
    lint_Destruct(&lHolder);
    assert(lHolder == NULL);
    
    return 0;
}

void PrintList(lint *list){
    lint *lRover = list;
    int iMemberCount = 0;
    printf("Printing all members of list...\n");
    while(lRover != NULL){
    	printf("\tNode %d: %d\n", iMemberCount++, lint_Get_Value(lRover));
    	lint_Next(&lRover);
    }
    printf("EOL Reached, printed %d items.\n\n", iMemberCount);
}
