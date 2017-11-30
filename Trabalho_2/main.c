#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned pegaIndicePagina(unsigned addr){
	unsigned page = addr >> 16;
	return page;
}

int main(void){
  int P1,P2,P3,P4;

	int *memoria_fisica = (int *)malloc(sizeof(int) * 256);

	if(!memoria_fisica) {
		printf("Erro na alocacao da memoria fisica.\n");
		exit(1);
	}

	for(i = 0; i < 256; i++) {
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
          File *arq4 = fopen("simulador.log", "r");

          while( fscanf(arq4, "%x %c", &addr4, &rw4) == 2 ) {
						int indicePagina = pegaIndicePagina(addr4);

          }
        }
      }else{
        //Processo Filho 3 (compressor.log)
        unsigned addr3;
				char rw3;
        File *arq3 = fopen("compressor.log", "r");

        while( fscanf(arq3, "%x %c", &addr3, &rw3) == 2 ) {
					int indicePagina = pegaIndicePagina(addr3);

        }
      }
    }else{
      //Processo Filho 2 (matrix.log)
      unsigned addr2;
			char rw2;
      File *arq2 = fopen("matrix.log", "r");

	    while( fscanf(arq2, "%x %c", &addr2, &rw2) == 2 ) {
				int indicePagina = pegaIndicePagina(addr2);

      }
    }
  }else{
    //Processo Filho 1 (compilador.log)
    unsigned addr1;
		char rw1;
    File *arq1 = fopen("compilador.log", "r");

    while( fscanf(arq1, "%x %c", &addr1, &rw1) == 2 ) {
			int indicePagina = pegaIndicePagina(addr1);

    }
  }

  return 0;
}
