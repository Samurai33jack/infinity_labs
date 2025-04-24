/* bst api header 30/06/24 CR:"" */

#ifndef __BST_CR7_H__
#define __BST_CR7_H__

#include <stddef.h> /* size_t */

typedef struct bst bst_t;

typedef int (*BST_CmpFunc_t)(const void *, const void *);
typedef int (*BST_ActionFunc_t)(void *, void *);

int Print2DTree(bst_t *bst);

bst_t *BSTCreate(BST_CmpFunc_t cmp_func);
/*
O(1)
Receives: Comp func of BST_CompFunc_t type.
Create a bst handler.
Returns: Pointer to the handler of type bst_t.
*/


int BSTDestroy(bst_t *bst);
/*
O(n)
Receives: handler of type bst_t.
Delete the whole tree.
Returns: 0 for success and other value for fail.
*/


int BSTInsert(bst_t *bst, const void *data); 
/*
O(h)
Receives: handler of type bst_t and void pointer to data.
checks if data is valid for insertion 
and inserts the data element in the correct place in the tree.
Returns: 0 for success and other value for fail.
*/


int BSTRemove(bst_t *bst, const void *data); 
/*
O(h)
Receives: handler of type bst_t and void pointer to data.
removes the data element while maintaining the bst validity 
Returns: 0 for success and other value for fail.
*/


int BSTFind(const bst_t *bst, const void *data); 
/*
O(h)
Receives: handler of type bst_t and void pointer to data to find.
checks if data exists in the tree
Returns: 1 if found and other value for fail.
*/


size_t BSTSize(const bst_t *bst); 
/*
O(n)
Receives: handler of type bst_t.
Returns the total amount of nodes in the bst
Returns: the size of type size_t.
*/


int BSTIsEmpty(const bst_t *bst); 
/*
O(1)
Receives: handler of type bst_t.
check if the bst is empty or not
Returns: 1 if empty and other value if not.
*/


int BSTForEach(bst_t *bst, BST_ActionFunc_t action_func, void *params);
/*
O(n)
Receives: handler of type bst_t, pointer to an action function and void pointer to parameters.
Run through each node in the given bst and apply the given function for each node.
Returns: 0 for success and other value for fail.
*/

#endif