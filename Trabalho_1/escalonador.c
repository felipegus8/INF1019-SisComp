#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "Fila.h"
#include <stdlib.h>
#include <stdio.h>
#include "escalonador.h"


int main (int argc,char *argv[]){

  for (int i = 0; i < argc; i++){

	    printf("%s\n",argv[i]);

  }

  return 0;
}
