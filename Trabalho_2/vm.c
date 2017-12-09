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
	TabelaPagina accessedPage;

	tp = shmat(processSegment, NULL, 0);

	down(semId);

	accessedPage = tp[pagina];

	accessedPage.r_ou_w = modo_abertura;

	if(accessedPage.p == 0) {
		pageFaults[numero_processo]++;
		kill(getppid(), SIGUSR1);
 	} else {
		printf("physical address %d and offset %d \n",accesedPage.physicalAddress, offset);
	
	}

	accesedPage.

	up(semdId);



}
