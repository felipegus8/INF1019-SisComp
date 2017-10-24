#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "Fila.h"
#include <time.h>
#define true 1
#define false 0

Fila *f1;
Fila *f2;
Fila *f3;
Fila *processosEmIO;

time_t current_time;
int didCallSignal = false;
int didEndedProcess = false;
Processo current_Process;
//Protótipos das funções
void escalonaRoundRobin(Fila *fila,int quantumFila);
void insereProcessosInicio(Fila *p1, Processo* p);
void desceParaF2(Processo p1);
void desceParaF3(Processo p2);
void sobeParaF1(Processo p2);
void sobeParaF2(Processo p3);
void terminouExecucao(int signum);
void entrouNoIO(int signum);
void infiniteLoopUntilSignalORQuantumEnd(int quantumFila);
int checkSizeOfArray(int *array);
int checkWhereArrayReallyStarts(Processo processo_Atual);
void terminouIO(int signum);

int main (int argc,char *argv[]) {
  Processo* vet;
  signal(SIGUSR1, entrouNoIO);
  signal(SIGUSR2,terminouIO);
  signal(SIGCHLD,terminouExecucao);
  f1 = initFila(f1);
  f2 = initFila(f2);
  f3 = initFila(f3);
  processosEmIO = initFila(processosEmIO);

  //Crio um vetor de processos
  vet = (Processo*)malloc((argc) * sizeof(Processo));
  if(vet == NULL){
    printf("falta de memoria\n");
    exit(1);
  }

  int i;
  for (i = 0; i < argc; i++){
    int tamanho_max = sizeof(argv[i]) - 8;
    int j = 0;
    vet[i].nome = (char*)malloc(tamanho_max * sizeof(char));
		if(vet[i].nome == NULL){
			printf("falta de memoria\n");
			exit(1);
		}

    vet[i].pid = 0;
    vet[i].estado_Atual = Nao_Iniciado;

    //Preenche o array nome
    int x = 5;
    char c = argv[i][x];

    while(c != ' '){
      vet[i].nome[j] = argv[i][x];
      x++;
      j++;
      c = argv[i][x];
    }

    vet[i].nome[j] = '\0';

    //Preenche o array rajadas_tempo
		int tot = 0;
		int count = x+2;

		//define o tamanho do array
    c = argv[i][count];
    while(c){
			tot++;
      count+=2;
      c = argv[i][count];
		}

		vet[i].rajadas_tempo = (int*)malloc(tot * sizeof(int));
		if(vet[i].rajadas_tempo == NULL){
			printf("falta de memoria\n");
			exit(1);
		}

		//define os valores no array
		count = x+2;
    int pos;
		for(pos = 0; pos < tot ;pos++){
			vet[i].rajadas_tempo[pos] = argv[i][count] - '0';
			count+=2;
		}
  }

  insereProcessosInicio(f1,vet);
  escalonaRoundRobin(f1,1);

	return 0;

}

void insereProcessosInicio(Fila *p1, Processo* p) {
  int i;
  for (i = 0;i < 10;i++) {
    insereProcesso(f1,p[i]);
  }
}

void desceParaF2(Processo p1) {
  insereProcesso(f2,p1);
}

void desceParaF3(Processo p2) {
  insereProcesso(f3,p2);
}
void sobeParaF1(Processo p2) {
  insereProcesso(f1,p2);
}
void sobeParaF2(Processo p3) {
  insereProcesso(f2,p3);
}

