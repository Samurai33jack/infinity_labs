/* vsa api lib  25/06/24 CR:"uriel"*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/vsa.h"

#define MAGIC 42424242424242 

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

struct vsa_handler {
	#ifndef NDEBUG
	size_t magic_number;
	#endif
	long block_size;
};

vsa_t *VsaInit(const void *pool, size_t pool_size)
{
	vsa_t *start_handler = NULL;
	vsa_t *end_handler = NULL;

	if(NULL == pool)
	{	
		printf("no valid pool provided\n");
		return NULL;
	}

	start_handler = (vsa_t *)pool;
	start_handler->block_size = -(pool_size - (2 * sizeof(vsa_t)));

	end_handler = (vsa_t *)((char *)pool + pool_size - sizeof(vsa_t));
	end_handler->block_size = MAGIC;

	#ifndef NDEBUG
	start_handler->magic_number = MAGIC;
	end_handler->magic_number = MAGIC;
	#endif

	return start_handler;
}

void *VsaAlloc(vsa_t *handler, size_t block_size)
{
	vsa_t *cur_pointer = NULL;
	vsa_t *next_pointer = NULL;
	vsa_t *target_pointer = NULL;

	void *new_block = NULL;
	long new_block_size = 0;

	if(NULL == handler)
	{
		printf("no valid handler provided\n");
		return NULL;
	}

	new_block_size = VsaLca(handler);

	if ((long)block_size > new_block_size)
	{
			printf("not enough space dude\n");
			return NULL;
	}

	cur_pointer = handler;
	next_pointer = (vsa_t *)((char *)cur_pointer + ABS(cur_pointer->block_size) + sizeof(vsa_t));
	
	while (cur_pointer->block_size != MAGIC)
	{

		if ((cur_pointer->block_size < 0) && (ABS(cur_pointer->block_size) >= (long)block_size) && 
		(ABS(cur_pointer->block_size) <= new_block_size))
		{	
			target_pointer = cur_pointer;
			new_block_size = ABS(cur_pointer->block_size);
		}

		cur_pointer = next_pointer;
		next_pointer = (vsa_t *)((char *)cur_pointer + ABS(cur_pointer->block_size) + sizeof(vsa_t));
	}
	
	new_block_size = target_pointer->block_size;

	#ifndef NDEBUG
	target_pointer->magic_number = MAGIC;
	#endif

	target_pointer->block_size = block_size;

	new_block = target_pointer + 1;
	next_pointer = (vsa_t *)((char *)target_pointer + ABS(target_pointer->block_size) + sizeof(vsa_t));

	#ifndef NDEBUG
		next_pointer->magic_number = MAGIC;
	#endif
	next_pointer->block_size = (new_block_size + ABS(target_pointer->block_size) + sizeof(vsa_t));

	return new_block;
}

void VsaFree(void *block)
{
	vsa_t *cur_pointer = NULL;

	if(NULL == block)
	{
		printf("no valid block provided\n");
		return ;
	}

	cur_pointer = (vsa_t *)((char *)block - sizeof(vsa_t));

	#ifndef NDEBUG
	if (cur_pointer->magic_number != MAGIC)
	{
		printf("sir we have a problem there is no magic here\n");
	}
	#endif

	cur_pointer->block_size *= -1;
}

size_t VsaLca(vsa_t *handler)
{
	vsa_t *cur_pointer = NULL;
	vsa_t *next_pointer = NULL;

	size_t lca = 0;
	
	cur_pointer = handler;

	while (cur_pointer->block_size != MAGIC)
	{
		next_pointer = (vsa_t *)((char *)cur_pointer + ABS(cur_pointer->block_size) + sizeof(vsa_t));

		while ((next_pointer->block_size < 0) && (cur_pointer->block_size < 0))
		{
			cur_pointer->block_size -= ABS(next_pointer->block_size) - sizeof(vsa_t);
			next_pointer += ABS(next_pointer->block_size);
		}

		lca = (ABS(cur_pointer->block_size) > (long)lca) ? (size_t)ABS(cur_pointer->block_size) : lca;

		cur_pointer = next_pointer;
	}

	return lca;
}
