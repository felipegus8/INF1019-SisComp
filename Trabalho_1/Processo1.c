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

  for(i=0; i < argc; i++) {
    for(j=0; j < atoi(argv[i]);j++) {
      printf("QTD ARGV:%d\n",atoi(argv[i]));
      printf("%d \n", getpid());
      sleep(1);
    }
    //Isso é pra ele não fazer I/O na última vez
    sleep(3);
    kill(getppid(),SIGUSR2);
  }


  return 0;
}
