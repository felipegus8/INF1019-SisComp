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
time_t current_time;
int didCallSignal = false;
int didEndedProcess = false;
Processo current_Process;
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

int main(void){
// int main (int argc,char *argv[]) {
  Processo* vet;
  signal(SIGUSR1, entrouNoIO);
  signal(SIGUSR2,terminouIO);
  signal(SIGCHLD,terminouExecucao);
  f1 = initFila(f1);
  f2 = initFila(f2);
  f3 = initFila(f3);
  processosEmIO = initFila(processosEmIO);
  //Crio um vetor de processos
  vet = (Processo*)malloc((5) * sizeof(Processo));
  if(vet == NULL){
    printf("falta de memoria\n");
    exit(1);
  }

  int i;

/*
  for (i = 0; i < argc; i++){
    int done = 1;
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

  */
  for(i = 0 ;i<5;i++) {
    vet[i].pid = 0;
    vet[i].estado_Atual = Nao_Iniciado;
    vet[i].nome = (char*)malloc(20 * sizeof(char));
    vet[i].rajadas_tempo = (int*)malloc(5 * sizeof(int));
    vet[i].rajadas_tempo[0] = 3;
    vet[i].rajadas_tempo[1] = 4;
    vet[i].rajadas_tempo[2] = 5;
    vet[i].pos_fila=1;
    vet[i].pos_rajada=0;
  }
  strcpy(vet[0].nome,"processo1");
  strcpy(vet[1].nome,"processo2");
  strcpy(vet[2].nome,"processo3");
  strcpy(vet[3].nome,"processo4");
  strcpy(vet[4].nome,"processo5");

  for (i=0;i<5;i++) {
    vet[i].pid = forkDeTodosOsProcessos(vet[i]);
  }

  insereProcessosInicio(f1,vet);

  escalonaRoundRobin();
	return 0;

}

void insereProcessosInicio(Fila *f1, Processo* p) {
  int i;
  for (i = 0;i < 5;i++) {
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
  printf("%d",tamanhoFila(f2));
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
  int i = 0, j = 0;
  int quantumFila;
  Processo processo_Atual;
  int didStartExecuting = false;
  int estourouQuantumFila = 0;
  while(true) {
    if (tamanhoFila(f1) != 0) {
        processo_Atual = removeProcesso(f1);
        quantumFila = 1;
    }
    else if (tamanhoFila(f2) != 0) {
      processo_Atual = removeProcesso(f2);
      quantumFila = 2;
    }
    else if (tamanhoFila(f3) != 0) {
      processo_Atual = removeProcesso(f3);
      quantumFila = 4;
    }
    else {
      break;
    }
    current_Process = processo_Atual;
    didEndedProcess = false;
        if (processo_Atual.estado_Atual != Finalizado) {
          //Continuando a execução de um processo
         printf(">> Executando processo de nome: %s \n\n", processo_Atual.nome);
          current_time = time(NULL);
          kill(processo_Atual.pid,SIGCONT);
          infiniteLoopUntilSignalORQuantumEnd(quantumFila);
        //  sleep(quantumFila);
          if (!didEndedProcess) {
            printf("<< Interrompendo processo de nome: %s por tempo\n\n", processo_Atual.nome);
          //  printf("%d\n",processo_Atual.pid);
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
      //    printf("Rajada atual antes:%d\n",processo_Atual.rajadas_tempo[j]);
          processo_Atual.rajadas_tempo[j] -= quantumFila;
        //  printf("Rajada atual depois:%d\n",processo_Atual.rajadas_tempo[j]);
          //Checa qual o estado_Atual do Processo.Tem que fazer o loop de novo porque é depois da subtração.
          j = checkWhereArrayReallyStarts(processo_Atual);
      //    printf("J:%d\n",j);
          //TODO:Atributo na struct processo dizendo onde ele está no array de rajadas.A partir disso alterar o estado do processo para Espera_IO.
          processo_Atual.pos_rajada = j;
          //TODO:Guardar a fila onde o processo estava antes de ser mandado para I/O.
          if(j<= checkSizeOfArray(processo_Atual.rajadas_tempo)) {
        //    printf("Entrou em espera\n");
            processo_Atual.estado_Atual = Em_Espera;
          }
          else  {
            printf("<< Finalizando processo de nome: %s \n\n", processo_Atual.nome);
            processo_Atual.estado_Atual = Finalizado;
          }
          current_Process = processo_Atual;
        }
//        }
    printf("POS NA FILA PORCESSO: %d\n", processo_Atual.pos_fila);
    //Andar com o processo_Atual pelas 3 filas
    switch (quantumFila){
      case 1:
        printf("Enum:%d\n",processo_Atual.estado_Atual);
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
        else if (!estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
          printf("Subiu processo para  fila 1\n");
          processo_Atual.pos_fila = 1;
          sobeParaF1(processo_Atual);
          break;
        }
      case 3:
        if (estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
          printf("Ficou na fila 3\n");
          processo_Atual.pos_fila = 3;
          insereProcesso(f3,processo_Atual);
          break;
        }
        if (!estourouQuantumFila && processo_Atual.estado_Atual != Finalizado) {
          printf("Subiu processo para  fila 2\n");
          processo_Atual.pos_fila = 2;
          sobeParaF2(processo_Atual);
          break;
        }
    }
  printf("Chegou aqui");

}
}

void infiniteLoopUntilSignalORQuantumEnd(int quantumFila) {
  while(1) {
    int newTime = time(NULL);
    if (didCallSignal == true)
    break;
    if ((int)(newTime - current_time) >= quantumFila) {
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
  if (signum != 17 && signum != 19 && signum != 20) {
    printf("%d\n", signum);
    didEndedProcess = true;
    kill(current_Process.pid,SIGKILL);
  }
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
