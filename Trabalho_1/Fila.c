#include <stdlib.h>
#include "Fila.h"

#define MAXFILA 100

//TAD - Fila Circular
typedef struct fila {
    Processo f[100];
    int ini;
    int fim;
    int count;
} Fila;

Fila* initFila(Fila *f) {
  f = (Fila*)malloc(sizeof(Fila));
  if(f == NULL){
    printf("falta de memoria\n");
    exit(1);
  }
  f->ini = 0;
  f->fim = 0;
  f->count = 0;
  return f;
}

void insereProcesso(Fila *fila, Processo p) {
    (fila->count)++;
    fila->f[fila->fim] = p;
    fila->fim = (fila->fim+1)%MAXFILA;
}

Processo removeProcesso(Fila *fila) {
    Processo valor;
    valor = fila->f[fila->ini];
    fila->ini = (fila->ini+1)%MAXFILA;
    (fila->count)--;

    return valor;
}

int filaCheia(Fila *fila) {
    return (fila->count == 200);
}

int filaVazia(Fila *fila) {
    return (fila->count == 0);
}

int tamanhoFila(Fila *fila) {
  return fila->count;
}
