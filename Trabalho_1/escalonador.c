#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "Fila.h"
#include <stdlib.h>
#include <stdio.h>


Fila * f1;
Fila *f2;
Fila *f3;
Processo *p;
void insereProcessosInicio();
void desceParaF2();
void desceParaF3();
void sobeParaF1();
void sobeParaF2();


int main (int argc,char *argv[]){
  int flag_rajada = 0;
  Processo* vet;

  //Crio um vetor de processos
  vet = (Processo*)malloc((argc) * sizeof(Processo));

  for (int i = 0; i < argc; i++){
    int done = 1;
    int tamanho_max = sizeof(argv[i]) - 8;
    int j = 0;

    vet[i].nome = (char*)malloc(tamanho_max * sizeof(char));
    vet[i].pid = 0;
    vet[i].estado_Atual = Não_Iniciado;
    for(int x = 5;done && x < sizeof(argv[i]); x++){
      char c = argv[i][x];

      if(c != ' '){

        vet[i].nome[j] = argv[i][x];
        j++;

      }else{
        vet[i].nome[j+1] = '\0';
        flag_rajada = 1;
      }

      if(flag_rajada){

        vet[i].rajadas_tempo[0] = argv[i][7+j] - '0';
        vet[i].rajadas_tempo[1] = argv[i][9+j] - '0';
        vet[i].rajadas_tempo[2] = argv[i][11+j] - '0';

        done = 0;
        flag_rajada = 0;

      }
    }
  }
}

void insereProcessosInicio() {
  for (int i = 0;i < 10;i++) {
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
  int i = 0;
  while(tamanhoFila(fila) != 0) {
    Processo processo_Atual = removeProcesso(fila);
    //CASO DE UM PROCESSO AINDA NÃO TER SIDO INICIALIZADO
    if (processo_Atual.estado_Atual == Não_Iniciado) {
        processo_Atual.pid = fork();
        if(processo_Atual.pid != 0) {
          //Processo Pai
          //Para pelo tempo do quantum
          sleep(quantumFila);
          int j = 0;
          for(j=0;processo_Atual.rajadas_tempo[j] <= 0;j++)
          if(j>=3) break;

          //Subtrai do array de rajadas_tempo o que já foi executado
          processo_Atual.rajadas_tempo[j] -= quantumFila;
          //Interrompe o processo
          printf("<< Interrompendo processo de nome: %s por tempo\n\n", processo_Atual.nome);
          kill(processo_Atual.pid,SIGSTOP);

          //Checa qual o estado_Atual do Processo
          for(j=0;processo_Atual.rajadas_tempo[j] <= 0;j++)
          if(j>=3) break;
          if(j<=3)
          processo_Atual.estado_Atual = Em_Espera;
          else
          processo_Atual.estado_Atual = Finalizado;
        }
        else {
          printf(">> Executando processo de nome: %s \n\n", processo_Atual.nome);
          execve(processo_Atual.nome, NULL, NULL);
        }
    }
    else {
        if (processo_Atual.estado_Atual != Finalizado) {
          //Continuando a execução de um processo
          printf(">> Executando processo de nome: %s \n\n", processo_Atual.nome);
          kill(processo_Atual.pid,SIGCONT);

          sleep(quantumFila);
          //Parando o processo
          printf("<< Interrompendo processo de nome: %s por tempo\n\n", processo_Atual.nome);
          kill(processo_Atual.pid,SIGSTOP);
          int j = 0;
          for(j=0;processo_Atual.rajadas_tempo[j] <= 0;j++)
          if(j>=3) break;

          //Subtrai do array de rajadas_tempo o que já foi executado
          processo_Atual.rajadas_tempo[j] -= quantumFila;

          if(processo_Atual.rajadas_tempo[j] == 0){
            printf("<< Finalizando processo de nome: %s \n\n", processo_Atual.nome);
            processo_Atual.estado_Atual = Finalizado;
          }
        }
    }
    /* TODO:Andar com o processo_Atual pelas 3 filas    */
    switch quantumFila:
        case 1:
        if (processo_Atual.estado_Atual != Finalizado) {
            desceParaF2();
        }
        case 2:
        if (processo_Atual.estado_Atual != Finalizado) {
            desceParaF3();
        }
        if (processo_Atual.estado_Atual == Finalizado) {
            sobeParaF1();
        }
        case 3:
        if (processo_Atual.estado_Atual == Finalizado) {
            sobeParaF1();
        }
        if (processo_Atual.estado_Atual != Finalizado) {
            sobeParaF2();
        }
    i++;
  }
}
