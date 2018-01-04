#include <stdio.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main() {
    int pid1,pid2,i = 0,pidAux;
    if (( pid1 = fork()) == 0) {
        //Processo filho 1
        while(1) {
            printf("Filho 1\n");
        }
    }
    else {
        if((pid2 = fork()) == 0) {
            //Processo filho 2
            while(1) {
                printf("Filho 2\n");
            }
        }
        else {
            //Processo Pai
            for(i=0;i<10;i++) {
                kill(pid1,SIGSTOP);
                kill(pid2, SIGCONT);
                sleep(1);
                pidAux = pid1;
                pid1 = pid2;
                pid2 = pidAux;
            }
            kill(pid1, SIGKILL);
            kill(pid2, SIGKILL);
            exit(1);
        }
    }
    return 0;
}
