/* hash table api lib  30/06/24 CR:""*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/ht.h"
#include "../include/sll.h"

#define LOAD_FACTOR_MAX 0.77

struct hash_table {
	SLL_t **hash_arr;
	HashFunc_t hashFunc;
	CompFunc_t CompFunc;
	size_t table_size;
};
/*
int HashIsMaxLoad(hash_t *table)
{
	float load_factor = 0;
	size_t i = 0;

	size_t cur_bucket_items_amount = 0;
	size_t all_entries_amount = 0;

	for (i = 0; i < table->table_size; ++i)
	{	
		if (SLLIsEmpty(table->hash_arr[i]))
		{
			cur_bucket_items_amount = 0;
		}
		else
		{
			cur_bucket_items_amount = SLLCount(table->hash_arr[i]);
		}

		all_entries_amount += cur_bucket_items_amount;
		
	}

	load_factor = ((float)all_entries_amount / (float)(table->table_size));

	printf("load factor is %f\n", load_factor);
	printf("max load factor is %f\n", LOAD_FACTOR_MAX);

	return (load_factor >= LOAD_FACTOR_MAX);
}

float HashBucketSD(hash_t *table)
{
	size_t i = 0;

	size_t table_size = table->table_size;
	size_t cur_bucket_items_amount = 0;

	float mean = 0;
	float variance = 0;

	size_t *tmp_arr = (size_t *)calloc(table_size, sizeof(size_t));

	for (i = 0; i < table_size; ++i)
	{	
		if (SLLIsEmpty(table->hash_arr[i]))
		{
			cur_bucket_items_amount = 0;
		}
		else
		{
			cur_bucket_items_amount = SLLCount(table->hash_arr[i]);
		}

		tmp_arr[i] = cur_bucket_items_amount;
		mean += (float)cur_bucket_items_amount;
	}

	mean /= table_size;

	for (i = 0; i < table_size; ++i)
	{
		variance += pow((tmp_arr[i] - mean), 2);
	}

	variance = sqrt(variance / table_size);

	return variance;
}
*/
hash_t *HTCreate(size_t table_size, HashFunc_t hashFunc, CompFunc_t compFunc)
{
	hash_t *new_hash_table = (hash_t *)malloc(sizeof(hash_t));
	SLL_t **new_hash_table_arr = (SLL_t **)calloc(table_size, sizeof(SLL_t *));

	new_hash_table->hash_arr = new_hash_table_arr;
	new_hash_table->hashFunc = hashFunc;
	new_hash_table->CompFunc = compFunc;
	new_hash_table->table_size = table_size;

	return new_hash_table;
}

void HTDestroy(hash_t *table)
{
	SLL_t *val_list = NULL;
	size_t i = 0;

	assert(table);

	for (i = 0; i < table->table_size; ++i)
	{		
		val_list = (table->hash_arr)[i];

		if(val_list)
		{
			SLLDestroy(val_list);
		}
	}

	free(table->hash_arr);
	free(table);
}

void HTRemove(hash_t *table, void *key)
{
	size_t i = 0;

	assert(table);

	i = (*(size_t *)key);

	SLLDestroy(table->hash_arr[i]);

	table->hash_arr[i] = NULL;
}

void HTInsert(hash_t *table, void *value)
{
	SLL_t *val_list = NULL;
	size_t i = 0;

	assert(table);

	i = table->hashFunc(value);

	val_list = table->hash_arr[i];

	if (val_list)
	{
		SLLInsert(val_list, SLLFirst(val_list), value);
	}
	else
	{	
		table->hash_arr[i] = SLLCreate();
		SLLInsert(table->hash_arr[i], SLLFirst(table->hash_arr[i]), value);
	}

}

size_t HTSize(const hash_t *table)
{	
	SLL_t *val_list = NULL;

	size_t i = 0;
	size_t counter = 0;

	assert(table);

	for (i = 0; i < table->table_size; ++i)
	{
		val_list = table->hash_arr[i];

		if(val_list != NULL)
		{
			++counter;
		}
	}

	return counter;
}

int HTIsEmpty(hash_t *table)
{
	size_t i = 0;

	assert(table);

	for (i = 0; i < table->table_size; ++i)
	{
		if(table->hash_arr[i])
		{
			break;
		}
	}

	return ((i < table->table_size) ? 0 : 1);
}

int HTFind(hash_t *table, void *value)
{
	size_t i = 0;

	assert(table);

	i = table->hashFunc(value);

	if(NULL == table->hash_arr[i])
	{
		return 0;
	}

	if(SLLFind(table->hash_arr[i], value))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void HTForEach(hash_t *table, HT_FuncPtr_t func, void *params)
{
	SLL_t *val_list = NULL;
	size_t i = 0;

	assert(table);

	for (i = 0; i < table->table_size; ++i)
	{
		val_list = table->hash_arr[i];

		if(val_list)
		{
			SLLForEach(SLLFirst(val_list), SLLLast(val_list), func, params);
		}
	}
}