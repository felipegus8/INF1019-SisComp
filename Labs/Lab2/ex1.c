#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

void childFunction(int vetor[],int inicial,int final,int valor){
    int found = 0;
     printf("Filho : %d\n", getpid());
     for(int i = inicial;i < final;i++) {
     printf("Inicial:%d,Final:%d\n",inicial,final);
     if (vetor[i] == valor) {
      found = 1;
      printf("Encontrou na posicao %d do vetor\n",i);
    }
  }
      if (found != 1) {
      printf("Não encontrou ocorrência do valor\n");
   }
}

int main(){
    int qtdFilhos = 4,status;
    int pidAtual, i;
    int vetor[10] = {1,2,3,4,5,6,7,8};
    for(i=0; i<qtdFilhos; i++){
        switch(pidAtual = fork()){
            case 0:
                childFunction(vetor,2*i,(2*i) + 2,4);
                return 0;
            case -1:
            break;
            default:
                waitpid(-1,&status,0);
                break;
        }

    }
    wait(NULL);
    printf("Pai tem %d filhos\n", i);

    return 0;
}
