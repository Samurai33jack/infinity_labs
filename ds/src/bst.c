/* bst api lib 30/06/24 CR:"shahar not fadlon" */

#include <assert.h> /* for assert */ 
#include <stdlib.h> /* for malloc */ 
#include <stdio.h> /* for printf */ 
#include "../include/bst.h"

#define SPACE_ADD 10

typedef struct bst_node bst_node_t;
typedef bst_node_t* bst_iter_t;

struct bst_node {
	void *data;
	bst_iter_t *children;
};

struct bst {
	bst_iter_t root;
	BST_CmpFunc_t cmp_func;
	size_t bst_total_nodes;
};

static bst_iter_t BSTFindNodeDaddy(bst_t *bst, bst_iter_t node, const void *data);
static void BSTRemoveDegreeCheck(bst_t *bst, bst_iter_t node_to_remove, bst_iter_t node_daddy, int child_index);
static void BSTFreeNodeWithTwoChild(bst_t *bst, bst_iter_t node);

static void Print2D(bst_iter_t node, size_t space)
{
    size_t i = 0;

    if (NULL == node)
    {
        return;
    }

    space += SPACE_ADD;
 
    Print2D(node->children[1], space);
 
    printf("\n");

    for (i = SPACE_ADD; i < (space); ++i)
    {
        printf(" ");
    }

    printf("%d\n", *(int *)(node->data));

    Print2D(node->children[0], space);
}

int Print2DTree(bst_t *bst)
{
	assert(bst);

	Print2D(bst->root, 0);

	return 0;
}

bst_t *BSTCreate(BST_CmpFunc_t cmp_func)
{
	bst_t *new_bst = NULL;

	assert(cmp_func);

	new_bst = (bst_t *)malloc(sizeof(bst_t));

	new_bst->root = NULL;
	new_bst->cmp_func = cmp_func;
	new_bst->bst_total_nodes = 0;

	return new_bst;
}

static bst_iter_t BSTNodeCreate(const void *data)
{
	bst_iter_t new_bst_node = NULL;

	assert(data);

	new_bst_node = (bst_iter_t)malloc(sizeof(bst_node_t));

	new_bst_node->children = (bst_iter_t*)malloc(2 * sizeof(bst_iter_t));

	new_bst_node->children[0] = NULL;
	new_bst_node->children[1] = NULL;

	new_bst_node->data = (void *)data;

	return new_bst_node;
}

static bst_iter_t FindNode(bst_iter_t cur_node, const bst_t *bst, const void *data)
{
	assert(bst);
	assert(data);

	if ((bst->root == NULL) || (0 == bst->cmp_func(data, bst->root)))
	{
		return bst->root;
	}

	if(NULL == cur_node)
	{
		return cur_node;
	}

	if (0 == bst->cmp_func(data, cur_node->data))
	{
		return cur_node;
	}

	if (0 > bst->cmp_func(data, cur_node->data))
	{
		return FindNode(cur_node->children[0], bst, data);
	}

	return FindNode(cur_node->children[1], bst, data);
}

