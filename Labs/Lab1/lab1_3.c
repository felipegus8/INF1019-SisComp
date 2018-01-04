#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<stdlib.h>

int main(void) {
     int status,status2;
     int pid = fork(),i = 0; 

     if(pid!=0) {
      printf("Processo Pai Inicializado\n");
      waitpid(-1,&status,0);
      for(i=0;i<100;i++) {
      printf("Pai - Numero:%d\n",i);
      sleep(1);
      }
      printf("Processo pai vai finalizar\n");
      
      printf("Processo pai finalizado\n");
 }
      else {
      int pid2 = fork(); 
      if (pid2 != 0) { //Filho
      printf("Filho foi criado\n");
      waitpid(-1,&status2,0);
      for(i=100;i<200;i++) {
      printf("Filho - Numero:%d\n",i);
      sleep(2);
}
      printf("Processo filho vai finalizar\n");
      
      printf("Processo filho finalizado\n");
     }
      else {  //Neto
      printf("Neto foi criado\n");
      for(i=200;i<300;i++) {
      printf("Neto - Numero:%d\n",i);
      sleep(3);
     }
     printf("Processo neto vai finalizar\n");
    }
}
}
