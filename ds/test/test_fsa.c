/* fsa api tests  24/06/24 CR:"edwin"*/

#include "../include/fsa.h"

int main()
{	
	size_t number_of_blocks = 5;
	size_t block_size = 10;
	size_t pool_size = 0;
	void *new_pool = NULL;
	void *block = NULL;



	fsa_t *cur_handler = NULL;

	pool_size = FsaSuggestSize(block_size, number_of_blocks);
	printf("suggested size is %ld\n", pool_size);

	new_pool = (void*)malloc(pool_size);

	cur_handler = FsaInit(new_pool, block_size, pool_size);

	block = FsaAlloc(cur_handler);






	FsaFree(cur_handler, block);






	return 0;
}
