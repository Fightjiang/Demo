#include "myShell.h" 

void signal_handle() { return ; } 
int main() {
    signal(SIGINT  , signal_handle) ; // father process catch Ctrl + C SIGINT signal , but do nothing
    signal(SIGTSTP , signal_handle) ; // father process catch Ctrl + \ SIGTSTP signal , but do nothing 
    
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
                int Pipeflag = -1 , backflag = -1 , input_redirect_flag = -1 , output_redirect_flag = -1; 
                for(i = 0 ; i < count - 1 ; ++i) {
                    if(strcmp(*(parameters + i) , "||") == 0) {
                        Pipeflag = i ;  
                    }
                    if(strcmp(*(parameters + i) , "&") == 0) {
                        backflag = i ;  
                    }
                    if(strcmp(*(parameters + i) , ">") == 0) {
                        output_redirect_flag = i ;  
                    }
                    if(strcmp(*(parameters + i) , "<") == 0) {
                        input_redirect_flag = i ;  
                    }
                }
                // 支持重定向 ">" , "<" , "||""
                if(input_redirect_flag != -1 || output_redirect_flag != -1) redirect(command , parameters , count - 1 , Pipeflag , input_redirect_flag , output_redirect_flag , backflag)  ;  
                // 支持管道
                else if(Pipeflag != -1) Pipe_command(command , parameters , count - 1 , Pipeflag) ; 
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
