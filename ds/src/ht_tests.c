/* hash table api tests  30/06/24 CR:""*/

#include "../include/ht.h"

#include <stdio.h>
#include <math.h>



size_t HashFuncTest(void *val)
{
    size_t value = *(size_t *)val;

    value = value % 42;

    return value;
}

int CompFuncTest(void *val1, void *val2)
{
     size_t value1 = *(size_t *)val1;
     size_t value2 = *(size_t *)val2;

     return (value1 == value2);
}

int HTFuncTest(void *data, void *val)
{
    if(NULL == data)
    {
        return 0;
    }

    if (data == val)
    {
        printf("%ld\n", *(size_t *)val);
    }

    return 0;
}

int main()
{   
    hash_t *new_hash_table = NULL;
    size_t hash_table_size = 42;

    int num1 = 3;
    int num2 = 8;
    int num3 = 777;
    int num4 = 42;

    size_t key4 = 0;
    
    int *num1_ptr = &num1;
    int *num2_ptr = &num2;
    int *num3_ptr = &num3;
    int *num4_ptr = &num4;

    size_t *key4_ptr = &key4;

    int test_flag = 0;

    new_hash_table = 
    HTCreate(hash_table_size, HashFuncTest, CompFuncTest);

    if (!HTIsEmpty(new_hash_table))
    {
        printf("test 1 failed \n");
        ++test_flag;
    }

    HTInsert(new_hash_table, num1_ptr);
    HTInsert(new_hash_table, num2_ptr);
    HTInsert(new_hash_table, num3_ptr);
    HTInsert(new_hash_table, num4_ptr);

    if (4 != HTSize(new_hash_table))
    {
        printf("test 2 failed \n");
        ++test_flag;
    }

    if (!HTFind(new_hash_table, num4_ptr))
    {
        printf("test 3 failed \n");
        ++test_flag;
    }

    HTRemove(new_hash_table, key4_ptr);

    if (HTFind(new_hash_table, num4_ptr))
    {
        printf("test 4 failed \n");
        ++test_flag;
    }

    printf("the load check of the hash table is %d\n", HashIsMaxLoad(new_hash_table));

    printf("the sd of the hash table is %f\n", HashBucketSD(new_hash_table));

    HTForEach(new_hash_table, HTFuncTest, num3_ptr);

    HTDestroy(new_hash_table);

    /***********************/

    if (0 == test_flag)
    {
        printf("Well done young padawan!\n");
    }
    else
    {
        printf("you have errors, many errors, this many errors %d\n", test_flag);
    }

    return 0;
}