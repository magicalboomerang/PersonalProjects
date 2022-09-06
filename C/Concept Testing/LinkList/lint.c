#include <stdlib.h>
#include "lint.h"

// "Private" functions
int _getValue(lint *litem);
void _setValue(lint *litem, int value);
lint *_getItemReference(lint *litem, int index);
lint *_getTail(lint *litem);

// Constructor/Destructor
lint *lint_Construct(int NewValue){
    lint *litem = (lint *)malloc(sizeof(lint));
    litem->iValue = NewValue;
    return litem;
}

/* Purpose, destructs this element and every element in this chain
 * 
 * Arguments: litem - the head of the chain to be destructed.
 *
 * Returns: Nothing, destroys the argument.
 *
 * Bugs: None known
*/
void lint_Destruct(lint *litem){
    lint *ltrash;
    while(litem){
        ltrash = litem;
        litem = litem->link;
        free(ltrash);
    }
}

int lint_Get_Value(lint *litem, int index){
    lint *llink = _getItemReference(litem, index);
    return _getValue(llink);
}

void lint_Set_Value(lint *litem, int index, int value){
    lint *llink = _getItemReference(litem, index);
    _setValue(llink, value);
}

void lint_Add(lint *lhead, lint *ltail){
    lint *llink = _getTail(lhead);
    if(llink != NULL){
        llink->link = ltail;
    }
}

// Takes a LL and an integer, appends a new int node
void lint_Add_int(lint *litem, int NewValue){
    lint *lnew = lint_Construct(NewValue);
    lint_Add(litem, lnew);
}

// PRIVATE FUNCTIONS! NO PEEKING!

int _getValue(lint *litem){
    if(litem != NULL)
        return litem->iValue;
    else
        return -1;
}

void _setValue(lint *litem, int value){
    if(litem != NULL)
        litem->iValue = value;
}

// Returns: either a reference to an item that exists at the exact index, or NULL.
lint *_getItemReference(lint *litem, int index){
    int litem_index;
    lint *found_litem = (index < 0) ? NULL : litem;
    
    // This algorithm handles litem == NULL
    for(litem_index = 1; litem_index <= index && found_litem != NULL; litem_index++)
        found_litem = found_litem->link;
    return found_litem;
}

lint *_getTail(lint *litem){
    lint *tailItem = NULL;
    lint *headItem = litem;
    
    // When the head is NULL we have the tail!
    while(headItem){
        tailItem = headItem;
        headItem = headItem->link;
    };
    
    return tailItem;
}
