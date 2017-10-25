#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
void acabouIO(int sigNum);

int main () {
    signal(SIGUSR2,acabouIO);
    sleep(3);
    printf("Acabou I/O\n");
    raise(SIGUSR2);
    return 0;
}

void acabouIO(int sigNum) {
  printf("Acabou essa merda\n");
}
