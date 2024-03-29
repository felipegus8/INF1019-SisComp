#include "escalonador.h"

typedef struct fila Fila;
//Cria uma fila
Fila* initFila(Fila *f);

//Inserir processo na fila
void insereProcesso(Fila *fila, Processo p);

//Remover processo da Fila
Processo removeProcesso(Fila *fila);

//Checa se a fila está cheia
int filaCheia(Fila *fila);


//Checa se a fila está vazia
int filaVazia(Fila *fila);

//Retorna o tamanho da Fila
int tamanhoFila(Fila *fila);
