#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main (int argc, char *argv[]) {
  int i, j;
  printf("Chamou Processo\n");
  for(i=0; i < argc; i++) {
    for(j=0; j < atoi(argv[i]);j++) {
      printf("PID:%d \n", getpid());
      sleep(1);
    }
    int p_pid = getppid();
    printf("\nPID PAI:%d\n",p_pid);
    kill(p_pid,SIGUSR1);
    if (i - argc != 1) {
    sleep(3);
    printf("\nPassou o sinal\n");
    kill(p_pid,SIGUSR2);
  }

}


  return 0;
}
