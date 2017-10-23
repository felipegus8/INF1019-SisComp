#include<stdio.h>

typedef enum {
   Nao_Iniciado = 1,
   Finalizado = 2,
   Em_Espera = 3,
   Espera_IO = 4
} Estados;

typedef struct processo  {
  char *nome;
  int pid;
  int rajadas_tempo[20];
  Estados estado_Atual;
} Processo;
