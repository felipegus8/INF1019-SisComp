#include<stdio.h>

typedef enum {
   Nao_Iniciado = 1,
   Finalizado = 2,
   Em_Espera = 3
} Estados;

typedef struct processo  {
  char *nome;
  int pid;
  int rajadas_tempo[3];
  Estados estado_Atual;
} Processo;
