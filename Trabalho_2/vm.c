#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include "vm.h"
#include "gm.h"
#include "semaphore.h"
#include <sys/sem.h>


void trans(int numero_processo,int pagina,int offset,char modo_abertura) {
	TabelaPagina *tp;
	int processSegment = shms[numero_processo];
	TabelaPagina accesedPage;

	tp = shmat(processSegment, NULL, 0);

	down(semId);

	accesedPage = tp[pagina];

	if(accesedPage.p == 0) {
		pageFaults[numero_processo]++;
		kill(getppid(), SIGUSR1);
		
  }

	accesedPage.

	up(semdId);



}
