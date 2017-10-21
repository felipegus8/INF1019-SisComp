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

void desceParaF2() {
  Processo p1 = removeProcesso(f1);
  insereProcesso(f2,p1);
}

void desceParaF3() {
  Processo p2 = removeProcesso(f2);
  insereProcesso(f3,p2);
}
void sobeParaF1() {
  Processo p2 = removeProcesso(f2);
  insereProcesso(f1,p2);
}
void sobeParaF2() {
  Processo p3 = removeProcesso(f3);
  insereProcesso(f2,p3);
}
