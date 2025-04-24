/* hash table api header  30/06/24 CR:""*/
#ifndef __HT_CR7_H__
#define __HT_CR7_H__

#include <stddef.h> /* size_t */
#include <math.h>


typedef struct hash_table hash_t;

typedef int (*HT_FuncPtr_t)(void *, void *);
typedef int (*CompFunc_t )(void *, void *);

typedef size_t (*HashFunc_t)(void *);

int HashIsMaxLoad(hash_t *table);

float HashBucketSD(hash_t *table);

/* O(1) */
hash_t *HTCreate(size_t table_size,HashFunc_t hashFunc, CompFunc_t compFunc);

/* O(n^2) */
void HTDestroy(hash_t *table);

/* O(1) || O(n) */
void HTRemove(hash_t *table, void *key); 

/* O(1) */
void HTInsert(hash_t *table, void *value);

/* O(n) */
size_t HTSize(const hash_t *table);

/* O(n) */
int HTIsEmpty(hash_t *table);

/* O(1) */
int HTFind(hash_t *table, void *value);

/* O(n) */
void HTForEach(hash_t *table, HT_FuncPtr_t func, void *params);

#endif

