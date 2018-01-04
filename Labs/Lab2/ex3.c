#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(void){
    
    
    char *argv[100],entry[100],*token;
    int status,i = 0,pidAtual;
    
    while(1){
        strcpy(entry, "");
        
        printf("Minha Shell >> ");
        scanf("%[^\n]%*c", entry);
        
        if (strcmp(entry,"exit") == 0 || strcmp(entry,"") == 0) {
            exit(1);
        }
        token = strtok(entry," \"");
        
        for(i=0;token!= NULL;i++) {
            argv[i] = token;
            token = strtok(NULL," \"");
        }
        
        argv[i] = NULL;
        
        switch(pidAtual = fork()) {
            case 0:
            execvp(argv[0], argv);
            default:
            waitpid(-1,&status,0);
            i = 0;
        }
        
    }
    return 0;
}
