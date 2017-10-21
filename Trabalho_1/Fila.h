#include "escalonador.h"

typedef struct fila Fila;
//Cria uma fila
static void initFila(Fila *f);

//Inserir processo na fila
static void insereProcesso(Fila *fila, Processo p);

//Remover processo da Fila
static Processo removeProcesso(Fila *fila);

//Checa se a fila está cheia
static int filaCheia(Fila *fila);


//Checa se a fila está vazia
static int filaVazia(Fila *fila);

