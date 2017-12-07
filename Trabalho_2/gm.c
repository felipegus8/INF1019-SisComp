#include "gm.h"
#define MAXTABELA 65536//NAO SEI O TAMANHO DA TABELA DE PAGINAS

typedef struct tabelaPagina {
	char r_ou_w;
	int id_processo;
	int acesso; //contador de acessos
} TabelaPagina;

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

	segmento = shmget (id, paginaTam*sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	vetorTabelaPaginas = (TabelaPagina *) shmat (segmento, 0, 0);
	if(!vetorTabelaPaginas) {
		printf("Faltou memoria\n");
		exit(1);
	}

	for(i=0;i < MAXTABELA; i++){
		t[i].acesso = 0;
	}

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
					TabelaPagina *vetorTabelaPaginas4 = criaVetorTabelaPaginas(MAXTABELA,8081);

          while( fscanf(arq4, "%x %c", &addr4, &rw4) == 2 ) {
						int indicePagina = pegaIndicePagina(addr4);

          }
        }
      }else{
        //Processo Filho 3 (compressor.log)
        unsigned addr3;
				char rw3;
        File *arq3 = fopen("compressor.log", "r");
				TabelaPagina *vetorTabelaPaginas3 = criaVetorTabelaPaginas(MAXTABELA,5081);

        while( fscanf(arq3, "%x %c", &addr3, &rw3) == 2 ) {
					int indicePagina = pegaIndicePagina(addr3);

        }
      }
    }else{
      //Processo Filho 2 (matriz.log)
      unsigned addr2;
			char rw2;
      File *arq2 = fopen("matriz.log", "r");
			TabelaPagina *vetorTabelaPaginas2 = criaVetorTabelaPaginas(MAXTABELA,7861);

	    while( fscanf(arq2, "%x %c", &addr2, &rw2) == 2 ) {
				int indicePagina = pegaIndicePagina(addr2);

      }
    }
  }else{
    //Processo Filho 1 (compilador.log)
    unsigned addr1;
		char rw1;
    File *arq1 = fopen("compilador.log", "r");
		TabelaPagina *vetorTabelaPaginas1 = criaVetorTabelaPaginas(MAXTABELA,1234);

    while( fscanf(arq1, "%x %c", &addr1, &rw1) == 2 ) {
			int indicePagina = pegaIndicePagina(addr1);

    }
  }

  return 0;
}
