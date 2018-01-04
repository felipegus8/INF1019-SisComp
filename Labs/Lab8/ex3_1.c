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

	int segment,semId;
	int *sharedValue;

	segment = shmget (IPC_PRIVATE, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	sharedValue = (int*) shmat (segment, 0, 0);
	*sharedValue = 0;

	semId = semget (8765	, 1, 0666 | IPC_CREAT);
	setSemValue(semId);
	while (1) {
		semaforoP(semId);
		*sharedValue = (*sharedValue) + 1;
		printf("Após soma: %d\n",*sharedValue);
		sleep(rand()%3);
		semaforoV(semId);
		sleep(rand()%2);
	}
	shmdt (sharedValue);
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
