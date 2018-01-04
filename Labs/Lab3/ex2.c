#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

	int segmento;
	char *msgDoDia;

	segmento = shmget (8180, sizeof(char) * 120, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if(segmento == -1) {
		printf("erro");
		exit(1);
	}

	msgDoDia = (char *) shmat (segmento, 0, 0);
	printf("Digite a mensagem do dia:\n");
  scanf("%[^\n]", msgDoDia);


	printf("pai: %s \n", msgDoDia);
	shmdt(msgDoDia);
	return 0;
}
