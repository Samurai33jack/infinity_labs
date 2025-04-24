/* stack api lib  27/06/24 CR:"omer"*/

#include <assert.h> /*for assert*/
#include <stdlib.h> /* for malloc*/
#include <stdio.h> /* for printf */
#include "../include/stack.h"

struct handler_stack {
	size_t capacity;
	size_t elem_size;
	size_t top;
	void *header;
};

stack_t *StackCreate(size_t element_size, size_t num_elements)
{	static stack_t *stack_ptr = NULL;
	
	stack_ptr = (stack_t *)((char *)malloc(element_size * num_elements + sizeof(stack_t)));
	stack_ptr->capacity = num_elements;
	stack_ptr->elem_size = element_size;
	stack_ptr->top = 0;
	stack_ptr->header = (void *)((char *)stack_ptr + sizeof(stack_t) - element_size);

	return stack_ptr;
}

void StackDestory(stack_t *handler)
{
	free(handler);
}
 
void StackPop(stack_t *handler)
{
	/*
	assert(0 < handler->top);
	*/

	if(0 >= handler->top)
	{
		printf("stack underflow\n");
		return ;
	}

	handler->top -= 1;
	handler->header = (char *)handler->header - handler->elem_size;
}
    

void StackPush(stack_t *handler, void *val)
{
	/*
	assert(handler->top < handler->capacity);
	*/

	if(handler->top >= handler->capacity)
	{
		printf("stack overflow\n");
		return ;
	}

	handler->top += 1;
	handler->header = (char *)handler->header + handler->elem_size;

	if(handler->elem_size <= sizeof(char))
	{
		*(char *)handler->header = *(char *)val;
	}
	else if(handler->elem_size <= sizeof(short))
	{
		*(short *)handler->header = *(short *)val;
	}
	else
	{
		*(size_t *)handler->header = *(size_t *)val;
	}
}

void *StackPeek(stack_t *handler)
{
	return handler->header;
}
    
size_t StackSize(stack_t *handler)
{
	return handler->top;
}

int StackIsEmpty(stack_t *handler)
{
	return (handler->top ? 1 : 0);
}

size_t StackCapacity(stack_t *handler)
{
	return handler->capacity;
}
