#include<stdio.h>

typedef enum Estados {
   Não_Iniciado,
   Finalizado,
   Em_Espera
} Estados;

typedef struct processo  {
  char *nome;
  int pid;
  int rajadas_tempo[3];
  enum Estados estado_Atual;
} Processo;
