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

void exec_redirect(char **argv ,int count) {
    int inputPos = 0 , i = 0 , outputPos = 0 ; 
    for(i = 0 ; i <= count ; ++i) {
        if(strcmp(*(argv + i) , ">") == 0) outputPos = i ; // Pipe nums ;
        if(strcmp(*(argv + i) , "<") == 0) inputPos = i ; // Pipe nums ;
    }
    if(outputPos != 0){
        char *outfilename = argv[outputPos + 1] ; 
        int fdout = open(outfilename , O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(fdout == -1) { perror("open file") ; exit(1) ; }
        if(dup2(fdout , STDOUT_FILENO) == -1) { perror("dup2") ; exit(1) ; }
        if(close(fdout) == -1) { perror("close") ; exit(1) ;} 
    }
    if(inputPos != 0) {
        char *infilename = argv[inputPos + 1] ; 
        int fdin = open(infilename , O_RDONLY) ;
        if(fdin == -1) { perror("open file") ; exit(1) ; }
        if(dup2(fdin , STDIN_FILENO) == -1) { perror("dup2") ; exit(1) ; }
        if(close(fdin) == -1) { perror("close") ; exit(1) ;} 
    }

    char **arg_ver = (char **) malloc(sizeof(char *) * (count + 2)) ;
    int j = 0 ; 
    for(i = 0 ; i <= count ; ++i , ++j) {
        if(strcmp(*(argv + i) , "<") == 0 || strcmp(*(argv + i) , "||") == 0 || strcmp(*(argv + i) , ">") == 0) break ; 
        *(arg_ver + j) = (char *) malloc(strlen(argv[i]) + 1) ;
        strcpy(*(arg_ver + j) , argv[i]) ;
        (*(arg_ver + j))[strlen(argv[i])] = '\0' ; 
    }   
    arg_ver[j] = NULL ; 
    if(execvp(*arg_ver , arg_ver) == -1) {
        printf("%s : command not foud\n" , arg_ver[0]) ; 
        printf("shell command execute error!!!\n") ; 
        exit(EXIT_FAILURE) ; 
    }
}

void redirect(char *command , char **parameters , int count , int Pipeflag , int inputFlag , int outFlag , int backflag) {
    if(backflag != -1) signal(SIGCHLD,SIG_IGN); // backgroud running 

    // 这块代码也不知道多 冗余了 ， 每个函数都有，我写的什么垃圾吖？
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
  
    *(argv + count + 1) = NULL ; 
    
    int PipeCount = 0 ; 
    for(i = 0 ; i <= count ; ++i) {
        if(strcmp(*(argv + i) , "||") == 0) ++PipeCount ; // Pipe nums ;
    }

    if(PipeCount == 0) { // 无管道 simple redirect 
         pid_t pid = fork() ; 
        if(pid > 0) {
            waitpid(pid , 0 , 0 ) ; 
        }else if(pid == 0){
            exec_redirect(argv , count) ; 
        }
    }else {

        int TotalPos = 0 ; 
        int pfd[PipeCount  + 1][2]  ; 
        for(i = 0 ; i <= PipeCount ; ++i) {
            if(pipe(pfd[i]) == -1) { perror("pipe error") ;  exit(EXIT_FAILURE) ;}
        }
        // 这个 for 循环的处理，我感觉还是挺不错的，有几个管道就 fork 出几 + 1个进程，但是将 标准输入和输出 是绑定在不同的地方的，有的是管道，有的是文件。
        // 但是 是不是应该考虑和前面的管道代码联合起来呢？ 避免代码冗余？
        int pipeNum = 0 ;
        for(pipeNum = 0 ; pipeNum <= PipeCount ; ++pipeNum) {
          //  printf("TotalPos %d\n" , TotalPos) ; 
            pid_t pid = fork() ; 
            if(pid == 0) {
                int outputPos = 0 ; 
                for(i = TotalPos ; i <= count ; ++i) {
                    if(strcmp(*(argv + i) , "||") == 0) break; 
                    if(strcmp(*(argv + i) , ">") == 0) outputPos = i ; // Pipe nums ;
                }
                //printf("outputPos = %d \n" , outputPos) ; 
                if(outputPos != 0) {

                    char *filename = argv[outputPos + 1] ; 
                    int fdout = open(filename , O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                    if(fdout == -1) { perror("open") ; exit(1) ;}
                    if(dup2(fdout , STDOUT_FILENO) == -1) { perror("dup2") ; exit(1) ; }
                    if(close(fdout) == -1) { perror("close") ; exit(1) ;} 

                }else { // STDOUT binding to pipe
                    // don't need to read
                  //  printf("%d out  %d\n" , pipeNum , PipeCount) ; 
                    if(pipeNum != PipeCount) {
                        
                        if(close(pfd[pipeNum + 1][0]) == -1) { printf("close pipe[0] error\n") ; exit(EXIT_FAILURE) ; }
                        
                        if(pfd[pipeNum + 1][1] != STDOUT_FILENO){
                            if(dup2(pfd[pipeNum + 1][1] , STDOUT_FILENO) == -1) { printf("dup2 error\n") ;  exit(EXIT_FAILURE) ;   }
                        }
                        if(close(pfd[pipeNum + 1][1]) == -1) { printf("close pipe[1] error\n") ;  exit(EXIT_FAILURE) ;  }
                      
                    }
                }
                int inputPos = 0 ; 
                for(i = TotalPos ; i <= count ; ++i) {
                    if(strcmp(*(argv + i) , "||") == 0) break; 
                    if(strcmp(*(argv + i) , "<") == 0) inputPos = i ; // Pipe nums ;
                }
                if(inputPos != 0){ // STDIN binding to fdin

                    char *filename = argv[inputPos + 1] ; 
                    int fdin = open(filename , O_RDONLY) ;
                    if(fdin == -1) { perror("open file") ; exit(1) ; }
                    if(dup2(fdin , STDIN_FILENO) == -1) { perror("dup2") ; exit(1) ; }
                    if(close(fdin) == -1) { perror("close") ; exit(1) ;} 

                }else { // STDIN binding to pipeIn
                    // don't need to write
                    if(close(pfd[pipeNum][1]) == -1) { printf("close pipe[1] error\n") ; exit(EXIT_FAILURE) ; }
                    if(pfd[pipeNum][0] != STDIN_FILENO){
                        if(dup2(pfd[pipeNum][0] , STDIN_FILENO) == -1) { printf("dup2 error\n") ;  exit(EXIT_FAILURE) ;  }
                    }
                    if(close(pfd[pipeNum][0]) == -1) { printf("close pipe[1] error\n") ;  exit(EXIT_FAILURE) ;  }
                }

                // 垃圾 代码
                char **arg_ver = (char **) malloc(sizeof(char *) * (count + 2)) ;
                int j = 0 ; 
                for(i = TotalPos ; i <= count ; ++i , ++j) {
                    if(strcmp(*(argv + i) , "<") == 0 || strcmp(*(argv + i) , "||") == 0 || strcmp(*(argv + i) , ">") == 0) break ; 
                    *(arg_ver + j) = (char *) malloc(strlen(argv[i]) + 1) ;
                    strcpy(*(arg_ver + j) , argv[i]) ;
                    (*(arg_ver + j))[strlen(argv[i])] = '\0' ; 
                }   
                arg_ver[j] = NULL ;
               // printf("command %s %d %d\n" , *arg_ver , TotalPos , count) ; 
                if(execvp(*arg_ver , arg_ver) == -1) {
                    printf("%s : command not foud\n" , arg_ver[0]) ; 
                    printf("shell command execute error!!!\n") ; 
                    exit(EXIT_FAILURE) ; 
                }
                //printf("free\n") ;  
            }else {
                // 这个 close 很关键，父子进程会拷贝 文件描述符，故不止在子进程里需要关闭，在父进程也需要关闭。
                close(pfd[pipeNum][0]) ; 
                close(pfd[pipeNum][1]) ; 
                // 下面这个代码很垃圾，只因为我想同步父子进程中的 TotalPos 变量，其实很好优化的，只需要可以在 fork 之前就把 执行命令 提取出来，而且不用担心子进程内存泄漏。
                // 这代码写的真垃圾，怪需求太仓促了，不能怪我，有时间一定优化。
                int j = 0 ; 
                for(j = TotalPos ; j <= count ; ++j) {
                    if(strcmp(*(argv + j) , "<") == 0 || strcmp(*(argv + j) , "||") == 0 || strcmp(*(argv + j) , ">") == 0) break ; 
                }   
                if(j <= count && (strcmp(*(argv + j) , "<") == 0 || strcmp(*(argv + j) , ">") == 0))
                    TotalPos = j + 3;
                else 
                    TotalPos = j + 1 ; 
                
                waitpid(pid , 0 , 0 ) ; 
            }

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