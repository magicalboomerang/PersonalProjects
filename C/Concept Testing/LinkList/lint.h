#ifndef LINT_H
#define LINT_H


typedef struct linked_integer{
	int iValue;
	struct linked_integer *link;
} lint;

// Constructor/Destructor
lint *lint_Construct(int);
void lint_Destruct(lint **);

// Modify individual values
int lint_Get_Value(lint *, int);
void lint_Set_Value(lint *, int, int);

// Modify list
void lint_Append(lint **, lint **);
void lint_Append_int(lint **, int);
void lint_Insert(lint **, lint **, int);
void lint_Trim(lint **, int);

#endif

