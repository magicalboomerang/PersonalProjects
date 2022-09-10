#ifndef LINT_H
#define LINT_H


typedef struct linked_integer{
	int iValue;
	struct linked_integer *link;
} lint;

// Constructor/Destructor
lint *lint_Construct(int);
void lint_Destruct(lint **);

// Rover and iterating functions
void lint_Next(lint **);
void lint_Forward(lint **, int);
void lint_End(lint **);

// Modify individual values
int lint_Get_Indexed_Value(lint *, int);
void lint_Set_Indexed_Value(lint *, int, int);
int lint_Get_Value(lint *);
void lint_Set_Value(lint *, int);

// Modify list
void lint_Append(lint **, lint **);
void lint_Append_int(lint **, int);
void lint_Insert(lint **, lint **, int);
lint *lint_Split(lint **, int);
void lint_Trim(lint **, int);
void lint_Reverse(lint **);

#endif

