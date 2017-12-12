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
  contador = (int*) shmat(segmentoContador,NULL,0);
	(*contador) ++;
	printf("Contador:%d\n",(*contador));
	if((*contador) > 0 && (*contador) % 32 == 0) {
		TabelaPagina *t;
		int i;
		vetAux = (int *) shmat (segmentoAux,NULL,0);
		vetposaux = (int *) shmat(segmento2Aux,NULL,0);
		for(i=0;i<10/*Pode trocar o valor aqui*/;i++) {
			switch(vetposaux[i]) {
				case 1:
				t = (TabelaPagina *)shmat(shms[0], NULL, 0);
				break;
				case 2:
				t = (TabelaPagina *)shmat(shms[1], NULL, 0);
				break;
				case 3:
				t = (TabelaPagina *)shmat(shms[2], NULL, 0);
				break;
				case 4:
				t = (TabelaPagina *)shmat(shms[3], NULL, 0);
				break;
				default:
				printf("deu ruim\n");
				break;
			}
			t[vetAux[i]].acesso = 0;
			shmdt(t);
		}
		printf("Atualizou todos os acessos como sendo 0\n");
		qtdPageFaults = (int *) shmat(segmentoQtdPageFaults,NULL,0);
		printf("QTD DE PAGE FAULTS:%d\n";qtdPageFaults);
		qtdEscrita = (int *) shmat(segmentoQTDEscrita,NULL,0);
		printf("QTD DE SWAPS:%d\n",qtdEscrita);
		shmdt(vetAux);
		shmdt(vetposaux);
	}
	//printf("Vai atualizar processSegment\n");
	int processSegment = shms[numero_processo - 1];
	tp = shmat(processSegment, NULL, 0);

	int semAtual;
	//printf("Passou\n");
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

		qtdPageFaults = (int *) shmat(segmentoQtdPageFaults,NULL,0);
		(*qtdPageFaults)++;
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