int BSTFind(const bst_t *bst, const void *data)
{
	assert(bst);
	assert(data);

	if (NULL == FindNode(bst->root, bst, data))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static int BSTInsertHelper(bst_iter_t cur_node, bst_t *bst, const void *data)
{	
	if (0 > bst->cmp_func(data, cur_node->data))
	{
		if (NULL == cur_node->children[0])
		{
			cur_node->children[0] = BSTNodeCreate(data);
			++bst->bst_total_nodes;
			return 0;
		}

		return BSTInsertHelper(cur_node->children[0], bst, data);
	}
	else
	{
		if (NULL == cur_node->children[1])
		{
			cur_node->children[1] = BSTNodeCreate(data);
			++bst->bst_total_nodes;
			return 0;
		}

		return BSTInsertHelper(cur_node->children[1], bst, data);
	}

	return 1;
}

int BSTInsert(bst_t *bst, const void *data)
{
	int return_status = 0;

	assert(bst);
	assert(data);
	assert(1 != BSTFind(bst, data));

	if (NULL == bst->root)
	{
		bst->root = BSTNodeCreate(data);
		++bst->bst_total_nodes;
		return 0;
	}

	return_status = BSTInsertHelper(bst->root, bst, data);

	return return_status;
}

static int BSTNodeDegree(bst_iter_t node)
{	
	int degree = 0;

	if (node->children[0])
	{
		++degree;
	}

	if (node->children[1])
	{
		++degree;
	}

	return degree;
}

static void BSTSwapNodeData(bst_iter_t node1, bst_iter_t node2)
{	
	void *tmp = NULL;

	tmp = node1->data;
	node1->data = node2->data;
	node2->data = tmp;
}

static void BSTFreeNode(bst_t *bst, bst_iter_t node, bst_iter_t node_daddy, int child_index)
{
	if (NULL == node)
	{
		return;
	}

	if ((NULL == bst->root) || (0 == bst->cmp_func(node->data, bst->root->data)))
	{
		bst->root = NULL;
		free(bst->root->children);
		free(bst->root);
		return;
	}

	if (NULL != node_daddy)
	{
		node_daddy->children[child_index] = NULL;
		free(node->children);
		free(node);
	}
}

static void BSTFreeNodeWithOneChild(bst_t *bst, bst_iter_t node)
{	
	if (NULL == node)
	{
		return;
	}
	if (NULL == node->children[0])
	{
		BSTSwapNodeData(node, node->children[1]);
		BSTRemoveDegreeCheck(bst, node->children[1], node, 1);
	}
	else
	{
		BSTSwapNodeData(node, node->children[0]);
		BSTRemoveDegreeCheck(bst, node->children[0], node, 0);
	}
}

static bst_iter_t BSTFindIOP(bst_t *bst, bst_iter_t cur_node, const void *data)
{	
	bst_iter_t IOP = NULL;

	if (0 <= bst->cmp_func(cur_node->data, data))
	{
		IOP = cur_node;
	}

	if (NULL != cur_node->children[1])
	{
		return BSTFindIOP(bst, cur_node->children[1], data);
	}

	return IOP;
}

static void BSTFreeNodeWithTwoChild(bst_t *bst, bst_iter_t node)
{	
	bst_iter_t IOP = NULL;
	bst_iter_t IOP_daddy = node;

	IOP = BSTFindIOP(bst, node->children[0], node->children[0]->data);

	IOP_daddy = BSTFindNodeDaddy(bst, bst->root, IOP->data);

	BSTSwapNodeData(node, IOP);


	if (BSTNodeDegree(IOP))
	{
		BSTFreeNodeWithOneChild(bst, IOP);
	}
	else
	{
		BSTFreeNode(bst, IOP, IOP_daddy, 0);
	}
}

static void BSTRemoveDegreeCheck(bst_t *bst, bst_iter_t node_to_remove, bst_iter_t node_daddy, int child_index)
{
	int node_degree = BSTNodeDegree(node_to_remove);

	if (0 == node_degree)
	{
		BSTFreeNode(bst, node_to_remove, node_daddy, child_index);
	}
	else if (1 == node_degree)
	{
		BSTFreeNodeWithOneChild(bst, node_to_remove);
	}
	else if (2 == node_degree)
	{
		BSTFreeNodeWithTwoChild(bst, node_to_remove);
	}
}

static bst_iter_t BSTFindNodeDaddy(bst_t *bst, bst_iter_t node, const void *data)
{
	if ((NULL == node) && (0 == bst->cmp_func(data, bst->root->data)))
	{
		return NULL;
	}

	if (((NULL != node->children[1]) && (0 == bst->cmp_func(data, node->children[1]->data))) ||
	 ((NULL != node->children[0]) && (0 == bst->cmp_func(data, node->children[0]->data))))
	{
		return node;
	}

	return BSTFindNodeDaddy(bst, node->children[1], data);
	return BSTFindNodeDaddy(bst, node->children[0], data);
}

int BSTRemove(bst_t *bst, const void *data)
{
	bst_iter_t node_to_remove = NULL;
	bst_iter_t node_to_remove_daddy = NULL;
	int node_to_remove_daddy_index = -1;

	assert(bst);
	assert(data);

	node_to_remove = FindNode(bst->root, bst, data);
	node_to_remove_daddy = BSTFindNodeDaddy(bst, bst->root, data);

	if ((NULL != node_to_remove_daddy) && (NULL != node_to_remove))
	{
		node_to_remove_daddy_index = ((0 < bst->cmp_func(node_to_remove->data, node_to_remove_daddy->data)) ? 1 : 0);
	}
	
	if (NULL == node_to_remove)
	{
		return 1;
	}
	else if (0 == BSTNodeDegree(node_to_remove))
	{
		BSTFreeNode(bst, node_to_remove, node_to_remove_daddy, node_to_remove_daddy_index);
	}
	else if (1 == BSTNodeDegree(node_to_remove))
	{
		BSTFreeNodeWithOneChild(bst, node_to_remove);
	}
	else if (2 == BSTNodeDegree(node_to_remove))
	{
		BSTFreeNodeWithTwoChild(bst, node_to_remove);
	}

	--bst->bst_total_nodes;

	return 0;
}

static void BSTDestroyHelper(bst_iter_t node)
{
	if (NULL == node)
	{
		return;
	}
	
	BSTDestroyHelper(node->children[0]);
	BSTDestroyHelper(node->children[1]);

	free(node->children);
	free(node);
}

int BSTDestroy(bst_t *bst)
{
	assert(bst);

	BSTDestroyHelper(bst->root);

	free(bst);

	return 0;
}

size_t BSTSize(const bst_t *bst)
{
	assert(bst);

	return bst->bst_total_nodes;
}

int BSTIsEmpty(const bst_t *bst)
{
	assert(bst);

	if (NULL == bst->root)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int BSTForEachHelper(bst_iter_t node, BST_ActionFunc_t action_func, void *params)
{
	if (NULL == node)
	{
		return 0;
	}

	BSTForEachHelper(node->children[0], action_func, params);
	BSTForEachHelper(node->children[1],action_func, params);

	return action_func(node->data,params);
}

int BSTForEach(bst_t *bst, BST_ActionFunc_t action_func, void *params)
{	
	assert(bst);
	assert(action_func);

	return BSTForEachHelper(bst->root, action_func, params);
}
