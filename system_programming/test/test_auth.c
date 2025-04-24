/***************************************************
* Authenticator test                       
* Author: Jakob Shnider, Cr7                       
* Date of writing: 22/07/24                                
* Last Updated: 22/07/24                                    
* Reviewed by: uriel                                   
***************************************************/

#include <stdio.h> /*printf*/
#include <stdlib.h> /*system*/
#include "../include/auth.h"

static void PrintMsg(int status, char *name)
{
    if (1 == status)
    {
        printf("you are worthy to enter this domain my lord %s\n", name);
    }
}

int main()
{
    int test_flag = 0;

    int status = 0;

    AuthAddUser("jack","super123duper");
    AuthAddUser("umar","notsuper123notduper");

    status = AuthAddUser("jack","y2y");
    if (-1 != status)
    {
        printf("test 1 failed \n");
        ++test_flag;
    }

    AuthDeleteUser("jack");

    status = AuthAddUser("jack","yy");
    if (status)
    {
        printf("test 2 failed \n");
        ++test_flag;
    }

    status = AuthAuthentication("jack", "super123duper");
    PrintMsg(status, "jack");

    if (-1 != status)
    {
        printf("test 3 failed \n");
        ++test_flag;
    }

    status = AuthAuthentication("umar", "fdfsdfsdf");
    PrintMsg(status, "umar");

    if (-1 != status)
    {
        printf("test 4 failed \n");
        ++test_flag;
    }

    status = AuthAuthentication("uma22r", "fdfsdfsdf");
    PrintMsg(status, "uma22r");

    if (-1 != status)
    {
        printf("test 5 failed \n");
        ++test_flag;
    }

    status = AuthAuthentication("jack", "yy");
    PrintMsg(status, "jack");

    if (1 != status)
    {
        printf("test 6 failed \n");
        ++test_flag;
    }

    /***********************/

 	if (0 == test_flag)
    {
        printf("all tests passed!\n");
    }
    else
    {
    }

    system("rm hoho.txt"); 

	return 0;
}

