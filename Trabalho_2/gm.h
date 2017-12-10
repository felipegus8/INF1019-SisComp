

int shms[4];
int semId;
int semId2;
int semId3;
int semId4;
int semSignalId;
int pageFaults[4];

#define TRUE 1
#define FALSE 0

#ifndef TP

  typedef struct tabelaPagina { 
	char r_ou_w;
	int id_processo;
	int acesso; //contador de acesso
        char r; //reference byte
        char m; //modified byte
        char p; //see if page is present
	int physicalAddress;

  }TabelaPagina;

  #define TP

#endif
