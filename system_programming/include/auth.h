/***************************************************
* Authenticator header                       
* Author: Jakob Shnider, Cr7                       
* Date of writing: 22/07/24                                
* Last Updated: 22/07/24                                    
* Reviewed by:                                    
***************************************************/

#ifndef __AUTH_CR7_H__
#define __AUTH_CR7_H__

/*
Receives: pointer to username and passowrd chars.
add the username and the hash password to files.
Returns: return 0 if worked, otherwise -1.
*/
int AuthAddUser(const char *username, const char *password);

/*
Receives: pointer to username char.
remove the given username and his password from the files.
Returns: return 0 if worked, otherwise -1.
*/
int AuthDeleteUser(const char *username);

/*
Receives: pointer to username and passowrd chars.
check is the given username is in the file and if the associated password is valid.
Returns: return 0 if worked, otherwise -1.
*/
int AuthAuthentication(const char* username, const char *password);

#endif