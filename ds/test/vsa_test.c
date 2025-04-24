/* vsa api tests  25/06/24 CR:"uriel"*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/vsa.h"

int main()
{	
	size_t pool_size = 4242;
	void *new_pool = NULL;
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;

	vsa_t *cur_handler = NULL;

	new_pool = (void*)malloc(pool_size);

	if (!new_pool)
	{
		printf("allocation failed\n");
	}

	cur_handler = VsaInit(new_pool, pool_size);
	
	
	block1 = VsaAlloc(cur_handler, 42);


	block2 = VsaAlloc(cur_handler, 142);


	block3 = VsaAlloc(cur_handler, 333);


	VsaFree(block1);

	block4 = VsaAlloc(cur_handler, 1042);


	VsaFree(block3);


	if(block1 && block2 && block3 && block4)
	{
		printf("well done young padawan!\n");
	}

	block3 = VsaAlloc(cur_handler, 3353);

	if(!block3)
	{
		printf("good test\n");
	}
	free(new_pool);
	return 0;
}

	
