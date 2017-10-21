#include "Fila.h"
#include <stdlib.h>

#define MAXFILA 200

//TAD - Fila Circular
typedef struct fila {
    Processo f[200];
    int ini;
    int fim;
    int count;
} Fila;

void initFila(Fila *f) {
    f->ini = 0;
    f->fim = 0;
    f->count = 0;
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
