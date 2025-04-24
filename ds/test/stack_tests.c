/* stack api tests  27/06/24 CR:"omer"*/

#include "../include/stack.h"
#include <stdio.h> /* for printf*/

int main()
{	
	stack_t *new_stack = NULL;
	stack_t *new_stack2 = NULL;
	stack_t *new_stack3 = NULL;
	stack_t *new_stack4 = NULL;

	char c_num1 = 24;
	char *c_num1_ptr = &c_num1;

	short s_num1 = 124;
	short *s_num1_ptr = &s_num1;
	
	int num1 = 42;
	int num2 = 88;
	int *num1_ptr = &num1;
	int *num2_ptr = &num2;

	double d_num1 = 42.124142;
	double *d_num1_ptr = &d_num1;

	int test_flag = 0;

	int small_stack = 2;
	int big_stack = 10;

	new_stack = StackCreate(sizeof(int), big_stack);
	new_stack2 = StackCreate(sizeof(double), big_stack);
	new_stack3 = StackCreate(sizeof(char), small_stack);
	new_stack4 = StackCreate(sizeof(short), small_stack);

	if (StackIsEmpty(new_stack))
	{
		printf("test 1 failed \n");
		++test_flag;
	}

	if ((size_t)big_stack != StackCapacity(new_stack))
	{
		printf("test 2 failed \n");
		++test_flag;
	}
	
	StackPush(new_stack, num1_ptr);

	if (num1 != *(int *)StackPeek(new_stack))
	{
		printf("test 3 failed \n");
		++test_flag;
	}

	StackPush(new_stack, num2_ptr);

	if (num2 != *(int *)StackPeek(new_stack))
	{
		printf("test 4 failed \n");
		++test_flag;
	}
	
	if (2 != StackSize(new_stack))
	{
		printf("test 5 failed \n");
		++test_flag;
	}

	StackPop(new_stack);

	if (1 != StackSize(new_stack))
	{
		printf("test 6 failed \n");
		++test_flag;
	}

		if (num1 != *(int *)StackPeek(new_stack))
	{
		printf("test 7 failed \n");
		++test_flag;
	}

	StackDestory(new_stack);

	/***********************/

	StackPush(new_stack2, d_num1_ptr);

	if (d_num1 != *(double *)StackPeek(new_stack2))
	{
		printf("test 8 failed \n");
		++test_flag;
	}

	StackPop(new_stack2);
	StackPop(new_stack2);
	printf("if stack underflow then test passed \n");

	/***********************/

	StackPush(new_stack3, c_num1_ptr);

	if (c_num1 != *(char *)StackPeek(new_stack3))
	{
		printf("test 10 failed \n");
		++test_flag;
	}

	/***********************/

	StackPush(new_stack4, s_num1_ptr);

	if (s_num1 != *(short *)StackPeek(new_stack4))
	{
		printf("test 11 failed \n");
		++test_flag;
	}

	StackPush(new_stack4, s_num1_ptr);
	StackPush(new_stack4, s_num1_ptr);

	printf("if stack overflow then test passed \n");

	StackDestory(new_stack2);
	StackDestory(new_stack3);
	StackDestory(new_stack4);

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
