#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void){
  char comando[21];
  char comandotemp[301];
  char **parms = NULL;
  int num_cmd = 0;
  int flagIni = 1;
  int flagProg = 1;

  while (flagIni){
    //Solicita o comando (sair, ler programas)
    printf("########################################################################\n");
    printf("Digite o nome do comando desejado: \n- ler_programas\n- sair\n");
    printf("########################################################################\n");
    scanf("%s", comando);
    setbuf(stdin, NULL);
    if (strcmp(comando, "sair") == 0){
      flagIni = 0;
    }else if (strcmp(comando, "ler_programas") == 0){
      while (flagProg) {
        // Pede os programas para ser executados
        printf("########################################################################\n");
        printf("- Digite o nome de programas executaveis: \n\texec <nome_programa> (<tempo em ms>,<tempo em ms>,...) \n- Para finalizar a lista de programas digite 'parar'\n");
        printf("########################################################################\n");
        scanf("%[^\n]s", comandotemp);
        setbuf(stdin, NULL);
        if(strcmp(comandotemp, "parar") == 0 ){
          flagIni = 0;
          flagProg = 0;
        }else{
          //Aloca e preenche o vetor de parâmetros com o comando para executar o programa
          parms[num_cmd] = (char*)malloc((strlen(comandotemp) - 4) * sizeof(char));
          if (parms[num_cmd] == NULL){
            printf("Erro ao alocar memoria\n\n");
            exit(1);
          }
          strcpy(parms[num_cmd], comandotemp);
          num_cmd++;
        }
      }
      execve ("escalonador", parms, 0);
    }
    else{
      printf("comando %s nao reconhecido.\n Comandos aceitos: \nler_programas \nsair \n\n", comando);
    }
  }
  return 0;
}
