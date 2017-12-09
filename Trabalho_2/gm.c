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


#define MAXTABELA 65536//NAO SEI O TAMANHO DA TABELA DE PAGINAS


int P1,P2,P3,P4;

int busca_menos_acessado(TabelaPagina *t){
	int i, menor;
	menor = 0;
	for(i=1;i < MAXTABELA; i++){
		if(t[menor].acesso > t[i].acesso){
			menor = i;
		}
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

	segmento = shmget (id, paginaTam *  sizeof(TabelaPagina), IPC_CREAT | S_IRUSR | S_IWUSR);

	vetorTabelaPaginas = shmat(segmento, NULL, 0);


	if(!vetorTabelaPaginas) {
		printf("Faltou memoria\n");
		exit(1);
	}

	/* initializing shared memomry segment*/
	for(int i=0;i < paginaTam; i++){
		vetorTabelaPaginas[i].acesso = 0;
	}

	return segmento;
}

unsigned pegaIndicePagina(unsigned addr){
	unsigned page = addr >> 16;
	return page;
}

void sigHandler(int signal, siginfo_t *siginfo, void *context) {

	//to know if it is SIGUSR1
	if(signal == 10) {
		if(P3 == siginfo->si_pid) {
			printf("ai caralha\n");
		}
		printf ("Sending PID: %d, UID: %d p1 pid: %d\n",
			siginfo->si_pid, siginfo->si_uid),P3;
	}


}


int main(void){
	int segmento;
	int memoria_fisica[256];

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

	memset(&act, '\0', sizeof(act));

	act.sa_sigaction = &sigHandler;
	
	/* this line needs to be before the if, otherwise the code won't work */
	act.sa_flags = SA_SIGINFO;
	
	if (sigaction(SIGUSR1, &act, NULL) == -1) {
        	perror("Error: cannot handle SIGUSR1"); // Should not happen
        }
	
	for(int i = 0; i < 256; i++) {
		memoria_fisica[i] = -1;
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
	while(1) {
		//printf("automaticamente\n");
		//sleep(1);
	  }

        }else{
          //Processo Filho 4 (simulador.log)
          unsigned addr4;
					char rw4;
          FILE *arq4 = fopen("simulador.log", "r");


          while( fscanf(arq4, "%x %c", &addr4, &rw4) == 2 ) {
						int indicePagina = pegaIndicePagina(addr4);

          }
        }
      }else{
        //Processo Filho 3 (compressor.log)
        unsigned addr3;
				char rw3;
        FILE *arq3 = fopen("compressor.log", "r");


	printf("to mandando a caceta\n");
	kill(getppid(), 10);


        while( fscanf(arq3, "%x %c", &addr3, &rw3) == 2 ) {
					int indicePagina = pegaIndicePagina(addr3);

        }
      }
    }else{
      //Processo Filho 2 (matriz.log)
      unsigned addr2;
			char rw2;
      FILE *arq2 = fopen("matriz.log", "r");


	    while( fscanf(arq2, "%x %c", &addr2, &rw2) == 2 ) {
				int indicePagina = pegaIndicePagina(addr2);

      }
    }
    /* N ESQUECE DE APAGAR ESSA PORRA!!! */
    //shmdt(shms[0]);
    //shmdt(shms[1]);
    //shmdt(shms[2]);
    //shmdt(shms[3]);

   }else{
    //Processo Filho 1 (compilador.log)
    unsigned addr1;
    char rw1;


    FILE *arq1 = fopen("compilador.log", "r");
    

    while( fscanf(arq1, "%x %c", &addr1, &rw1) == 2 ) {
			int indicePagina = pegaIndicePagina(addr1);

    }
  }


  return 0;
}
