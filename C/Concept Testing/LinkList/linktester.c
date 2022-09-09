#include <stdio.h>
#include <assert.h>
#include "lint.h"

int main(int argc, char **argv){
    lint *lTester, *lHolder;
    
    lTester = lint_Construct(0);
    lHolder = lint_Construct(1);
    
    lint_Append(&lTester, &lHolder);
    assert(lHolder == NULL);
    
    lint_Append_int(lTester, 2);
    
    lint_Set_Value(lTester, 1, 4);
        
    for(int iDisplayIndex = 0; iDisplayIndex < 4; iDisplayIndex++)
    	printf("Node %d: %d\n", iDisplayIndex, lint_Get_Value(lTester, iDisplayIndex));
    
    lint_Destruct(&lTester);
    assert(lTester == NULL);
    
    return 0;
}
