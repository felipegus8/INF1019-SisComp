#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int signal);

int main() {
    
    int num1,num2;
    int soma, subtracao, multiplicacao, divisao;
    
    signal(SIGFPE, handler);
    
    printf("Digite dois numeros \n");
    scanf("%d %d", &num1, &num2);
    
    soma = num1 + num2;
    subtracao = num1 - num2;
    multiplicacao = num1 * num2;
    divisao = num1 / num2;
    
    printf("Soma:%d \n", soma);
    printf("Subtracao:%d \n", subtracao);
    printf("Multiplicacao:%d \n", multiplicacao);
    printf("Divisao:%d \n", divisao);
    
    return 0;
}

void handler(int signal) {
    printf("Erro:Divisão por 0!\n");
    exit(-1);
}
