#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

void imprimirMatriz(int * mat);

int main () {
    int i, j, segmento1, segmento2, id, status, val,segmento1_aux,segmento2_aux;
    int  **mat,**mat2;

    // Aloca a memória compartilhada
    segmento1 = shmget(IPC_PRIVATE, 16 * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);
    mat = (int **) shmat (segmento1, 0, 0);
    
    //Attach de cada posição da matriz.
    for(i=0;i<16;i++) {
        segmento1_aux = shmget (IPC_PRIVATE, sizeof(int) * 16, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        mat[i] = (int *) shmat (segmento1_aux, 0, 0);
    }
    segmento2 = shmget(IPC_PRIVATE, 16 * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);
    mat2 = (int **) shmat (segmento2, 0, 0);

    for(i=0;i<9;i++) {
        segmento2_aux = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        mat2[i] = (int *) shmat (segmento2_aux, 0, 0);
    }
    
    printf("Entre com os numeros da matriz:\n");
    for (i = 0; i < 4; i++) {
      printf("Linha %d: ", i + 1);
      for (j=0;j<4;j++) {
        scanf(" %d", &val);
        mat[i][j] = val;
      }
    }

    if ((id = fork()) < 0) { // Processo pai
            puts ("Erro na criação do novo processo");
            exit (-2);
    }
    else if (id == 0) { // Processo filho 1
            for (i = 0; i < 4; i++) {
                mat2[i][0] = mat[0][i];
            }

            id = fork();

            if (id == 0) { // Processo filho 2
                for (i = 0; i < 4; i++) {
                  mat2[i][1] = mat[1][i];
                }

                id = fork();

                if (id == 0) { // Processo filho 3
                    for (i = 0; i < 4; i++) {
                    mat2[i][2] = mat[2][i];
                    }

                    id = fork();

                    if (id == 0) { // Processo filho 4
                        for (i = 0; i < 4; i++) {
                            mat2[i][3] = mat[3][i];
                        }
                    }
                }
            }
    }
    else { // Processo pai
        sleep(1);
        waitpid(-1, &status, 0);
        printf ("\nMatriz:\n");
        for(i=0;i<4;i++) {
          printf("%d\t%d\t%d\t%d\n",mat[i][0],mat[i][1],mat[i][2],mat[i][3]);
        }
        printf ("Transposta:\n");
        for(i=0;i<4;i++) {
          printf("%d\t%d\t%d\t%d\n",mat2[i][0],mat2[i][1],mat2[i][2],mat2[i][3]);
        }
    }

    for(i=0;i<4;i++) {
        shmdt(mat[i]);
        shmdt(mat2[i]);
    }
    shmctl(segmento1_aux, IPC_RMID, 0);
    shmctl(segmento2_aux, IPC_RMID, 0);
    
    // Libera a memória compartilhada das matrizes
    shmdt (mat);
    shmdt (mat2);

    // Libera a memória compartilhada
    shmctl (segmento1, IPC_RMID, 0);
    shmctl (segmento2, IPC_RMID, 0);

    return 0;
}
