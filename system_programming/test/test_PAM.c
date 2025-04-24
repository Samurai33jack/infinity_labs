/***************************************************
* Authenticator PAM test                       
* Author: Jakob Shnider, Cr7                       
* Date of writing: 22/07/24                                
* Last Updated: 22/07/24                                    
* Reviewed by: uriel                                   
***************************************************/
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>

static struct pam_conv conv = {
    misc_conv,
    NULL
};

static void PrintMsg(int status, char *name);
int PAMMagic(char *user);

int main()
{
    char *user1 = "jakobshnider";
    char *user2 = "Umar";
    char *user3 = "labuser1";

    int status = 0;

    printf("user: %s\n", user1);
    status = PAMMagic(user1);
    PrintMsg(status, user1);
    
    printf("user: %s\n", user2);
    status = PAMMagic(user2);
    PrintMsg(status, user2);

    printf("user: %s\n", user3);
    status = PAMMagic(user3);
    PrintMsg(status, user3);
    
    return 0; 
}

static void PrintMsg(int status, char *name)
{
    if (0 == status)
    {
        printf("you are worthy to enter this domain my lord %s\n", name);
    }
}

int PAMMagic(char *user)
{
    pam_handle_t *pamh = NULL;
    int return_val = 0; 

    return_val = pam_start("check", user, &conv, &pamh);

    if (return_val == PAM_SUCCESS)
    {
        return_val = pam_authenticate(pamh, 0);    /* is user really user? */
    }

    if (return_val == PAM_SUCCESS)
    {
        return_val = pam_acct_mgmt(pamh, 0);       /* permitted access? */
    }

    if (return_val == PAM_SUCCESS) 
    {
    	fprintf(stdout, "Authenticated\n");
    } 
    else 
    {
    	fprintf(stdout, "Not Authenticated\n");
    }

    if (pam_end(pamh,return_val) != PAM_SUCCESS) 
    {    
    	pamh = NULL;
    	fprintf(stderr, "check_user: failed to release authenticator\n");
    	exit(1);
    }

    return ( return_val == PAM_SUCCESS ? 0:1 ); 
}

