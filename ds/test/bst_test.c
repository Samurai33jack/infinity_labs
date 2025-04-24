/* bst api test 30/06/24 CR:"shahar not fadlon" */

#include <stdio.h> /* printf */
#include "../include/bst.h" /* for assert */ 

int IntComp(const void *data1, const void *data2)
{
    return (*(int *)data1 - *(int *)data2);
}

int PrintNode(void *data, void *params)
{
    (void)params;
    printf("%d\n", *(int *)data);
    
    return 0;
}

int main()
{   
    int test_flag = 0;
    
    int arr[] = {7, 21, 42, 3, 888, 777, 33, 11, 0, 2};

    bst_t *new_bst_test = NULL;

    new_bst_test = BSTCreate(IntComp);

    if (!BSTIsEmpty(new_bst_test))
    {
        printf("test 1 failed \n");
        ++test_flag;
    }
    BSTInsert(new_bst_test , &arr[0]); 
    BSTInsert(new_bst_test , &arr[1]); 
    BSTInsert(new_bst_test , &arr[2]); 
    BSTInsert(new_bst_test , &arr[3]); 
    BSTInsert(new_bst_test , &arr[4]); 
    BSTInsert(new_bst_test , &arr[5]); 
    BSTInsert(new_bst_test , &arr[6]); 
    BSTInsert(new_bst_test , &arr[7]); 
    BSTInsert(new_bst_test , &arr[8]); 
    BSTInsert(new_bst_test , &arr[9]); 

    printf("\n\n");
    Print2DTree(new_bst_test);
    printf("\n\n");

    if (10 != BSTSize(new_bst_test))
    {
        printf("test 2 failed \n");
        ++test_flag;
    }

    if (!BSTFind(new_bst_test, &arr[1]))
    {
        printf("test 3 failed \n");
        ++test_flag;
    }

    BSTRemove(new_bst_test, &arr[2]);

    printf("\n\n");
    Print2DTree(new_bst_test);
    printf("\n\n");

    if (BSTFind(new_bst_test, &arr[2]))
    {
        printf("test 4 failed \n");
        ++test_flag;
    }

    if (0 != BSTForEach(new_bst_test, PrintNode, NULL))
    {
        printf("test 5 failed \n");
        ++test_flag;
    }

    if (BSTDestroy(new_bst_test))
    {
        printf("test 6 failed \n");
        ++test_flag;
    }
    
    /***********************/

    if (0 == test_flag)
    {
        printf("\nAll tests passed!\n");
    }
    else
    {
        printf("you have errors, many errors, this many errors %d\n", test_flag);
    }

    return 0;
}
