#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void){
  int P1,P2,P3,P4;

  P1 = fork();
  if(P1 != 0){
    P2 = fork();

    if(P2 != 0){
      P3 = fork();

      if(P3 != 0){
        P4 = fork();

        if(P4 != 0){
          //Processo Pai

        }else{
          //Processo Filho 4 (simulador.log)
          unsigned addr4, rw4;
          File *arq4 = fopen("simulador.log", "r");

          while( fscanf(arq4, "%x %c", &addr4, &rw4) == 2 ) {

          }
        }
      }else{
        //Processo Filho 3 (compressor.log)
        unsigned addr3, rw3;
        File *arq3 = fopen("compressor.log", "r");

        while( fscanf(arq3, "%x %c", &addr3, &rw3) == 2 ) {

        }
      }
    }else{
      //Processo Filho 2 (matrix.log)
      unsigned addr2, rw2;
      File *arq2 = fopen("matrix.log", "r");

	    while( fscanf(arq2, "%x %c", &addr2, &rw2) == 2 ) {

      }
    }
  }else{
    //Processo Filho 1 (compilador.log)
    unsigned addr1, rw1;
    File *arq1 = fopen("compilador.log", "r");

    while( fscanf(arq1, "%x %c", &addr1, &rw1) == 2 ) {

    }
  }

  return 0;
}
