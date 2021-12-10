#ifndef MYSHELL_H
#define MYSHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>

#define MAXLINE 1024        // 命令的最大长度
#define MAXPARAMTERS 10     // 参数的最大个数
#define max_name_len  256
#define max_path_len  1024
//#define DEBUG

void type_prompt() ; 
int read_command(char * , char **) ; 
void builtin_command(char * , char ** , int) ; 
void External_command(char * , char ** , int) ; 
void commandInbackgroud(char * , char ** , int) ; 
void Pipe_command(char * , char ** , int ,int ) ; 
void redirect(char * , char ** , int , int ,int ,int ,int) ; 
#endif
