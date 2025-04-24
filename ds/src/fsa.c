/* fsa api lib  24/06/24 CR:"edwin"*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/fsa.h"

#define MIN_BLOCK_SIZE sizeof(void *)


struct fsa_handler {
	void *next_block;
};


size_t FsaSuggestSize(size_t block_size, size_t block_number)
{
	if (block_size < MIN_BLOCK_SIZE)
	{
		block_size = MIN_BLOCK_SIZE;

		printf("block size too small, updated to minimum size %ld\n", MIN_BLOCK_SIZE);
	}

	return (sizeof(fsa_t) + (block_size % sizeof(fsa_t)) + (block_size * block_number));
}

fsa_t *FsaInit(void *pool, size_t block_size, size_t pool_size)
{
	fsa_t *fsa_t_pointer = NULL;

	void **start_block_ptr = NULL;

	size_t i = 0;
	size_t loop_end = (pool_size / block_size);

	if (!pool)
	{
		printf("pool allocation failed\n");
		exit(1);
	}

	start_block_ptr = (void **)((char *)pool + sizeof(fsa_t));
	fsa_t_pointer = (fsa_t *)pool;

	fsa_t_pointer->next_block = start_block_ptr;		

	for (i = 1; i < (loop_end - 1); ++i)
	{
		*start_block_ptr = (void *)((char *)start_block_ptr + block_size);
		start_block_ptr = *start_block_ptr;
	}

	*start_block_ptr = NULL;

	return fsa_t_pointer;
}

void *FsaAlloc(fsa_t *handler)
{
	void *block = handler->next_block; 
	
   	handler->next_block = *(void **)handler->next_block;

	return block;
}

void FsaFree(fsa_t *handler, void *block)
{
	*((void **)block) = handler->next_block;
	handler->next_block = block;	
}

size_t FsaCountFree(fsa_t *handler)
{
	size_t counter = 0;
	fsa_t *next_block_ptr = handler->next_block;

	while(next_block_ptr)
	{
		next_block_ptr = *(void **)next_block_ptr;
		++counter;
	} 

	return counter;
}

