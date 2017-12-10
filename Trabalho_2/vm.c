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

	printf("antes do semAtual %d e sem atual %d\n",numero_processo, semAtual);

	down(semAtual);
	

	if(tp[pagina].p == 0) {
		int rw;	
		
		if(modo_abertura == 'R') {
                	rw = 0;
        	} else if(modo_abertura == 'W') {
                	rw = 1;
        	}

                printf("entre o atual e o do signal %d\n",numero_processo);
	
		TabelaPagina accessedPage = { .r_ou_w = rw, .id_processo = numero_processo, .acesso = 0, .r = 1, .m = rw, .p = 0, .physicalAddress = -1 };
		pageFaults[numero_processo - 1]++;
		TabelaPagina tpFinal = { .r_ou_w = rw, .id_processo = numero_processo, .acesso = pagina, .r = 1, .m = rw, .p = 0, .physicalAddress = -1 };
		tp[pagina] = accessedPage;
		tp[65536] = tpFinal;
	        down(semSignalId);	
		printf("kill number: %d e process number: %d\n", kill(getppid(), SIGUSR1), numero_processo);
 	} else {
		up(semAtual);
		printf("physical address %x and offset %x \n",tp[pagina].physicalAddress, offset);
	
	}

	


}
