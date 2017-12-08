
  #include "vm.h"

int shms[4];
#ifndef TP

  typedef struct tabelaPagina {
	   char r_ou_w;
	    int id_processo;
	     int acesso; //contador de acessos
  }TabelaPagina;

  #define TP

#endif
