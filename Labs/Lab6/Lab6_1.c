
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
  int fd[2],dadosescritos,dadoslidos;
  pipe(fd);
  const char textoDoPipe[] = "Testando Pipe";
  char textoLido[sizeof textoDoPipe];
  if (fork() == 0) {
    //filho
    dadosescritos = write(fd[1],textoDoPipe,strlen(textoDoPipe) + 1);
      printf("%d dados escritos:%s\n",dadosescritos,textoDoPipe);
  }
  else {
    dadoslidos = read(fd[0],textoLido,sizeof textoLido);
    printf("%d dados lidos: %s\n",dadoslidos,textoLido);
  }
  close(fd[0]);
  close (fd[1]);

  return 0;
}
