#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#define BUFFER_SIZE 100


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


int main(void)
{
int qtd = 0, i;
int memId, semId;
char* p;
char c;
pid_t pid = 0; /* Id do processo */

/* Criando memoria compartilhada */

memId = shmget(IPC_PRIVATE, BUFFER_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
p = (char *)shmat(memId, 0,0);

/* Fim da criacao de memoria compartilhada */

/* Criando o semaforo */

semId = semget(9557,1,IPC_CREAT); /* Cria um novo semaforo */

if (semId == -1)
{
printf("Erro ao criar semaforo\n");
exit(1);
}

/* Fim da criacao do semaforo */

setSemValue(semId); /* Inicializa o valor do semaforo com 1 */

pid = fork();

if (pid < 0) /* Erro ao criar novo processo */
{
printf("Erro ao criar novo processo\n");
exit(1);
}
else if (pid == 0) /* Processo filho */
{
printf("Entrando no processo filho\n");
while (1)
{
semaforoP(semId); /* Entra na area critica */
printf("Executando o processo filho (Coletar mensagem)\n");
printf("Escreva a mensagem: \n");
scanf("%s", p);
printf("Fim da execucao do processo filho (incrementando o semaforo...)\n");
semaforoV(semId); /* Sai da area critica */
}
}
else /* Processo pai */
{
printf("Entrando no processo pai\n");
while (1)
{
sleep(1); /* Para evitar erro */
semaforoP(semId); /* Entra na area critica */
printf("Executando o processo pai (Printar mensagem)\n");
printf("%s\n",p);
printf("Fim da execucao do processo filho (incrementando o semaforo...)\n");
semaforoV(semId); /* Sai da area critica */
}
}
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
