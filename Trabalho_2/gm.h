

int shms[4];
int semId;
int pageFaults[4];

#define TRUE 1
#define FALSE 0

#ifndef TP

  typedef struct tabelaPagina {
	   char r_ou_w;
	   int id_processo;
	   int acesso; //contador de acessos
     char r; //reference byte
     char m; //modified byte
     char p; //see if page is present
  }TabelaPagina;

  #define TP

#endif
