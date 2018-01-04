#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main () {

	int segmento1, segmento2, segmento3, segmento1_aux, segmento2_aux, segmento3_aux, id_filho;
	int **matriz1, **matriz2, **matriz3;
	int i, j;

 //Attach da matriz em si.
	segmento1 = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	matriz1 = (int **) shmat (segmento1, 0, 0);

 //Attach de cada posição da matriz.
	for(i=0;i<9;i++) {
		segmento1_aux = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		matriz1[i] = (int *) shmat (segmento1_aux, 0, 0);
	}

   //Attach da matriz em si.
	segmento2 = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	matriz2 = (int **) shmat (segmento2, 0, 0);

  //Attach de cada posição da matriz.
	for(i=0;i<9;i++) {
		segmento2_aux = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		matriz2[i] = (int *) shmat (segmento2_aux, 0, 0);
	}
  //Attach da matriz em si.
	segmento3 = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	matriz3 = (int **) shmat (segmento3, 0, 0);

  //Attach de cada posição da matriz.
	for(i=0;i<9;i++) {
		segmento3_aux = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		matriz3[i] = (int *) shmat (segmento3_aux, 0, 0);
	}

  //Preenchendo a primeira matriz com o valor 8 em cada posição.
	for(i=0;i<3;i++) {
		for(j=0; j<3; j++) {
			matriz1[i][j] = 8;
		}
	}
 //Preenchendo a segunda matriz com o valor 88 em cada posição.
	for(i=0;i<3;i++) {
		for(j=0; j<3; j++) {
			matriz2[i][j] = 88;
		}
	}

 //Fazendo um fork para cada linha da matriz e preenchendo a matriz resultado dentro de cada linha.
	for(i=0;i<3;i++) {
		if ((id_filho = fork()) < 0) {
			puts("Erro na criação do novo processo");
			exit(1);
		}
		else if (id_filho==0) {
			for(j=0;j<3;j++) {
				matriz3[i][j] = matriz1[i][j] + matriz2[i][j];
			}
			shmdt(matriz1);
			shmdt(matriz2);
			shmdt(matriz3);
			exit(1);
		}
		else {
			wait(0);
		}
	}

  //Print na matriz.
	for(i=0;i<3;i++) {
		for(j=0;j<3;j++) {
			printf("%d\t", matriz3[i][j]);
		}
		printf("\n");
	}

  //Detach de cada posição de cada matriz.
	for(i=0;i<3;i++) {
		shmdt(matriz1[i]);
		shmdt(matriz2[i]);
		shmdt(matriz3[i]);
	}
	//Detach dos segmentos auxiliares.
	shmctl(segmento1_aux, IPC_RMID, 0);
	shmctl(segmento2_aux, IPC_RMID, 0);
	shmctl(segmento3_aux, IPC_RMID, 0);

//Detach da matriz em si
	shmdt(matriz1);
	shmdt(matriz2);
	shmdt(matriz3);
	shmctl(segmento1, IPC_RMID, 0);
	shmctl(segmento2, IPC_RMID, 0);
	shmctl(segmento2, IPC_RMID, 0);
	return 0;
}
