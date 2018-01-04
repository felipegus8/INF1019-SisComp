#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void inicioHandler(int signal);
void terminoHandler(int signal);

time_t inicio = 0, final = 0;

int main(int argc, char *argv[]) {

    signal(SIGUSR1, inicioHandler);
    signal(SIGUSR2, terminoHandler);
    while(1) {
        //Loop infinito para esperar pelos sinais
    }
    return 0;
}

void inicioHandler(int signal) {
    inicio = time(NULL);
}

void terminoHandler(int signal) {
    long int custoLig;
    final = time(NULL);
    long int tempoChamada = final - inicio;
    printf("Tempo da ligação: %ld segundos \n", tempoChamada);
    
    if(tempoChamada <= 60) {
        custoLig = tempoChamada*2;
    }
    else {
        custoLig = 60*2 + (tempoChamada - 60);
    }
    
    printf("Custo R$: %ld,%.2ld\n", custoLig/100, custoLig%100);
    
    exit(1);
}

