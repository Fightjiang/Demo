#include "myShell.h"

void help() {
    printf("myShell bash, version 1.0 \n") ; 
    printf("These shell commands are defined internally. Type 'help' to see this list. \n") ; 
    printf("This Shell is ver simple , and if you want to add other functions , please contact me !\n\n") ; 

    printf("exit \t help \t cd \n")  ; 
} 

void builtin_command(char *command , char **parameters , int count) {
    if(strcmp(command , "exit") == 0) {
        exit(0) ; 
    }else if(strcmp(command , "help") == 0) {
        help() ; 
    }else if(strcmp(command , "cd") == 0) {

        char *my_path = (char *) malloc(MAXLINE) ; 
        char *oldPath = (char *) malloc(MAXLINE) ; 
        char *newPath = (char *) malloc(MAXLINE) ; 
        getcwd(oldPath , MAXLINE) ; 
        memset(my_path , 0 , sizeof(my_path)) ;
        if(count > 1) {
            printf("bash: cd: too many arguments\n") ; 
        }else {
            struct passwd *pwd  = getpwuid(getuid()) ;
            // 没有参数 或者 带有 ~ 默认回到家目录
            if(count == 0 || parameters[0][0] == '~') {
                strcat(my_path , "/home/");
                strcat(my_path ,  pwd->pw_name) ;
                if(count > 0 && parameters[0][0] == '~') {
                    strcat(my_path , parameters[0] + 1) ; 
                }
            }else if(parameters[0][0] == '.') {
                if(parameters[0][1] == '\0' || strcmp(pwd->pw_dir , parameters[0]) == 0) { // 本级目录
                    getcwd(my_path , MAXLINE) ; 
                }else {
                    if(parameters[0][1] == '.') { // .. 上级目录 ../test1
                        getcwd(my_path , MAXLINE) ; 
                        int i = 0 , end = 0 ; 
                        for(i = strlen(my_path) - 1 ; i >= 0 ; --i) {
                            if(my_path[i] == '/') {
                                end = i; break ; 
                            }
                        }
                        int pos = end ; 
                        for(i = 2 ; i < strlen(parameters[0]) ; ++i) {
                            my_path[pos++] = parameters[0][i] ; 
                        }
                        my_path[pos] = '\0' ; 
                      //  printf("my_path %s %d \n" , my_path , pos) ; 
                    }else {// ./test1
                        int i = 0 , pos = 0 ; 
                        for(i = 0 ; i < strlen(pwd->pw_dir) ; ++i) {
                            my_path[pos++] = pwd->pw_dir[i] ; 
                        }
                        for(i = 1 ; i < strlen(parameters[0]) ; ++i) {
                            my_path[pos++] = parameters[0][i] ; 
                        }
                        my_path[pos] = '\0' ; 
                    }
                }
            }else { // 最普通的
                int i = 0 , pos = 0 ; 
                for(i = 0 ; i < strlen(parameters[0]); ++i) {
                    my_path[pos++] = parameters[0][i] ; 
                }//printf("\n\n test3: %s %s %s %d\n\n\n" , path ,parametes[0] , path , strlen(parametes[0])) ; 
                my_path[pos] = '\0' ; 
            }
            /*
                利用目录是否发生修改 来判断 cd  的目录是否存在
                newPath , 
            */
            // printf("my_path %s\n" , my_path) ; 
            chdir(my_path) ; // 切换目录 
            getcwd(newPath , MAXLINE) ;  // 获取最新的目录
            //printf("oldPath %s , pwd->pw_dir %s , my_path = %s\n" , oldPath , pwd->pw_dir , my_path) ;
            if(strcmp(oldPath , newPath) == 0 && strcmp(oldPath , my_path) != 0) {
                printf("bash: cd : %s : No such file or directory\n" , parameters[0]) ; 
            }
            free(my_path) ;
            free(oldPath) ; 
            free(newPath) ; 
        }    
    }
}
void External_command(char *command , char **parameters , int count) {
    char ** argv = (char **) malloc(sizeof(char *) * (count + 2)) ;
    memset(argv , 0 , sizeof(argv)) ;  
    *argv = (char *) malloc(strlen(command) + 1) ; 
    strcpy(*argv , command) ; 
    int i = 1 ; 
    for(i = 1 ; i <= count ; ++i) {
        *(argv + i) = (char *) malloc(strlen(parameters[i-1]) + 1) ;
        strcpy(*(argv + i) , parameters[i-1]) ;
        (*(argv + i))[strlen(parameters[i-1])] = '\0' ; 
    }
    // exec 要求要第二个参数以空指针结束 
    *(argv + count + 1) = NULL ; 
    // exec 系列的系统调用是把当前程序替换成要执行的程序，已经和本程序无光了
    pid_t pid = fork() ; 
    if(pid > 0) {
        waitpid(pid , 0 , 0 ) ; // 父进程等待子进程结束
    }else if(pid == 0 ){
        if(execvp(command , argv) == -1) {
            printf("%s : command not foud\n" , command) ; 
            printf("shell command execute error!!!\n") ; 
            exit(EXIT_FAILURE) ; 
        } 
    }else {
        perror("fork error") ; 
    }
    for(i = 0 ; i <= count  ; ++i) {
        free(*(argv + i)) ; 
    }
    free(argv) ; 
}
// 将前一进程的标准输出重定向到管道的写数据端，将后一进程的标准输入重定向到管道的读数据端
void Pipe_command(char *command , char **parameters , int count , int pos ) {
    // 将两个命令拆开
    char ** arg_vec1 = (char **) malloc(sizeof(char *) * (pos + 2)) ; // 第一条命令
    /*
        这一块可以做一个代码上的优化，将命令 全部放入 parameters 中，这样就不用另外再做拆分了。
    */
    memset(arg_vec1 , 0 , sizeof(arg_vec1)) ;  
    *arg_vec1 = (char *) malloc(strlen(command) + 1) ; 
    strcpy(*arg_vec1 , command) ; 
    int i = 1 ; 
    for(i = 1 ; i <= pos ; ++i) {
        *(arg_vec1 + i) = (char *) malloc(strlen(parameters[i-1]) + 1) ;
        strcpy(*(arg_vec1 + i) , parameters[i-1]) ;
        (*(arg_vec1 + i))[strlen(parameters[i-1])] = '\0' ; 
    }
    *(arg_vec1 + pos + 1) = NULL ; 

    char ** arg_vec2 = &parameters[pos + 1] ; // 第二条命令
    *(arg_vec2 + count ) = NULL ; 

    int pfd[2]  ; 
    if(pipe(pfd) == -1) {
        perror("pipe error") ; 
        exit(EXIT_FAILURE) ; 
    }
    // 创建子进程执行 命令 1 
    pid_t pid = fork() ; 
    if(pid == 0) {
        // 关闭管道的读数据端
        if(close(pfd[0]) == -1) {
            perror("close pipe[0] error") ; 
            exit(EXIT_FAILURE) ; 
        }
        if(pfd[1] != STDOUT_FILENO){
            // 将标准输出重定向到管道的写数据端
            if(dup2(pfd[1] , STDOUT_FILENO) == -1) {
                perror("dup2 error") ; 
                exit(EXIT_FAILURE) ; 
            }
        }
        if(close(pfd[1]) == -1) {
            perror("close pipe[1] error") ; 
            exit(EXIT_FAILURE) ; 
        }
        // 执行命令 
        if(execvp(*arg_vec1 , arg_vec1) == -1) {
            printf("%s : command not foud\n" , command) ; 
            printf("shell command execute error!!!\n") ; 
            exit(EXIT_FAILURE) ; 
        }
    }else if(pid > 0){
        waitpid(pid , 0 , 0 ) ; // 父进程等待子进程结束
    }else {
        perror("fork error") ; 
    }

    // 再次创建子进程执行 命令 2
    pid = fork() ; 
    if(pid == 0) {
        // 关闭管道的写数据端
        if(close(pfd[1]) == -1) {
            perror("close pipe[0] error") ; 
            exit(EXIT_FAILURE) ; 
        }
        if(pfd[0] != STDIN_FILENO){
            // 将标准输入重定向到管道的读数据端
            if(dup2(pfd[0] , STDIN_FILENO) == -1) {
                perror("dup2 error") ; 
                exit(EXIT_FAILURE) ; 
            }
        }
        if(close(pfd[0]) == -1) {
            perror("close pipe[1] error") ; 
            exit(EXIT_FAILURE) ; 
        }
        // 执行命令 
        if(execvp(*arg_vec2 , arg_vec2) == -1) {
            printf("%s : command not foud\n" , command) ; 
            printf("shell command execute error!!!\n") ; 
            exit(EXIT_FAILURE) ; 
        }
    }
    // 进程A不需要管道通信，关闭管道的两端
    if (close(pfd[0]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    if (close(pfd[1]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    waitpid(pid , 0 , 0 ) ; // 父进程等待子进程结束
    for(i = 0 ; i <= pos + 1  ; ++i) {
        free(*(arg_vec1 + i)) ; 
        *(arg_vec1 + i) = NULL ; 
    }
    free(arg_vec1) ; 
}

/*
为了屏蔽键盘和控制台，把子进程的标准输入、输出映射成 /dev/null ，

signal(SIGCHLD,SIG_IGN) 父进程忽略 SIGCHLD 信号，内核把僵尸子进程转交给init进程去处理
这样 Shell 可以避免调用 wait/waitpid 从而直接运行下一条命令。

*/


void commandInbackgroud(char *command , char **parameters , int count) {
    char ** argv = (char **) malloc(sizeof(char *) * (count + 2)) ;
    memset(argv , 0 , sizeof(argv)) ;  
    *argv = (char *) malloc(strlen(command) + 1) ; 
    strcpy(*argv , command) ; 
    int i = 1 ; 
    for(i = 1 ; i <= count ; ++i) {
        *(argv + i) = (char *) malloc(strlen(parameters[i-1]) + 1) ;
        strcpy(*(argv + i) , parameters[i-1]) ;
        (*(argv + i))[strlen(parameters[i-1])] = '\0' ; 
    }
    // exec 要求要第二个参数以空指针结束 
    *(argv + count + 1) = NULL ; 

    signal(SIGCHLD,SIG_IGN); // 忽略子进程的信号，转为 init 接收
    pid_t pid = fork() ; 
    if(pid == 0) {
        //屏蔽键盘和控制台，把子进程的标准输入、输出映射成 /dev/null ，
        freopen( "/dev/null", "w", stdout );
        freopen( "/dev/null", "r", stdin ); 

        if(execvp(command , argv) == -1) {
            printf("%s : command not foud\n" , command) ; 
            printf("shell command execute error!!!\n") ; 
            exit(EXIT_FAILURE) ; 
        } 
    }

    for(i = 0 ; i <= count  ; ++i) {
        free(*(argv + i)) ; 
    }
    free(argv) ; 
}


#ifdef DEBUG  

int main() {
    char commad[10] = "ls" ; 
    char **parametes = (char **) malloc(sizeof(char*) * MAXPARAMTERS) ; 
    
    *parametes = (char *) malloc(10) ; 
    strcpy(*parametes , "-al") ; 
    *(parametes + 1) = (char *) malloc(10) ; 
    strcpy(*(parametes + 1) , "||") ; 
    *(parametes + 2) = (char *) malloc(10) ; 
    strcpy(*(parametes + 2) , "grep") ; 
    *(parametes + 3) = (char *) malloc(10) ; 
    strcpy(*(parametes + 3) , "test") ; 
    type_prompt() ; getchar() ;
    
    Pipe_command(commad , parametes , 4 , 1) ; 
    getchar() ; 
    int i = 0 ; 
    for(i = 0 ; i < 2 ; ++i) {
        free(*(parametes + i)) ; 
    }
    free(parametes) ; 
    
   // printf("%s\n" , commad) ; 
    return 0 ;
}


#endif