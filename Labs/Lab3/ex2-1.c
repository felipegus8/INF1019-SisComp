#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

	char *msgDoDia;
	int segmento;

	segmento = shmget (8180, sizeof(char) * 18, S_IRUSR | S_IWUSR);
	if(segmento == -1) {
		printf("erro");
		exit(1);
	}
	msgDoDia = (char *) shmat (segmento, 0, 0);

	printf("A mensagem do dia é: %s \n", msgDoDia);

	shmdt(msgDoDia);
	shmctl(segmento, IPC_RMID, 0);
	return 0;
}
