/***************************************************
* Authenticator lib                       
* Author: Jakob Shnider, Cr7                       
* Date of writing: 22/07/24                                
* Last Updated: 22/07/24                                    
* Reviewed by: uriel                                   
***************************************************/

#include <stdio.h> /*size_t*/
#include <unistd.h> /*seek & read*/

#include <string.h> /*strcmp & strcat*/
#include <assert.h> /*assert*/
#include <crypt.h> /*crypt*/

#include "../include/auth.h" /*auth*/

#define CREDENTIALS_FILE_PATH "hoho.txt"
#define BUFFER_SIZE 512

static int FindInFile (FILE *file_ptr, const char *str)
{
    char buffer[BUFFER_SIZE];
    int is_found = 0;

	while ((!is_found) && (NULL != fgets(buffer, BUFFER_SIZE, file_ptr)))
	{
 		if(NULL != (strstr(buffer, str))) 
 		{
			is_found = 1;
		}

	}

    return is_found;
}

int AuthAddUser(const char *username, const char *password)
{
	FILE *file_ptr = NULL;
	char entry[BUFFER_SIZE] = {0};
	char *hashed_passwd = NULL;

	int status = 0;

	file_ptr = fopen(CREDENTIALS_FILE_PATH, "a+");

	if ((NULL == file_ptr) || (FindInFile(file_ptr, username)))
	{
		status = -1;
	}

	fseek(file_ptr, 0, SEEK_END);

	if (-1 != status)
	{	

		hashed_passwd = crypt(password, username);

		strcat(entry, username);
		strcat(entry, ":");
		strcat(entry, hashed_passwd);	

		fprintf(file_ptr, "%s\n", entry);
	}
	
	fclose(file_ptr);

	return status;
}

static int DeleteLine(FILE *file_ptr, const char *username)
{
	FILE *temp_ptr = NULL;

	char buffer[BUFFER_SIZE] = {0};
   	int status = 0;

	temp_ptr = fopen("delete.tmp", "w+");
   
	if (NULL == temp_ptr)
	{
		status = -1;
	}

   	while (NULL != (fgets(buffer, BUFFER_SIZE, file_ptr)))
   	{
   		if (strncmp(buffer, username, strlen(username)))
        {
			fputs(buffer, temp_ptr);
      	}
    }

    fclose(file_ptr);
    fclose(temp_ptr);

    remove(CREDENTIALS_FILE_PATH);
   	rename("delete.tmp", CREDENTIALS_FILE_PATH);

   	return status;
}

int AuthDeleteUser(const char *username)
{
	FILE *file_ptr = NULL;
   	int status = 0;

   	file_ptr = fopen(CREDENTIALS_FILE_PATH, "r+");

   	if ((FindInFile(file_ptr, username)))
   	{
   		rewind(file_ptr);
   		status = DeleteLine(file_ptr, username);
   	}

    return status;
}

int AuthAuthentication(const char* username, const char *password)
{
	FILE *file_ptr = NULL;
	char buffer[BUFFER_SIZE] = {0};
	char *hashed_passwd = NULL;
   	char *old_passwd = NULL;

	int status = 0;
	int is_found = 0;

	file_ptr = fopen(CREDENTIALS_FILE_PATH, "r+");

	is_found = (FindInFile(file_ptr, username));

	if ((NULL == file_ptr) || (1 != is_found))
	{
		status = -1;
	}

	rewind(file_ptr);

	while ((0 == status) && (NULL != (fgets(buffer, BUFFER_SIZE, file_ptr))))
   	{
   		if (0 == strncmp(buffer, username, strlen(username)))
        {
			old_passwd = strtok(buffer, ":");
			old_passwd = strtok(NULL, ":");

			hashed_passwd = crypt(password, username);
		
			if (!strncmp(hashed_passwd, old_passwd, strlen(hashed_passwd)))
			{
				status = 1;
			}
			else
			{
				status = -1;
			}
      	}
    }
	
    fclose(file_ptr);

    return status;
}

