/* ext2 parser api header  17/07/24 CR:"mr fadlon"*/

#include <stdio.h> /*size_t*/


/* file names: ExtParser.h ExtParser.c ExtParser_test.c */
#ifndef __EXTPARSER_CR7_H__
#define __EXTPARSER_CR7_H__

/*--------------------------------------------------------------------------*/

typedef struct ext2 ext2_t;
/*--------------------------------------------------------------------------*/
/*
Receives: path to device.
initialinzing a file system struct
Returns: pointer to struct of ext_t. if failed, return NULL.
*/
ext2_t *ExtOpen(const char *device);

/*--------------------------------------------------------------------------*/
/*
Receives: pointer to struct of ext_t with file system.
frees all memory allocations.
Returns: 0 for success and -1 for failure.
*/
int ExtClose(ext2_t *fs);

/*--------------------------------------------------------------------------*/
/*
Receives: pointer to bottom of fs, char with path to the file.
Returns: int with the number of the inode,  or -1 if fail.
*/
int ExtGetFileInode(const ext2_t *fs, const char *path_to_file);

/*--------------------------------------------------------------------------*/
/*
Receives: pointer to struct of ext_t,
int number of the inode,
char pointer to write the data why read from the file,
size_t amount of bytes to read from the file. 
Returns: 0 for success and -1 for failure.
*/
int ExtReadBytes(const ext2_t *fs, const int inode_num, char *buffer, const size_t bytes_to_read);

/*--------------------------------------------------------------------------*/
/*
Receives: pointer to struct of ext_t.
Returns: positive long with size of the file, -1 if fail.
*/
long ExtGetFilesize(const ext2_t *fs, const int inode);

/*--------------------------------------------------------------------------*/

/*
Receives: pointer to struct of ext_t,
int of inode number,
short of new mode to set.
Returns: 0 if success or -1 for failure.
*/
int ExtChmod(const ext2_t *fs, const int inode_num, unsigned short new_mode);
#endif
