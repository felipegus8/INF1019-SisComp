#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "Fila.h"
#include <time.h>
#include <string.h>
#define true 1
#define false 0

Fila *f1;
Fila *f2;
Fila *f3;
Fila *processosEmIO;
Processo* vet;
long current_time;
long timeAfter_IO_Sent;
int didCallSignal = false;
int didEndedProcess = false;
Processo current_Process;
int qtd_Processos;
//Protótipos das funções
void escalonaRoundRobin();
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
char** convert_int_to_string_array(int array[]);
int forkDeTodosOsProcessos(Processo p);

 int main (int argc,char *argv[]) {
   Processo* vet;
   signal(SIGUSR1, entrouNoIO);
   signal(SIGUSR2,terminouIO);
   signal(SIGCHLD,terminouExecucao);
   qtd_Processos = argc;
   f1 = initFila(f1);
   f2 = initFila(f2);
   f3 = initFila(f3);
   processosEmIO = initFila(processosEmIO);
  //Crio um vetor de processos
    vet = (Processo*)malloc(argc * sizeof(Processo));
    if(vet == NULL) {
          printf("falta de memoria\n");
          exit(1);
      }

    int i;


  for (i = 0; i < qtd_Processos; i++){
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

  for (i=0;i<qtd_Processos;i++) {
    vet[i].pid = forkDeTodosOsProcessos(vet[i]);
  }

  insereProcessosInicio(f1,vet);

  escalonaRoundRobin();
	return 0;

}

void insereProcessosInicio(Fila *f1, Processo* p) {
  int i;
  for (i = 0;i < qtd_Processos;i++) {
    insereProcesso(f1,p[i]);
  }
}

int forkDeTodosOsProcessos(Processo p) {
    int pid = fork();
    if (pid == 0) { //Filho
      execv(p.nome,convert_int_to_string_array(p.rajadas_tempo));
    }
    else {
      kill(pid,SIGSTOP);
      return pid;
  }
  return 0;
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

void escalonaRoundRobin() {
  int j = 0;
  int quantumFila;
  Processo processo_Atual;
  int estourouQuantumFila = 0;
  int execute = true;
  while(true) {
    if (tamanhoFila(f1) != 0) {
        execute = true;
        processo_Atual = removeProcesso(f1);
        quantumFila = 1;
    }
    else if (tamanhoFila(f2) != 0) {
      execute = true;
      processo_Atual = removeProcesso(f2);
      quantumFila = 2;
    }
    else if (tamanhoFila(f3) != 0) {
      execute = true;
      processo_Atual = removeProcesso(f3);
      quantumFila = 4;
    }
   else if (tamanhoFila(processosEmIO) != 0) {
      execute = false;
    }
    else {
        break;
    }

    current_Process = processo_Atual;
    didEndedProcess = false;
    if (execute) {
        if (processo_Atual.estado_Atual != Finalizado) {
          //Continuando a execução de um processo
          printf(">> Executando processo de nome: %s \n\n", processo_Atual.nome);
          kill(processo_Atual.pid,SIGCONT);
          j = checkWhereArrayReallyStarts(processo_Atual);
          infiniteLoopUntilSignalORQuantumEnd(quantumFila);
          if (!didEndedProcess) {
            printf("<< Interrompendo processo de nome: %s por tempo\n\n", processo_Atual.nome);
            if (processo_Atual.rajadas_tempo[j] - quantumFila <= 0) {
                //Subtrai do array de rajadas_tempo o que já foi executado
                processo_Atual.rajadas_tempo[j] -= quantumFila;
                j = checkWhereArrayReallyStarts(processo_Atual);
                  //Atributo na struct processo dizendo onde ele está no array de rajadas.A partir disso alterar o estado do processo para Espera_IO.
                processo_Atual.pos_rajada = j;
                processo_Atual.estado_Atual = Espera_IO;
                estourouQuantumFila = false;
                insereProcesso(processosEmIO,current_Process);
                //Esperar receber sinal SIGUSR1
                pause();
            }
            else {
                processo_Atual.rajadas_tempo[j] -= quantumFila;
                processo_Atual.estado_Atual = Em_Espera;
                j = checkWhereArrayReallyStarts(processo_Atual);
                  //Atributo na struct processo dizendo onde ele está no array de rajadas.A partir disso alterar o estado do processo para Espera_IO.
                processo_Atual.pos_rajada = j;
                estourouQuantumFila = true;
                kill(processo_Atual.pid,SIGSTOP);
            }
          }
          if (processo_Atual.estado_Atual != Espera_IO) {
          if(j< checkSizeOfArray(processo_Atual.rajadas_tempo)) {
            processo_Atual.estado_Atual = Em_Espera;
          }
          else  {
            printf("<< Finalizando processo de nome: %s \n\n", processo_Atual.nome);
            processo_Atual.estado_Atual = Finalizado;
          }
          current_Process = processo_Atual;
        }

    if (processo_Atual.estado_Atual != Espera_IO) {
    switch (quantumFila){
      case 1:
        if (processo_Atual.estado_Atual != Finalizado) {
          printf("Desceu processo para  fila 2\n");
          processo_Atual.pos_fila = 2;
          desceParaF2(processo_Atual);
          break;
        }
      case 2:
        if (estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
          processo_Atual.pos_fila = 3;
          printf("Desceu processo para  fila 3\n");
          desceParaF3(processo_Atual);
          break;
        }
      case 4:
        if (estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
          printf("Ficou na fila 3\n");
          processo_Atual.pos_fila = 3;
          insereProcesso(f3,processo_Atual);
          break;
        }
    }
  }
      printf("Finalizado\n");

  }
}
}
}

void infiniteLoopUntilSignalORQuantumEnd(int quantumFila) {
  current_time = time(NULL);
  while(1) {
    long newTime = time(NULL);
    if ((int)(newTime - current_time) >= quantumFila || didCallSignal) {
        break;
    }
  }
}

void entrouNoIO(int signum) {
    if (signum == SIGUSR1) {
    printf("ESPERA POR IO\n");
    didCallSignal = true;
    return;
  }
}


void terminouExecucao(int signum) {
  if (signum != 17 && signum != 19 && signum != 20) {
    didEndedProcess = true;
    kill(current_Process.pid,SIGKILL);
  }
}

void terminouIO(int signum) {
    if (signum == SIGUSR2) {
    Processo processoIOFinalizado = removeProcesso(processosEmIO);
    kill(processoIOFinalizado.pid,SIGSTOP);
    printf("Chegou em terminouIO\n");
    switch (processoIOFinalizado.pos_fila) {
      case 1:
      processoIOFinalizado.pos_fila = 1;
      insereProcesso(f1,processoIOFinalizado);
      break;
      case 2:
      processoIOFinalizado.pos_fila = 1;
      insereProcesso(f1,processoIOFinalizado);
      break;
      case 3:
      processoIOFinalizado.pos_fila = 2;
      insereProcesso(f2,processoIOFinalizado);
      break;
    }
  }

}

int checkSizeOfArray(int *array) {
  return sizeof(array)/sizeof(int);
}

int checkWhereArrayReallyStarts(Processo processo_Atual) {
  int j = 0;
  for(j=0;j < checkSizeOfArray(processo_Atual.rajadas_tempo);j++)
    if (processo_Atual.rajadas_tempo[j] > 0) break;
  return j;
}

char** convert_int_to_string_array(int array[]) {
  int count_array = checkSizeOfArray(array);
  char** string_array = malloc(count_array * sizeof(char*));
  int i = 0;

  for (i = 0; i < count_array; i++) {
    char c[sizeof(int)];

    snprintf(c, sizeof(int), "%d", array[i]);

    string_array[i] = malloc(sizeof(c));
    strcpy(string_array[i], c);
  }

  return string_array;
}
