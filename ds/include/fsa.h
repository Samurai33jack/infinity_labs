/* fsa api header  24/06/24 CR:"edwin"*/

#ifndef __FSA_CR7_H__
#define __FSA_CR7_H__
#include <stdio.h>
#include <stdlib.h>

typedef struct fsa_handler fsa_t;

size_t FsaSuggestSize(size_t block_size, size_t block_number);

fsa_t *FsaInit(void *pool, size_t block_size, size_t pool_size);

void *FsaAlloc(fsa_t *handler);

void FsaFree(fsa_t *handler, void *block);

size_t FsaCountFree(fsa_t *handler);

#endif
