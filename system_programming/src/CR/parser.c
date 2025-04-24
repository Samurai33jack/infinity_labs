/***************************************************
* Ext-2 File System Parser lib                       
* Author: Jakob Shnider, Cr7                       
* Date of writing: 16/07/24                                
* Last Updated: 21/07/24                                    
* Reviewed by: "mr fadlon" , dan the man for chmod                                     
***************************************************/

#include <stdio.h> /*size_t*/
#include <stdlib.h> /*malloc*/
#include <unistd.h> /*seek & read*/
#include <fcntl.h> /*open*/
#include <string.h> /*strcmp & strcat*/
#include <assert.h> /*assert*/

#include "../include/ExtParser_fs.h" /*ext2*/
#include "../include/ExtParser.h" /*ext2 parser*/

#define ROOT_INODE 2  
#define DIGITS_AMOUNT 4  
#define PERMISSIONS_BIT_AMOUNT 12  
#define MAX_PERMISSION_VAL 7  
#define BIT_SET 3  

#define BLOCK_OFFSET(block) ((block)*block_size)

static unsigned int block_size = 0;
static unsigned int inode_size = 0; 

struct ext2 {
	int fd;
	struct ext2_super_block *super_block;
};

static void ReadInode(int fd, int inode_no, const struct ext2_group_desc *group, struct ext2_inode *inode)
{
	lseek(fd, BLOCK_OFFSET(group->bg_inode_table)+((inode_no -1) * inode_size), SEEK_SET);
	read(fd, inode, sizeof(struct ext2_inode));
}

static int ReadDir(int fd, const struct ext2_inode *inode, const char *name)
{
	void *block = NULL;
	char file_name[EXT2_NAME_LEN];
	struct ext2_dir_entry_2 *entry;
	unsigned int amount_of_entries = 0;
	int inode_index = -1;

	size_t i = 0;

	block = (void *)malloc(block_size);

	if (NULL == block) 
	{ 
		fprintf(stderr, "Memory error\n");
		close(fd);
		return -1;
	}

	for (i = 0; i < DIRECT_BLOCKS; ++i)
	{
		lseek(fd, BLOCK_OFFSET(inode->i_block[i]), SEEK_SET);
		read(fd, block, block_size);  

		while ((NULL != block) && (amount_of_entries < inode->i_size)) 
		{
        	entry = (struct ext2_dir_entry_2 *)((char *)block + amount_of_entries);   

        	if((NULL == entry) || (0 == entry->inode))        
        	{
        		break;
        	}   

        	memcpy(file_name, entry->name, entry->name_len);

			if(0 == strncmp(file_name, name, entry->name_len))         
        	{	
        		inode_index = entry->inode;
        		free(block);
        		return inode_index;
        	}

        	amount_of_entries += entry->rec_len;
    	}	
	}

	free(block);
	return inode_index;
}

ext2_t *ExtOpen(const char *device)
{
	int fd = 0;

	ext2_t *new_ext2 = NULL;

	assert(device);

	new_ext2 = (ext2_t*)malloc(sizeof(ext2_t));
	new_ext2->super_block = (struct ext2_super_block *)malloc(sizeof(struct ext2_super_block));

	fd = open(device, O_RDWR); 

	lseek(fd, BASE_OFFSET, SEEK_SET);
	read(fd, new_ext2->super_block, sizeof(struct ext2_super_block));

	if (EXT2_SUPER_MAGIC != new_ext2->super_block->s_magic)
	{
		close(fd);
		free(new_ext2->super_block);
		free(new_ext2);
		return NULL;
	}

	block_size = EXT2_BLOCK_SIZE(new_ext2->super_block);
	inode_size = new_ext2->super_block->s_inode_size;
	
	new_ext2->fd = fd;

	return new_ext2;
}

int ExtClose(ext2_t *fs)
{	
	int status = 0;
	assert(fs);

	status = close(fs->fd);
	free(fs->super_block);
	free(fs);

	return status;
}

static void ReadGroup(int fd, int offset, struct ext2_group_desc *group)
{
	lseek(fd, block_size + (offset * block_size), SEEK_SET);
	read(fd, group, sizeof(struct ext2_group_desc));
}