void escalonaRoundRobin(Fila *fila,int quantumFila) {
  int i = 0, j = 0;
  int estourouQuantumFila = 0;
  while(tamanhoFila(fila) != 0) {
    if (quantumFila != 1 && tamanhoFila(f1) != 0) {
      escalonaRoundRobin(f1,1);
    }
    Processo processo_Atual = removeProcesso(fila);
    current_Process = processo_Atual;
    didEndedProcess = false;
    //CASO DE UM PROCESSO AINDA NÃO TER SIDO INICIALIZADO
    if (processo_Atual.estado_Atual == Nao_Iniciado) {
        processo_Atual.pid = fork();
        if(processo_Atual.pid != 0) {
            infiniteLoopUntilSignalORQuantumEnd(quantumFila);
            //Interrompe o processo
            if (!didEndedProcess) {
            printf("<< Interrompendo processo de nome: %s por tempo\n\n", processo_Atual.nome);
            kill(processo_Atual.pid,SIGSTOP);
          }
            j = checkWhereArrayReallyStarts(processo_Atual);
            //Subtrai do array de rajadas_tempo o que já foi executado
            if (processo_Atual.rajadas_tempo[j] - quantumFila > 0) {
              estourouQuantumFila = true;
            }
            else {
              estourouQuantumFila = false;
            }
            processo_Atual.rajadas_tempo[j] -= quantumFila;
            //Checa qual o estado_Atual do Processo.Tem que fazer o loop de novo porque é depois da subtração.
            j = checkWhereArrayReallyStarts(processo_Atual);
            if(j<= checkSizeOfArray(processo_Atual.rajadas_tempo))
            processo_Atual.estado_Atual = Em_Espera;
            else
            printf("<< Finalizando processo de nome: %s \n\n", processo_Atual.nome);
            processo_Atual.estado_Atual = Finalizado;
            current_Process = processo_Atual;
        }
        else {
          printf(">> Executando processo de nome: %s \n\n", processo_Atual.nome);
          current_time = time(NULL);
          execve(processo_Atual.nome, NULL, NULL);
        }
    }
    else {
        if (processo_Atual.estado_Atual != Finalizado) {
          //Continuando a execução de um processo
          printf(">> Executando processo de nome: %s \n\n", processo_Atual.nome);
          kill(processo_Atual.pid,SIGCONT);
          infiniteLoopUntilSignalORQuantumEnd(quantumFila);
          if (!didEndedProcess) {
          printf("<< Interrompendo processo de nome: %s por tempo\n\n", processo_Atual.nome);
          kill(processo_Atual.pid,SIGSTOP);
        }
          j = checkWhereArrayReallyStarts(processo_Atual);
          //Subtrai do array de rajadas_tempo o que já foi executado
          if (processo_Atual.rajadas_tempo[j] - quantumFila >= 0) {
            estourouQuantumFila = true;
          }
          else {
            estourouQuantumFila = false;
          }
          processo_Atual.rajadas_tempo[j] -= quantumFila;
          //Checa qual o estado_Atual do Processo.Tem que fazer o loop de novo porque é depois da subtração.
          j = checkWhereArrayReallyStarts(processo_Atual);
          if(j<= checkSizeOfArray(processo_Atual.rajadas_tempo))
          processo_Atual.estado_Atual = Em_Espera;
          else
          printf("<< Finalizando processo de nome: %s \n\n", processo_Atual.nome);
          processo_Atual.estado_Atual = Finalizado;
          current_Process = processo_Atual;
          }
        }
    //Andar com o processo_Atual pelas 3 filas
    switch (quantumFila){
      case 1:
        if (processo_Atual.estado_Atual != Finalizado) {
          desceParaF2(processo_Atual);
        }
      case 2:
        if (estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
            desceParaF3(processo_Atual);
        }
        else if (!estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
            sobeParaF1(processo_Atual);
            //Chamada recursiva de escalonaRoundRobin
            escalonaRoundRobin(f1,1);
        }
      case 3:
        if (estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
          insereProcesso(f3,processo_Atual);
        }
        if (!estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
          sobeParaF2(processo_Atual);
          escalonaRoundRobin(f2,2);
        }
    }
    i++;
  }
}

void infiniteLoopUntilSignalORQuantumEnd(int quantumFila) {
  while(1) {
    int newTime = time(NULL);
    if (didCallSignal == true)
    break;
    if ((int)(newTime - current_time) <= quantumFila) {
        break;
    }
  }
}

void entrouNoIO(int signum) {
    insereProcesso(processosEmIO,current_Process);
    didCallSignal = true;
    //I/O

}
void terminouExecucao(int signum) {
    didEndedProcess = true;
    kill(current_Process.pid,SIGKILL);
}

void terminouIO(int signum) {
    Processo processoIOFinalizado = removeProcesso(processosEmIO);
    insereProcesso(f1,processoIOFinalizado);
}

int checkSizeOfArray(int *array) {
  return sizeof(array)/sizeof(int);
}

int checkWhereArrayReallyStarts(Processo processo_Atual) {
  int j = 0;
  for(j=0;j < checkSizeOfArray(processo_Atual.rajadas_tempo);j++)
  if (processo_Atual.rajadas_tempo <= 0) break;
  return j;
}
