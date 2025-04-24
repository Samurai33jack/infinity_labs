/* ext2 parser api tests  17/07/24 CR:"mr fadlon"*/

#include <stdio.h> /* for printf */

#include "../include/ExtParser_fs.h"
#include "../include/ExtParser.h" 

#define BUFFER_SIZE 1024

int main()
{
	ext2_t *new_fs = NULL;
	char *device = "/dev/ram0";
	char buffer[BUFFER_SIZE] = "";
	int inode_index = 0;
	int inode_size = 0;
    int test_flag = 0;
    unsigned short inode_new_permission = 456;

	new_fs = ExtOpen(device);

	if (!new_fs)
    {
        printf("test 1 failed \n");
        ++test_flag;
    }


	inode_index = ExtGetFileInode(new_fs, "/jack/hoho.txt");

 	if (13 != inode_index)
    {
        printf("test 2 failed \n");
        ++test_flag;
    }


	inode_size = ExtGetFilesize(new_fs, inode_index);

	if (82 != inode_size)
    {
        printf("test 3 failed \n");
        ++test_flag;
    }

    ExtReadBytes(new_fs, inode_index, buffer, inode_size);

    printf("buffer is: \n\n%s\n\n", buffer);

   
    if (ExtChmod(new_fs, inode_index, inode_new_permission))
    {
        printf("test 3.3 failed \n");
        ++test_flag;
    }
    /***********************/


	if (ExtClose(new_fs))
    {
        printf("test 4 failed \n");
        ++test_flag;
    }
    
 	if (0 == test_flag)
    {
        printf("all tests passed!\n");
    }
    else
    {
        printf("you have errors, many errors, this many errors %d\n", test_flag);
    }
	return 0;
}