int ExtGetFileInode(const ext2_t *fs, const char *path_to_file)
{
	char str[EXT2_NAME_LEN] = "";
	const char delimiters[] = "/";
    	char *token = NULL;

	int inode_index = ROOT_INODE; 
	int offset = inode_index / fs->super_block->s_inodes_per_group;

	static struct ext2_group_desc group;
	static struct ext2_inode inode;

	assert(fs && path_to_file);

	if ((!fs) || (!path_to_file))
	{
		return -1;
	}
	
	ReadGroup(fs->fd, offset, &group);
	ReadInode(fs->fd, inode_index, &group, &inode);

    	strcpy(str, path_to_file);
    	token = strtok(str, delimiters);
   
    	while (NULL != token)
    	{
    		inode_index = ReadDir(fs->fd, &inode, token);
    		ReadInode(fs->fd, inode_index, &group, &inode);
		
    		token = strtok(NULL, delimiters);
    	}

    	return inode_index; 
}

int ExtReadBytes(const ext2_t *fs, const int inode_num, char *buffer, const size_t bytes_to_read)
{
	int offset = 0;
	size_t cur_bytes_to_read = 0;
	size_t i = 0;
	void *block = NULL;

	static struct ext2_group_desc group;
	static struct ext2_inode inode_temp;

	assert(fs);

	cur_bytes_to_read = bytes_to_read;

	offset = inode_num / fs->super_block->s_inodes_per_group;

	ReadGroup(fs->fd, offset, &group);
	ReadInode(fs->fd, inode_num, &group, &inode_temp);

	block = malloc(block_size);

	if (NULL == block) 
	{ 
		fprintf(stderr, "Memory error\n");
		close(fs->fd);
		return -1;
	}

	for (i = 0; i < DIRECT_BLOCKS; ++i)
	{
		lseek(fs->fd, BLOCK_OFFSET(inode_temp.i_block[i]), SEEK_SET);
		read(fs->fd, block, block_size);  

		if (cur_bytes_to_read > block_size)
		{
			strncat(buffer, block, block_size);
			cur_bytes_to_read -= block_size;
		}
		else
		{
			strncat(buffer, block, cur_bytes_to_read);
			buffer[cur_bytes_to_read - 1] = '\0';

			free(block);
			return 0;
		}
	}

	free(block);
	return -1;
}

long ExtGetFilesize(const ext2_t *fs, const int inode)
{
	int offset = 0;

	static struct ext2_group_desc group;
	static struct ext2_inode inode_temp;

	assert(fs);

	offset = inode / fs->super_block->s_inodes_per_group;

	ReadGroup(fs->fd, offset, &group);
	ReadInode(fs->fd, inode, &group, &inode_temp);

	return inode_temp.i_size;
}

int ExtChmod(const ext2_t *fs, const int inode_num, unsigned short new_mode)
{
	int offset = 0;
	int status = 0;
	size_t i = 0;

	__u16 temp_new_mode = new_mode;
	__u16 cur_division = 1000;
	__u16 cur_permission = 0;
	__u16 temp_old_mode = 0;

	static struct ext2_group_desc group;
	static struct ext2_inode inode_temp;

	assert(fs);

	offset = inode_num / fs->super_block->s_inodes_per_group;

	ReadGroup(fs->fd, offset, &group);
	ReadInode(fs->fd, inode_num, &group, &inode_temp);

	temp_old_mode = inode_temp.i_mode;
	temp_old_mode >>= PERMISSIONS_BIT_AMOUNT;

	for (i = 0; i < DIGITS_AMOUNT; ++i)
	{
		cur_permission = temp_new_mode / cur_division;

		if (7 < cur_permission)
		{
			return -1;
		}

		temp_old_mode <<= BIT_SET;
		temp_old_mode |= cur_permission;

		temp_new_mode %= cur_division;
		cur_division /= 10;
	}

	inode_temp.i_mode = temp_old_mode;
	
	offset = BLOCK_OFFSET(group.bg_inode_table) + ((inode_num -1) * inode_size);
	
	lseek(fs->fd, offset, SEEK_SET);
	status = write(fs->fd, &inode_temp, sizeof(struct ext2_inode));

	return (status ? 0 : -1);
}

