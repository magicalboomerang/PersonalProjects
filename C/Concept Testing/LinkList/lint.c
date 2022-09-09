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
    litem->link = NULL;
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
void lint_Destruct(lint **litem){
    lint *ltrash;
    while(*litem){
        ltrash = *litem;
        *litem = (*litem)->link;
        free(ltrash);
    }
    *litem = NULL;
}

int lint_Get_Value(lint *litem, int index){
    lint *llink = _getItemReference(litem, index);
    return _getValue(llink);
}

void lint_Set_Value(lint *litem, int index, int value){
    lint *llink = _getItemReference(litem, index);
    _setValue(llink, value);
}

// If successful ltail == NULL
void lint_Append(lint **lhead, lint **ltail){
    if(lhead != NULL  && ltail != NULL){
        lint *llink = _getTail(*lhead);
        if(llink != NULL){
            llink->link = *ltail;
        } else {
            *lhead = *ltail;
        }
        *ltail = NULL;
    }
}

// linserted will be set to NULL if op successful
void lint_Insert(lint **lsearched, lint **linserted, int index){
    if(linserted != NULL && lsearched != NULL){
        if(index == 0){
            lint_Append(linserted, lsearched);
            *lsearched = *linserted;
            *linserted = NULL;
        } else if(index > 0){
            lint *llink = _getItemReference(*lsearched, index - 1);
            if(llink != NULL){
                lint_Append(linserted, &llink->link);
                llink->link = *linserted;
                *linserted = NULL;
            }
        }
    }
}

// Takes a LL and an integer, appends a new int node
void lint_Append_int(lint *litem, int NewValue){
    lint *lnew = lint_Construct(NewValue);
    lint_Append(&litem, &lnew);
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

/* Returns either:
 *  A reference to an item that exists at the exact linear index.
 *      - or -
 *  A NULL, because the index is out of bounds or the list DNE.
 */ 
lint *_getItemReference(lint *litem, int index){
    int litem_index;
    lint *found_litem = (index < 0) ? NULL : litem;
    
    // This algorithm handles litem == NULL
    for(litem_index = 0; litem_index < index && found_litem != NULL; litem_index++)
        found_litem = found_litem->link;
    return found_litem;
}


lint *_getTail(lint *litem){
    lint *tailItem = NULL;
    lint *headItem = litem;
    
    // When the head is NULL we have the tail!
    // Also if the tail refers to original head (litem)
    // There must be some kind of loop!
    while(headItem != NULL){
        tailItem = headItem;
        headItem = headItem->link;
    };
    
    return tailItem;
}
