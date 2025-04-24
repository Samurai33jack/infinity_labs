/* stack api header  27/06/24 CR:"omer"*/

#ifndef __STACK_CR7_H__
#define __STACK_CR7_H__
#include <stdio.h>

typedef struct handler_stack stack_t;

stack_t *StackCreate(size_t element_size, size_t num_elements);
/*
O(1)
Receives: size of elements of size_t type, number of elements of size_t type.
Allocate memory using: malloc(element size * num elements +sizeof(stack_t)).
Returns: stack handler of stack_t type.
*/


void StackDestory(stack_t *handler);
/*
O(1)
Receives: stack handler of stack_t type.
Free the stack.
*/
 
void StackPop(stack_t *handler);
/* 
o(1)
Receives: stack handler of type stack_t
Returns: void * to the top element
*/ 
    

void StackPush(stack_t *handler, void *val);
/*
O(1)
Recives: stack handler of type stack_t and void pointer to value
checks if stack is empty
Returns: returns 1 if empty and 0 if not
*/

void *StackPeek(stack_t *handler); 
/* 
o(1)
Receives: stack handler of type stack_t
Returns: void * to the top element
*/
    
size_t StackSize(stack_t *handler);
/*
O(1)
Receives: stack handler of type stack_t
checks how many items are in the stack
Returns: returns amount of items
*/

int StackIsEmpty(stack_t *handler);
/*
O(1)
Receives: stack handler of type stack_t
checks if stack is empty
Returns: returns 1 if empty and 0 if not
*/

size_t StackCapacity(stack_t *handler); 
/*
O(1)
Receives: stack handler of type stack_t
calculate max size of stack
Returns: max size of type size_t
*/
#endif
