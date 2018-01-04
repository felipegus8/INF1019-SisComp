#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    
    pid_t pid1, pid2, pid3;
    
    if((pid1 = fork()) == 0) {
        //Filho 1
       execv("processo1", NULL);
    }
    else {
        if((pid2=fork()) == 0) {
            //Filho 2
           execv("processo2", NULL);
        }
        else {
            if((pid3 = fork()) == 0) {
                //Filho 3
                 execv("processo3", NULL);
            }
            else {
                kill(pid1, SIGSTOP);
                kill(pid2, SIGSTOP);
                kill(pid3, SIGSTOP);
                
                while(1) {
                    kill(pid1, SIGCONT);
                    sleep(1);
                    kill(pid1, SIGSTOP);
                    kill(pid2, SIGCONT);
                    sleep(2);
                    kill(pid2, SIGSTOP);
                    kill(pid3, SIGCONT);
                    sleep(2);
                    kill(pid3, SIGSTOP);
                }
                
            }
        }
    }
    
    return 0;
}

