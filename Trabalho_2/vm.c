#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "vm.h"
#include "gm.h"


void trans(int numero_processo,int pagina,int offset,char modo_abertura) {
	int processShm = shms[numero_processo];



}
