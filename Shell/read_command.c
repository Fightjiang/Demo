#include "myShell.h"

int read_command(char *command , char **parameters) {
    char* buffer = (char *) malloc(MAXLINE) ; 
    memset(buffer , 0 , sizeof(buffer)) ; 
    fflush(stdin) ;  fgets(buffer , MAXLINE , stdin) ; 
    if(buffer[0] == '\0') return -1 ; 
    
    char *pStart = buffer , *pEnd = buffer ; 
    int count = 0 , i = 0 ; 
    while(*pEnd != '\0' && *pEnd != '\n') {
        //处理空格
        while(*pStart == ' ' && *pEnd == ' ') {
            ++pStart ; ++pEnd ;
        }
        // 如果是换行符，即结束了
        if(*pEnd == '\0' || *pEnd == '\n'){
            if(count == 0) return -1 ; 
            break ;
        }
        
        // 处理命令
        while(*pEnd != '\0' && *pEnd != '\n' && *pEnd != ' ') {
            ++pEnd ; 
        }
        if(count == 0) {
            i = 0 ; 
            while(pStart < pEnd) {
                *(command + i++) = *pStart++ ; 
            }
            *(command + i) = '\0' ;  ++count ; 
        }else if(count <= MAXPARAMTERS) {
            i = 0 ; 
            *(parameters + count - 1) = (char*) malloc(pEnd - pStart + 1) ; 
            while(pStart < pEnd) {
                parameters[count - 1][i++] = *pStart++ ; 
            }
            parameters[count - 1][i] = '\0' ;  ++count ;
        }else {
            break; 
        }
    }
    free(buffer) ;  
    return count ; 
}

#ifdef DEBUG
int main() {
    
    char *command = (char *) malloc(MAXLINE) , **parameters = (char **) malloc(MAXPARAMTERS) ; 
    
    int count = read_command(command , parameters) ;
    
    printf("input analysis : \n") ; 
    printf("command: %s\nparameters:\n" , command) ;
    int i ;
    for(i = 0 ; i < count - 1;  ++i) {
        printf("%s \n" , parameters[i]) ; 
        free(*(parameters + i)) ; 
    } 
    free(command) ; 

    free(parameters) ;
    return 0 ; 
}
#endif 
