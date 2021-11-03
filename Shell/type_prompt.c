#include "myShell.h"

void type_prompt(){
    struct passwd *pwd  = getpwuid(getuid()) ; /* A record in the user database.  */

    char *pathname = (char*) malloc(max_path_len)  , *hostname = (char *) malloc(max_name_len) ;  
   
    getcwd(pathname , max_path_len) ; // Get the pathname of the current working directory
    gethostname(hostname , max_name_len) ; //Put the name of the current host in no more than LEN bytes of NAME.
   
    /*
     1. ubuntu@VM-16-8-ubuntu:~/test$
     2. ubuntu@VM-16-8-ubuntu:/bin$
    */
   
    printf("%s@%s:" , pwd->pw_name , hostname) ; 
    
    // 在 home 目录下 , 前面应该用 ~ 代替
    if(strncmp(pathname , pwd->pw_dir , strlen(pwd->pw_dir)) == 0) {
        printf("~%s" , pathname + strlen(pwd->pw_dir)) ; 
    }else {
        printf("%s" , pathname) ; 
    } 

    if(geteuid() == 0)
		printf("#");
	else
		printf("$");
    free(pathname) ; 
    free(hostname) ; 
}
