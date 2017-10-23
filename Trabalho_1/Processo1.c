#include <stdio.h>

int main (int argc, char *argv[]) {
  int i, j;

  for(i=0; i < argc; i++) {
    for(j=0; j < atoi(argv[i]; j++) {
      printf("%d \n", getpid());
      sleep(1);
    }
    kill(getppid(),SIGUSR1);
    sleep(3);
    kill(getppid(),SIGUSR2);
  }


  return 0;
}
