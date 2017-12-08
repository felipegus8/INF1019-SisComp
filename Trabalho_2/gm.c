#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include "gm.h"
#include "vm.h"



#define MAXTABELA 65536//NAO SEI O TAMANHO DA TABELA DE PAGINAS



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

TabelaPagina *criaVetorTabelaPaginas(int paginaTam, int id) {
	TabelaPagina *vetorTabelaPaginas;
	int segmento;

	 /* creating and obtaining shared memory */

	printf("fucking id %d\n", id);

	segmento = shmget (id, paginaTam *  sizeof(TabelaPagina), IPC_CREAT | S_IRUSR | S_IWUSR);

	printf("mitico segmento: %d\n",segmento);

	printf("vetor table paginas %d\n",vetorTabelaPaginas);

	vetorTabelaPaginas = shmat(segmento, NULL, 0);

	
	if(!vetorTabelaPaginas) {
		printf("Faltou memoria\n");
		exit(1);
	}

	/* initializing shared memomry segment*/
	printf("olha eu aqui de novo %d\n", vetorTabelaPaginas[0]);
	for(int i=0;i < paginaTam; i++){
		vetorTabelaPaginas[i].acesso = 0;
	}
	printf("xaxando\n");

	return vetorTabelaPaginas;
}

unsigned pegaIndicePagina(unsigned addr){
	unsigned page = addr >> 16;
	return page;
}

int main(void){
  int P1,P2,P3,P4;
	int segmento;
	int memoria_fisica[256];
	/* precisa criar e inicializar antes dos forks se n dá blade*/
	TabelaPagina *vetorTabelaPaginas1 = criaVetorTabelaPaginas(MAXTABELA,55555);
	TabelaPagina *vetorTabelaPaginas2 = criaVetorTabelaPaginas(MAXTABELA,66666);
	TabelaPagina *vetorTabelaPaginas3 = criaVetorTabelaPaginas(MAXTABELA,77777);
	TabelaPagina *vetorTabelaPaginas4 = criaVetorTabelaPaginas(MAXTABELA,88888);

	shms[0] = vetorTabelaPaginas1;
	shms[1] = vetorTabelaPaginas2;
	shms[2] = vetorTabelaPaginas3;
	shms[3] = vetorTabelaPaginas4;


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
    shmdt(shms[0]);
    shmdt(shms[1]);
    shmdt(shms[2]);
    shmdt(shms[3]);
   
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
