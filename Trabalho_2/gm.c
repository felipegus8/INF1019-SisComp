#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <signal.h>
#include "semaphore.h"
#include "gm.h"
#include "vm.h"
#include <sys/sem.h>
#include <errno.h>
#include <time.h>

#define MAXTABELA 65536


int P1,P2,P3,P4;
int memoria_fisica[10];
int memoria_fisica_processo[10];

int currentPid = -1;


int busca_menos_acessado(){
	int i, menor;
	menor = 0;
	TabelaPagina *t;

	for(i=0;i < 10; /* troca para MAXTABELA dps */ i++){
		switch (memoria_fisica_processo[i]) {
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
		if((t[memoria_fisica[menor]].acesso > t[memoria_fisica[i]].acesso)){
			menor = i;
		}
		else if((t[memoria_fisica[menor]].acesso == t[memoria_fisica[i]].acesso)){
			if(t[memoria_fisica[menor]].m == 0 && t[memoria_fisica[i]].m == 1)
				menor = i;
			}


		shmdt(t);

	}



	return menor;
}

void initializePageFaultsArrays(int *pageFaults) {

	for(int i=0;i<4;i++) {
		pageFaults[i] = 0;
	}

}


int criaVetorTabelaPaginas(int paginaTam, int id) {
	TabelaPagina *vetorTabelaPaginas;
	int segmento;

	 /* creating and obtaining shared memory */

	segmento = shmget (id, (paginaTam + 1) *  sizeof(TabelaPagina), IPC_CREAT |  S_IRUSR | S_IWUSR);

	vetorTabelaPaginas = (TabelaPagina *) shmat(segmento, NULL, 0);


	if(!vetorTabelaPaginas) {
		printf("Faltou memoria\n");
		exit(1);
	}


	/* initializing shared memomry segment*/
	for(int i=0;i < paginaTam; i++){
		TabelaPagina tp = { .r_ou_w = -1, .id_processo = -1, .acesso = 0, .r = -1, .m = -1, .p = 0, .physicalAddress = -1 };
		vetorTabelaPaginas[i] = tp;
	}

	return segmento;
}

unsigned pegaIndicePagina(unsigned addr){
	unsigned page = addr >> 16;
	return page;
}

unsigned pegaOffset(unsigned addr) {
	unsigned offset = addr << 16;
	return offset;
}




int checkIfRamIsFull(int *availablePosition) {
	for(int i=0;i<256;i++) {
		if(memoria_fisica[i] == -1) {
			*availablePosition = i;
			return FALSE;
		}
	}
	return TRUE;
}

void allocatePage(int pid,int processNumber, int index) {
	TabelaPagina *tp,*tp2;
	int ramIsFull;
	int availablePosition;


	tp = (TabelaPagina *) shmat(shms[processNumber - 1], NULL, 0);


	ramIsFull = checkIfRamIsFull(&availablePosition);
	tp[index].p = 1;
  tp[index].r = 1;
  tp[index].acesso++;


	if(ramIsFull) {

		printf("encheu!!###################\n");
		int lostPageIndex = busca_menos_acessado();

		printf("lost page index: %d\n", lostPageIndex);

		if(memoria_fisica_processo[lostPageIndex] != processNumber) {
			tp2 = (TabelaPagina *) shmat(shms[memoria_fisica_processo[lostPageIndex] - 1],NULL,0);
			} else {
				printf("foi no mesmo\n");
				tp2 = (TabelaPagina *) shmat(shms[memoria_fisica_processo[lostPageIndex] - 1],NULL,0);
			}

		int processPageOwner = memoria_fisica_processo[lostPageIndex];

		printf("process page owner %d\n", processPageOwner);


		int processPid;
                if(processPageOwner == 1) {
                        processPid = P1;
                } else if(processPageOwner == 2) {
                        processPid = P2;
                } else if(processPageOwner == 3) {
                        processPid = P3;
                } else if(processPageOwner == 4) {
                        processPid = P4;
                }

		tp[index].physicalAddress = lostPageIndex;
		tp2[memoria_fisica[lostPageIndex]].p = 0;
    tp2[memoria_fisica[lostPageIndex]].physicalAddress = -1;

		memoria_fisica[tp[index].physicalAddress] = index;

		memoria_fisica_processo[tp[index].physicalAddress] = tp[index].id_processo;

	shmdt(tp2);
		kill(processPid, SIGUSR2);

	} else {

		/* putting page on physical memory  */

		memoria_fisica[availablePosition] = index;
		memoria_fisica_processo[availablePosition] = processNumber;

		/* finding process which has lost a page in physical memory */
		/* putting page on the virtual memory */

		tp[index].physicalAddress = availablePosition;

		tp2 = (TabelaPagina *) shmat(shms[processNumber - 1], NULL, 0);
		printf("PHYSICAL ADDRESS NA SHARED MEMORY INDEX:%d,%d\n",index,tp2[index].physicalAddress);

	}


	int currentTime = time(NULL);

	/* if page was modified sleep for 2 seconds if not sleep 1  */
	if(tp[index].m == 1) {
		while(1) {
			if((int)(time(NULL) - currentTime) >= 2) {
				kill(pid, SIGCONT);
				break;
			}

		}
	} else {
		while(1) {
			if((int)(time(NULL) - currentTime) >= 1) {
				kill(pid, SIGCONT);
				break;
			}
		}
	}

	shmdt(tp);
	int semAtual;
	switch(processNumber) {
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

	up(semAtual);

}

void notifyLoss(int signal) {
	printf("tomei um sigusr2 pq perdi a pagina\n");

}

void sigHandler(int signal, siginfo_t *siginfo, void *context) {

	TabelaPagina *tp;


	//to know if it is SIGUSR1
	if(signal == SIGUSR1) {

		kill(siginfo->si_pid, SIGSTOP);


		if(P1 == siginfo->si_pid) {
			tp = shmat(shms[0], NULL, 0);
			allocatePage(siginfo->si_pid,1, tp[MAXTABELA].acesso);
			shmdt(tp);
			up(semSignalId);
		} else if(P2 == siginfo->si_pid) {
      tp = shmat(shms[1], NULL, 0);
			allocatePage(siginfo->si_pid,2,tp[MAXTABELA].acesso);
      shmdt(tp);
			up(semSignalId);
		} else if(P3 == siginfo->si_pid) {
			tp = shmat(shms[2], NULL, 0);
			allocatePage(siginfo->si_pid,3, tp[MAXTABELA].acesso);
      shmdt(tp);
			up(semSignalId);
		} else if(P4 == siginfo->si_pid) {
      tp = shmat(shms[3], NULL, 0);
			allocatePage(siginfo->si_pid,4, tp[MAXTABELA].acesso);
      shmdt(tp);
			up(semSignalId);
		}

	}


}


int main(void){
	int segmento;

	struct sigaction act;

	/* precisa criar e inicializar antes dos forks se n dá blade*/
	int segmentoTabelaPaginas1 = criaVetorTabelaPaginas(MAXTABELA,55555);
	int segmentoTabelaPaginas2 = criaVetorTabelaPaginas(MAXTABELA,66666);
	int segmentoTabelaPaginas3 = criaVetorTabelaPaginas(MAXTABELA,77777);
	int segmentoTabelaPaginas4 = criaVetorTabelaPaginas(MAXTABELA,88888);

	initializePageFaultsArrays(pageFaults);

	shms[0] = segmentoTabelaPaginas1;
	shms[1] = segmentoTabelaPaginas2;
	shms[2] = segmentoTabelaPaginas3;
	shms[3] = segmentoTabelaPaginas4;

	semId = semget(10888, 1, 0666 | IPC_CREAT);
	setSemValue(semId);

	semId2 = semget(10889, 1, 0666 | IPC_CREAT);
  setSemValue(semId2);

	semId3 = semget(10890, 1, 0666 | IPC_CREAT);
  setSemValue(semId3);

	semId4 = semget(10891, 1, 0666 | IPC_CREAT);
  setSemValue(semId4);

	semSignalId = semget(10892, 1, 0666 | IPC_CREAT);
	setSemValue(semSignalId);

	memset(&act, '\0', sizeof(act));

	act.sa_sigaction = &sigHandler;

	/* this line needs to be before the if, otherwise the code won't work */
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &act, NULL) == -1) {
      perror("sigaction"); // Should not happen
    }


	signal(SIGUSR2, notifyLoss);

	for(int i = 0; i < 10; i++) {
		memoria_fisica[i] = -1;
		memoria_fisica_processo[i] = -1;
	}

  P1 = fork();
  if(P1 != 0){
    P2 = fork();

    if(P2 != 0){
      P3 = fork();

      if(P3 != 0){
        P4 = fork();

        if(P4 != 0){
          //Processo Pai
	  		while(1);
        }
				else{
          //Processo Filho 4 (simulador.log)
          unsigned addr4;
       	  char rw4;
          FILE *arq4 = fopen("simulador.log", "r");

      while( fscanf(arq4, "%x %c", &addr4, &rw4) == 2 ) {
		int indicePagina = pegaIndicePagina(addr4);
		int offsetePagina = addr4 & 0x0000FFFF;
		trans(4, indicePagina, offsetePagina, rw4);
	  }
	  TabelaPagina *tp = shmat(shms[3], NULL, 0);
          shmdt(tp);
        }
      }else{
        //Processo Filho 3 (compressor.log)
        unsigned addr3;
				char rw3;
        FILE *arq3 = fopen("compressor.log", "r");

    while( fscanf(arq3, "%x %c", &addr3, &rw3) == 2 ) {
		int indicePagina = pegaIndicePagina(addr3);
		int offsetePagina = addr3 & 0x0000FFFF;
		trans(3, indicePagina, offsetePagina, rw3);

        }

      	 TabelaPagina *tp = shmat(shms[2], NULL, 0);
    	shmdt(tp);
	}
    }else{
      //Processo Filho 2 (matriz.log)
      unsigned addr2;
			char rw2;
      FILE *arq2 = fopen("matriz.log", "r");

	    while( fscanf(arq2, "%x %c", &addr2, &rw2) == 2 ) {
		int indicePagina = pegaIndicePagina(addr2);
	  int offsetePagina = addr2 & 0x0000FFFF;
		trans(2, indicePagina, offsetePagina, rw2);
    }

    }
    TabelaPagina *tp = shmat(shms[1], NULL, 0);
    shmdt(tp);

   }else{
    //Processo Filho 1 (compilador.log)
    unsigned addr1;
    char rw1;


    FILE *arq1 = fopen("compilador.log", "r");


  while( fscanf(arq1, "%x %c", &addr1, &rw1) == 2 ) {
	int indicePagina = pegaIndicePagina(addr1);
	int offsetePagina = addr1 & 0x0000FFFF;
	trans(1, indicePagina, offsetePagina, rw1);
    }
    TabelaPagina *tp = shmat(shms[0], NULL, 0);
    shmdt(tp);
  }
  return 0;
}
