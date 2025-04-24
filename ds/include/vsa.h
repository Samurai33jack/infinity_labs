/* vsa api header  24/06/24 CR:"uriel"*/

#ifndef __VSA_CR7_H__
#define __VSA_CR7_H__

#include <stdio.h>

typedef struct vsa_handler vsa_t;

/*
Receives: pointer to allocated pool and pool size
initializes the pool for use 
Returns: pool handler of type vsa_t
*/
vsa_t *VsaInit(const void *pool, size_t pool_size);

/*
Receives: pointer to pool handler and desired block size
allocates a new block as required if possible
Returns: allocated block pointer of type void
*/
void *VsaAlloc(vsa_t *handler, size_t block_size);

/*
Receives: pointer to allocated block
frees the block
*/
void VsaFree(void *block);

/*
Receives: pointer to pool handler
checks largest chunk available 
Returns: largest chunk size of type size_t
*/
size_t VsaLca(vsa_t *handler);

#endif

