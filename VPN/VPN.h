 /***************************************************
*NAME:  VPN HEADER                                 *
*WRITTEN_BY:OMER.DAHAN                             *
*DATE:04/09/2024                                   *
*Reviewed By:                                      *
***************************************************/

#ifndef __VPN_H__
#define __VPN_H__

#include <stdio.h>

int OpenSocket();


int SetUpRouting();


int CleanUp(int fd, int socket);


int Encrypt(char *plain_text, char *cipher_text, int len);


int Decrypt(char *plain_text, char * cipher_text, int len);


int VPNHandler(FILE *fd);

#endif
