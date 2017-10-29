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
int didRemoveFromIO = false;
//Protótipos das funções
void escalonaRoundRobin();
void insereProcessosInicio(Fila *p1, Processo* p);
void desceParaF2(Processo p1);
void desceParaF3(Processo p2);
void sobeParaF1(Processo p2);
void sobeParaF2(Processo p3);
void terminouExecucao(int signum);
void entrouNoIO(int signum);
int infiniteLoopUntilSignalORQuantumEnd(int quantumFila);
int checkSizeOfArray(int *array);
int checkWhereArrayReallyStarts(Processo processo_Atual);
void terminouIO(int signum);
char** convert_int_to_string_array(Processo processo_Atual);
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
    vet[i].qtd_Rajadas = tot;
    if(vet[i].rajadas_tempo == NULL){
      printf("falta de memoria\n");
      exit(1);
    }

    //define os valores no array
    count = x+2;
    int pos;
    for(pos = 0; pos < tot ;pos++){
      vet[i].rajadas_tempo[pos] = argv[i][count] - '0';
      // printf("Rajada %d:%d",i,vet[i].rajadas_tempo[pos]);
      count+=2;
    }
  }
  for (i=0;i<qtd_Processos;i++) {
    vet[i].pid = forkDeTodosOsProcessos(vet[i]);
  }

//  while (kill(vet[0].pid,0) != 0);

  insereProcessosInicio(f1,vet);

  escalonaRoundRobin();
  return 0;

}

void insereProcessosInicio(Fila *f1, Processo* p) {
  int i;
  for (i = 0;i < qtd_Processos;i++) {
    if(p[i].pid != 0) {
      insereProcesso(f1,p[i]);
    }
  }
}

int forkDeTodosOsProcessos(Processo p) {
  char aux[50];
  char nomeCerto[50];
  strcpy(aux,p.nome);
  strcpy(nomeCerto,"./");
  strcat(nomeCerto,aux);
  int count_array = p.qtd_Rajadas;
  char** array = (char**)malloc(count_array * sizeof(char*));
  array = convert_int_to_string_array(p);
  int pid = fork();
  if (pid == 0) { //Filho
    execvp(nomeCerto,array);
    perror("execvp");
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
      printf("Está na fila 1\n");
      processo_Atual = removeProcesso(f1);
      if (processo_Atual.pid == 0) {
        break;
      }
      quantumFila = 1;
    }
    else if (tamanhoFila(f2) != 0) {
      execute = true;
      didRemoveFromIO = false;
      printf("Está na fila 2\n");
      processo_Atual = removeProcesso(f2);
      if (processo_Atual.pid == 0) {
        break;
      }
      quantumFila = 2;
    }
    else if (tamanhoFila(f3) != 0) {
      execute = true;
      didRemoveFromIO = false;
      printf("Está na fila 3\n");
      processo_Atual = removeProcesso(f3);
      if (processo_Atual.pid == 0) {
        break;
      }
      quantumFila = 4;
    }
    else if (tamanhoFila(processosEmIO) != 0) {
    //  printf("Espera na Fila de I/O\n");
      didRemoveFromIO = false;
      execute = false;
    }
    else {
      if (!didRemoveFromIO) {
            printf("Nenhum processo em nenhuma fila\n");
            break;
      }
    }

    current_Process = processo_Atual;
    didEndedProcess = false;
    if (execute) {
      if (processo_Atual.estado_Atual != Finalizado) {
        //Continuando a execução de um processo
        printf(">> Executando processo de nome: %s \n\n", processo_Atual.nome);
        kill(processo_Atual.pid,SIGCONT);
        j = checkWhereArrayReallyStarts(processo_Atual);
        int ret = infiniteLoopUntilSignalORQuantumEnd(quantumFila);
        printf("\nTime in Loop:%d\n",ret);
      //  if (!didEndedProcess) {
          printf("<< Interrompendo processo de nome: %s por tempo\n\n", processo_Atual.nome);
          if (processo_Atual.rajadas_tempo[j] - quantumFila <= 0 && j < processo_Atual.qtd_Rajadas - 1) {
            //Subtrai do array de rajadas_tempo o que já foi executado
            processo_Atual.rajadas_tempo[j] -= quantumFila;
            j = checkWhereArrayReallyStarts(processo_Atual);
            //Atributo na struct processo dizendo onde ele está no array de rajadas.A partir disso alterar o estado do processo para Espera_IO.
            processo_Atual.pos_rajada = j;
            processo_Atual.estado_Atual = Espera_IO;
            estourouQuantumFila = false;
            insereProcesso(processosEmIO,current_Process);
            printf("Inseriu na fila de I/O\n");
            //Esperar receber sinal SIGUSR1
            pause();
          }
          else {
          //  printf("Caiu aqui\n");
            processo_Atual.rajadas_tempo[j] -= quantumFila;
            processo_Atual.estado_Atual = Em_Espera;
            j = checkWhereArrayReallyStarts(processo_Atual);
            //Atributo na struct processo dizendo onde ele está no array de rajadas.A partir disso alterar o estado do processo para Espera_IO.
            processo_Atual.pos_rajada = j;
            estourouQuantumFila = true;
            printf("\nVai dar SIGSTOP no processo: %s\n",processo_Atual.nome);
            kill(processo_Atual.pid,SIGSTOP);
          }
      //  }
        if (processo_Atual.estado_Atual != Espera_IO) {
          if(j< processo_Atual.qtd_Rajadas) {
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
              printf("\nDesceu processo para  fila 2\n");
              processo_Atual.pos_fila = 2;
              desceParaF2(processo_Atual);
              break;
            }
            case 2:
            if (estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
              processo_Atual.pos_fila = 3;
              printf("\nDesceu processo para  fila 3\n");
              desceParaF3(processo_Atual);
              break;
            }
            case 4:
            if (estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
              printf("\nFicou na fila 3\n");
              processo_Atual.pos_fila = 3;
              insereProcesso(f3,processo_Atual);
              break;
            }
          }
        }


      }
    }
  }
      printf("Finalizado\n");
}

int infiniteLoopUntilSignalORQuantumEnd(int quantumFila) {
//  printf("\nComecou loop infinito\n");
//  printf("\nQuantum Fila:%d\n",quantumFila);
  current_time = time(NULL);
  while(1) {
    long newTime = time(NULL);
    if ((int)(newTime - current_time) >= quantumFila || didCallSignal) {
      didCallSignal = false;
      return (int)(newTime - current_time);
    }
  }
//  printf("\n Terminou loop infinito\n");
  return 0;
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
    didRemoveFromIO = true;
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

int checkWhereArrayReallyStarts(Processo processo_Atual) {
  int j = 0;
  for(j=0;j < processo_Atual.qtd_Rajadas;j++)
  if (processo_Atual.rajadas_tempo[j] > 0) break;
  return j;
}

char** convert_int_to_string_array(Processo processo_Atual) {
  int count_array = processo_Atual.qtd_Rajadas;
  char** string_array = (char**)malloc(count_array * sizeof(char*));
  int i = 0;

  for (i = 0; i < count_array; i++) {
    char c[sizeof(int)];

    snprintf(c, sizeof(int), "%d", processo_Atual.rajadas_tempo[i]);

    string_array[i] = (char*)malloc(sizeof(c));
    strcpy(string_array[i], c);
  }
  string_array[i] = NULL;

  return string_array;
}
