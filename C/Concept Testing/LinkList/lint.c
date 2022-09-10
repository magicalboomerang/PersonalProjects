#ifndef LINT_C
#define LINT_C

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

/* Purpose: Takes a list token and moves it forward exactly ONE element.
 * 
 * Arguments: lRover (lint **): A handle variable, used in managing indexing.
 *
 * Returns: Nothing (void), but changes the passed lRover variable.
 *
 * Bugs: None known
*/
void lint_Next(lint **lRover){
    if(lRover != NULL && *lRover != NULL)
        *lRover = (*lRover)->link;
}

/* Purpose: Takes a list token and moves it forward a desired amount
 * 
 * Arguments: lRover (lint **): A handle variable, used in managing indexing.
 *      index (int) the relative number of positions to move the handle.
 *
 * Returns: Nothing (void), but changes the passed lRover variable.
 *
 * Bugs: None known
*/
void lint_Forward(lint **lRover, int index){
    *lRover = _getItemReference(*lRover, index);
}

/* Purpose: Takes a list token and moves it to the last element.
 * 
 * Arguments: lRover (lint **): A handle variable, used in managing indexing.
 *
 * Returns: Nothing (void), but changes the passed lRover variable.
 *
 * Bugs: None known
*/
void lint_End(lint **lRover){
    *lRover = _getTail(*lRover);
}

/* Purpose: Returns the value of an element, relative to another.
 *      Currently this only works in the positive direction.
 * 
 * Arguments: litem (lint *): A list token to be used as 0.
 *      index (int>0): The relative position of the token to be read.
 *
 * Returns: An integer value of the requested element, or -0 if
 *      the element cannot be found.
 *
 * Bugs: None known
*/
int lint_Get_Indexed_Value(lint *litem, int index){
    lint *llink = _getItemReference(litem, index);
    return _getValue(llink);
}

/* Purpose: Sets the value of a list element, relative to another.
 * 
 * Arguments: litem (lint *): The list element to be used as 0.
 *      index (int): The relative position of the element to be set.
 *      value (int): The value to be stored in the target entry.
 *
 * Returns: None (void).
 *
 * Bugs: None known
*/
void lint_Set_Indexed_Value(lint *litem, int index, int value){
    lint *llink = _getItemReference(litem, index);
    _setValue(llink, value);
}


/* Purpose: Accesses the literal value of the entry.
 * 
 * Arguments: litem (lint *): The item whose contents will be read.
 *
 * Returns: An integer of the value stored in the entry.
 *
 * Bugs: None known
*/
int lint_Get_Value(lint *litem){
    return _getValue(litem);
}

/* Purpose: Sets the literal value of the entry.
 * 
 * Arguments: litem (lint *): The item whose contents will be set.
 *      iNew (int): The value to be stored in this entry.
 *
 * Returns: None (void).
 *
 * Bugs: None known
*/
void lint_Set_Value(lint *litem, int iNew){
    _setValue(litem, iNew);
}

// If successful ltail == NULL
/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: None (void).
 *
 * Bugs: None known
*/
void lint_Append(lint **lhead, lint **ltail){
    if(lhead != NULL  && ltail != NULL){ // Validate point to pointer
        lint *llink = _getTail(*lhead);// validates head
        if(llink != NULL){
            llink->link = *ltail;
        } else {
            *lhead = *ltail;
        }
        *ltail = NULL;
    }
}

// linserted will be set to NULL if op successful
/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
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

/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
lint *lint_Split(lint **litem, int iSplitIndex){
    lint *lDrop, *lHead = NULL;
    if(litem != NULL){
        if(iSplitIndex > 0){
            lDrop = _getItemReference(*litem, iSplitIndex - 1);
            if(lDrop != NULL){
                lHead = lDrop->link;
                lDrop->link = NULL;
            }
        } else if(iSplitIndex == 0) {
            lHead = *litem;
            *litem = NULL;
        }
    }
    
    return lHead;
}

// Takes a LL and an integer, appends a new int node
/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
void lint_Append_int(lint **litem, int NewValue){
    lint *lnew = lint_Construct(NewValue);
    lint_Append(litem, &lnew);
}

/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
void lint_Trim(lint **litem, int iTrimIndex){
    lint *lDrop;
    if(litem != NULL){
        if(iTrimIndex > 0){
            lDrop = _getItemReference(*litem, iTrimIndex - 1);
            if(lDrop != NULL)
                lint_Destruct(&(lDrop->link));
        } else if(iTrimIndex == 0) {
            lint_Destruct(litem);
        }
    }
}

/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
void lint_Reverse(lint **litem){
    lint *lCurrent, *lPrevious, *lNext;
    if(litem != NULL && *litem != NULL){
        lCurrent = *litem;
        lPrevious = NULL;
        lNext = lCurrent->link;
    }
    
    while(lNext != NULL){
        lCurrent->link = lPrevious;
        lPrevious = lCurrent;
        lCurrent = lNext;
        lNext = lNext->link;
    }
    
    if(lPrevious != NULL){
        *litem = lCurrent;
        lCurrent->link = lPrevious;
    }
}

// PRIVATE FUNCTIONS! NO PEEKING!
/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
int _getValue(lint *litem){
    if(litem != NULL)
        return litem->iValue;
    else
        return -0;
}

/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
void _setValue(lint *litem, int value){
    if(litem != NULL)
        litem->iValue = value;
}

/* Returns either:
 *  A reference to an item that exists at the exact linear index.
 *      - or -
 *  A NULL, because the index is out of bounds or the list DNE.
 */ 
/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
lint *_getItemReference(lint *litem, int index){
    int litem_index;
    lint *found_litem = (index < 0) ? NULL : litem;
    
    // This algorithm handles litem == NULL
    for(litem_index = 0; litem_index < index && found_litem != NULL; litem_index++)
        found_litem = found_litem->link;
    return found_litem;
}


/* Purpose: 
 * 
 * Arguments: 
 *
 * Returns: 
 *
 * Bugs: None known
*/
lint *_getTail(lint *litem){
    lint *tailItem = NULL;
    lint *headItem = litem;
    
    // When the head is NULL we have the tail!
    if(litem != NULL){
        do{
            tailItem = headItem;
            headItem = headItem->link;
        }while(headItem != NULL && headItem != litem);
    // If the lhead == litem here, we're in a ring and need to exit
    }
    return tailItem;
}

#endif
