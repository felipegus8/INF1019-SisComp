#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<stdlib.h>

int main(void) {
     int status;
     int teste = 0,i=0;
     int pid = fork();
     if(pid!=0) {
     printf("Processo Pai Inicializado\n");
     printf("My process ID : %d\n", getpid());
     printf("My child process ID is:%d\n",pid);
     for (i=0;i<100;i++) {
     teste += 5;
     printf("Pai:%d - %d\n",getpid(),teste);
     }
     waitpid(-1,&status,0);
     printf("Processo pai finalizado\n");
     exit(1);
}
     else {
      printf("Processo filho iniciado\n");
      printf("My process ID : %d\n", getpid());
      printf("My parent process ID is : %d\n", getppid());
       for (i=0;i<100;i++) {
       teste += 10;
       printf("Filho:%d - %d\n",getpid(),teste);
     }
      printf("Processo filho finalizado\n");
     }
}
