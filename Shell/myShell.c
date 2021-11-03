#include "myShell.h" 

int main() {
    
    char *command = (char *) malloc(MAXLINE) , **parameters = (char **) malloc(sizeof(char*) * MAXPARAMTERS) ; 
    int i ;
    while(1) {
        type_prompt() ;  
        int count = read_command(command , parameters) ;
        if(count <= 0) {
            continue ; 
        }else {
            if(strcmp(command , "help") == 0 || strcmp(command , "exit") == 0 || strcmp(command , "cd") == 0) {
                // 内置命令
                if(strcmp(command , "exit") == 0) break ; 
                builtin_command(command , parameters , count - 1) ; 
            }else {
                int Pipeflag = -1 , backflag = -1; 
                for(i = 0 ; i < count - 1 ; ++i) {
                    if(strcmp(*(parameters + i) , "||") == 0) {
                        Pipeflag = i ; break; 
                    }
                    if(strcmp(*(parameters + i) , "&") == 0) {
                        backflag = i ; break; 
                    }
                }
                // 支持管道
                if(Pipeflag != -1) Pipe_command(command , parameters , count - 1 , Pipeflag) ; 
                // 后台运行
                else if(backflag != -1) commandInbackgroud(command , parameters , count - 2) ;  
                // 普通外部命令
                else External_command(command , parameters , count - 1) ;
            }
        }
        for(i = 0 ; i < MAXPARAMTERS ; ++i) {
            if((*(parameters + i)) != NULL) {
                free(*(parameters + i)) ; 
                *(parameters + i) = NULL ; 
            }
        }
    }
    free(command) ; 
    free(parameters) ; 
    return 0 ;
}
