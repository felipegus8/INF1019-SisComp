#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

void childFunction(int matriz1[3][3],int matriz2[3][3],int linha){
    int linharesult[3] = {0,0,0};
    for (int i = 0;i < 3;i++) {
     linharesult[i] = matriz1[linha][i] + matriz2[linha][i]; 
  }
  printf("Linha %d da Matriz:\t%d\t%d\t%d\n",linha,linharesult[0],linharesult[1],linharesult[2]);
}

int main(){
    int qtdFilhos = 3,status;
    int pidAtual, i;
    int matriz1[3][3] = {{3,4,5},{6,7,8},{9,10,11}};
    int matriz2[3][3] = {{1,2,3},{3,4,5},{6,7,8}};
    for(i=0; i<qtdFilhos; i++){
        switch(pidAtual = fork()){
            case 0:
                childFunction(matriz1,matriz2,i);
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
