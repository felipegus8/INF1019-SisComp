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

	int processSegment = shms[numero_processo - 1];
	tp = shmat(processSegment, NULL, 0);

	int semAtual;

	switch(numero_processo) {
        case 1:
                semAtual = semId;
                break;
        case 2:
                semAtual = semId2;
                break;
        case 3:
                semAtual = semId3;
                break;
        case 4:
                semAtual = semId4;
                break;
        default:
                break;

        }


	down(semAtual);


	if(tp[pagina].p == 0) {
		int rw;

		if(modo_abertura == 'R') {
                	rw = 0;
        	} else if(modo_abertura == 'W') {
                	rw = 1;
        	}


		TabelaPagina accessedPage = { .r_ou_w = rw, .id_processo = numero_processo, .acesso = 0, .r = 1, .m = rw, .p = 0, .physicalAddress = -1 };
		pageFaults[numero_processo - 1]++;
		TabelaPagina tpFinal = { .r_ou_w = rw, .id_processo = numero_processo, .acesso = pagina, .r = 1, .m = rw, .p = 0, .physicalAddress = -1 };
		tp[pagina] = accessedPage;
	 	tp[65536] = tpFinal;
	  down(semSignalId);
		printf("kill number: %d e process number: %d\n", kill(getppid(), SIGUSR1), numero_processo);
		usleep(10000);
 	} else {
		up(semAtual);
		printf("Process %d, physical address %x ,%c , and offset %x \n",numero_processo,tp[pagina].physicalAddress,modo_abertura,offset);

	}
}
