#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


int main() {

	int entrada, saida;
	char string[100];

	if ((entrada = open("entrada.txt", O_RDWR | O_CREAT, 0666)) == -1) {
		return -1;
	}

	if ((saida = open("saida.txt", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
		return -1;
	}
	close(0);

    dup(entrada);

	scanf(" %[^\n]", string);
	close(0);

    dup2(saida,1);
	
	printf("%s \n", string);

	return 0;
}
