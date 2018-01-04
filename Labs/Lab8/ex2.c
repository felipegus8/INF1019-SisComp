#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#define BUFFER_SIZE 16

union  semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);



int main (int argc, char * argv[]) {

	int segment,pid;
	char *buffer;
	int idSemaforo;

	segment = shmget (IPC_PRIVATE, sizeof(char)*BUFFER_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	buffer = (char*) shmat (segment, 0, 0);

	if ((pid = fork()) != 0) {
		//Produtor
		idSemaforo = semget (8760, 1, 0666 | IPC_CREAT);
		setSemValue(idSemaforo);
		while (1) {
			int i = 0;
			char c;
			semaforoP(idSemaforo);
			printf("Produtor Executando area Critica\n");

			while ( i < BUFFER_SIZE) {
        printf("Digite um caracter: ");
				buffer[i] = getchar();
				while ( (c =  getchar()) != '\n' && c != EOF);
				i++;
			}
			printf("Buffer foi lido\n");
			semaforoV(idSemaforo);
			sleep(2);
		}

	}
	else {
		//Consumidor
		while ((idSemaforo = semget (8760, 1, 0666)) < 0) sleep (1);
		while(1) {
			int i=0;
			semaforoP(idSemaforo);
			printf("Consumidor Executando\n");
			while ( i < BUFFER_SIZE) {
				printf("%c", buffer[i]);
				i++;
			}
			printf("\n");
			printf("Buffer foi escrito\n");
			semaforoV(idSemaforo);
			sleep(2);
		}
	}
	shmdt (buffer);
	shmctl (segment, IPC_RMID, 0);



	return 0;
}

int setSemValue(int semId) {
	union semun semUnion;
	semUnion.val = 1;
	return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId) {
	union semun semUnion;
	semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId) {
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = -1;
	semB.sem_flg = SEM_UNDO;
	semop(semId, &semB, 1);
	return 0;
}

int semaforoV(int semId) {
	struct sembuf semB;

	semB.sem_num = 0;
	semB.sem_op = 1;
	semB.sem_flg = SEM_UNDO;
	semop(semId, &semB, 1);

	return 0;
}
