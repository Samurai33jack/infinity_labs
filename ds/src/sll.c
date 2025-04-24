/* sll api tests  30/06/24 CR:"ori"*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/sll.h"

struct node {
	void *data;
	node_t *next;
};

struct sll {
	node_t *head;
	node_t *tail;
};

SLL_t *SLLCreate()
{
	SLL_t *new_list = NULL;
	node_t *dummy_node = NULL;

	new_list = (SLL_t *)malloc(sizeof(SLL_t));

	dummy_node = (node_t *)malloc(sizeof(node_t));

	dummy_node->data = NULL;
	dummy_node->next = dummy_node;

	new_list->head = dummy_node;
	new_list->tail = dummy_node;

	return new_list;
}

void SLLDestroy(SLL_t *list)
{
	SLL_iter_t cur_node = list->head;
	SLL_iter_t tmp_node = NULL;

	assert(list);

	while(cur_node != list->tail)
	{
		tmp_node = SLLNext(cur_node);
		free(cur_node);
		cur_node = tmp_node;
	}

	free (list->tail);
	free (list);
}

SLL_iter_t SLLFirst(const SLL_t *list)
{
	assert(list);
	return list->head->next;
}

SLL_iter_t SLLLast(const SLL_t *list)
{
	assert(list);
	return list->tail;
}

SLL_iter_t SLLNext(SLL_iter_t current)
{	
	assert(current);
	return current->next;
}

int SLLIsEqual(const SLL_iter_t first, const SLL_iter_t second)
{
	assert(first);
	assert(second);
	return (first->data == second->data);
}

void *SLLGetData(const SLL_iter_t node)
{
	assert(node);
	return node->data;
}

void SLLSetData(SLL_iter_t node, const void* val)
{
	assert(node);
	node->data = (void *)val;
}

SLL_iter_t SLLInsert(SLL_t *list, SLL_iter_t node, const void* val)
{
	node_t *new_node = NULL;

	assert(list);
	assert(node);

	new_node = (node_t *)malloc(sizeof(node_t));

	new_node->data = (void *)val;

	if(list->tail != node)
	{	
		new_node->next = node->next;
		node->next = new_node;
	}
	else
	{
		new_node->next = NULL;
		node->next = new_node;
		list->tail = new_node;
	}

	return new_node;
}

void SLLRemove(SLL_t *list, SLL_iter_t node)
{
	SLL_iter_t cur_node = SLLFirst(list);

	assert(list);
	assert(node);

	if (cur_node == node)
	{
		list->head->next = node->next;
		free(node);
		return;
	}

	while (cur_node->next != node)
	{
		cur_node = SLLNext(cur_node);
	}

	cur_node->next = node->next;

	free(node);
}

int SLLIsEmpty(const SLL_t *list)
{
	if(NULL == list)
	{
		return 1;
	}

	return (list->head == list->tail);
}

static int SLLCountNode(void* node, void *counter)
{	
	assert(node);

	*(size_t *)counter += 1;
	return 0;
}

size_t SLLCount(const SLL_t *list)
{
	static size_t counter = 0;
	static size_t *counter_ptr = &counter;

	counter = 0;
	
	if (0 != SLLIsEmpty(list))
	{
		return 0;
	}

	SLLForEach(list->head, list->tail, SLLCountNode, counter_ptr);

	return counter;
}

static int SLLFindNode(void* cur_node, void *found_node)
{
	if (((SLL_iter_t)cur_node)->data == ((SLL_iter_t)found_node)->data)
	{
		found_node = (SLL_iter_t)cur_node;
		return 1;
	}
	else
	{
		return 0;
	}
}

SLL_iter_t SLLFind(SLL_t *list, const void *val)
{	
	static node_t tmp_node;
	static SLL_iter_t node_to_find = &tmp_node;

	assert(list);

    tmp_node.data = (void *)val;
    tmp_node.next = NULL;

	SLLForEach(list->head, list->tail, SLLFindNode, node_to_find);

    return node_to_find;
}

void SLLForEach(SLL_iter_t start,SLL_iter_t stop, FuncPtr_t func, void *params)
{	
	SLL_iter_t cur_node = NULL;

	assert(start);
	assert(stop);

	cur_node = start;

	while (cur_node != stop) 
	{	
		if(func(cur_node, params))
		{
			return;
		}
		else
		{
			cur_node = cur_node->next;
		}
	}
}
