/* sll api lib  30/06/24 CR:"ori"*/

#include "../include/sll.h"
#include <stdio.h>

int main()
{   
    SLL_t *new_list = NULL;

    SLL_iter_t new_list_iter = NULL;
    SLL_iter_t new_list2_iter = NULL;
    SLL_iter_t new_list3_iter = NULL;

    char c_num1 = 24;
    char *c_num1_ptr = &c_num1;
    
    int num1 = 42;
    int num2 = 88;
    
    int *num1_ptr = &num1;
    int *num2_ptr = &num2;

    int test_flag = 0;

    new_list = SLLCreate();
    new_list_iter = SLLFirst(new_list);

    if (!SLLIsEmpty(new_list))
    {
        printf("test 1 failed \n");
        ++test_flag;
    }

    new_list3_iter = SLLInsert(new_list, new_list_iter, num1_ptr);
    new_list3_iter = SLLInsert(new_list, new_list_iter, num2_ptr);
    new_list3_iter = SLLInsert(new_list, new_list_iter, num1_ptr);

    if (3 != SLLCount(new_list))
    {
        printf("test 2 failed \n");
        ++test_flag;
    }

    new_list_iter = SLLFind(new_list, num1_ptr);
    new_list2_iter = SLLFind(new_list, num2_ptr);

    if (!SLLIsEqual(new_list_iter, new_list2_iter))
    {
        printf("test 3 failed \n");
        ++test_flag;
    }

    if (SLLIsEqual(new_list_iter, new_list3_iter))
    {
        printf("test 4 failed \n");
        ++test_flag;
    }

    SLLSetData(new_list3_iter, c_num1_ptr);

    if (c_num1_ptr != SLLGetData(new_list3_iter))
    {
        printf("test 5 failed \n");
        ++test_flag;
    }
    new_list2_iter = SLLFirst(new_list);

    SLLRemove(new_list, new_list2_iter);

    SLLDestroy(new_list);

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