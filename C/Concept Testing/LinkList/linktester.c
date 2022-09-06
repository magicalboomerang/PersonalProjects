#include <stdio.h>
#include "lint.h"

int main(int argc, char **argv){
    lint *lTester, *lHolder;
    
    lTester = lint_Construct(0);
    lHolder = lint_Construct(1);
    
    lint_Add(lTester, lHolder);
    
    lint_Add_int(lTester, 2);
    
    lint_Set_Value(lTester, 1, 4);
    
    lint_Destruct(lTester);    
    
    return 0;
}
