/* ht api header  30/06/24 CR:"ori"*/

#ifndef __SLL_CR7_H__
#define __SLL_CR7_H__

#include <stddef.h> /* size_t */
#include <math.h>

typedef struct node node_t;
typedef struct sll SLL_t;
typedef node_t * SLL_iter_t;
typedef int (*FuncPtr_t)(void *, void *);

SLL_t *SLLCreate();
/*
O(1)
Receives: none.
Create a List.
Returns: Pointer to the handler of type SLL_t.
*/


void SLLDestroy(SLL_t *list);
/*
O(n)
Receives: handler of type SLL_t.
Delete the whole list.
Returns: none.
*/


SLL_iter_t SLLFirst(const SLL_t *list);
/*
O(1)
Receives: handler of type SLL_t.
Returns the first element in the list.
Returns: pointer of type node_t.
*/


SLL_iter_t SLLLast(const SLL_t *list);
/*
O(1)
Receives: handler of type SLL_t.
Returns the last element in the list.
Returns: pointer of type node_t.
*/


SLL_iter_t SLLNext(SLL_iter_t current);
/*
O(1)
Receives: iterator of type pointer to node_t.
Returns the next element.
Returns: pointer of type node_t.
*/


int SLLIsEqual(const SLL_iter_t first, const SLL_iter_t second);
/*
O(1)
Receives: 2 iterators of type pointer to node_t.
Returns if their datas are equal.
Returns: 1 if equals, 0 if not.
*/


void *SLLGetData(const SLL_iter_t node);
/*
O(1)
Receives: iterator of type pointer to node_t.
Returns the data of the node.
Returns: void pointer to the data.
*/


void SLLSetData(SLL_iter_t node, const void* val);
/*
O(1)
Receives: iterator of type pointer to node_t and void pointer to value.
sets the data of the node to be the data recieved by the user.
Returns: none.
*/


SLL_iter_t SLLInsert(SLL_t *list, SLL_iter_t node, const void* val);
/*
O(1)
Receives: handler of type SLL_t, node of type node_t pointer, void pointer value.
Insert a new node after the node given by the user with the value given by the user.
Returns: node_t pointer to the new node that was insterted.
*/


void SLLRemove(SLL_t *list, SLL_iter_t node);
/*
O(1)
Receives: handler of type SLL_t, node of type node_t pointer.
Remove the node given by the user from the list.
Returns: none.
*/


int SLLIsEmpty(const SLL_t *list);
/*
O(1)
Receives: list of nodes of type SLL_t.
check if the list is empty or not
Returns: 1 if empty, 0 if not.
*/


size_t SLLCount(const SLL_t *list);
/*
O(n)
Receives: list of nodes of type SLL_t.
Return the amount of elements in the list.
Returns: size_t value.
*/


SLL_iter_t SLLFind(SLL_t *list, const void *val);
/*
O(n)
Receives: list of nodes of type SLL_t, void pointer of value to search.
Search for a specific value in the list and return a pointer to that node.
Returns: Pointer to node with the given value.
*/


void SLLForEach(SLL_iter_t start, SLL_iter_t stop, FuncPtr_t func, void *params);
/*
O(n)
Receives: 2 nodes of type pointer to node_t, pointer to a function and void pointer to parameters.
Run through each node in the given range and apply the given function for each node.
Returns: none.
*/

#endif
