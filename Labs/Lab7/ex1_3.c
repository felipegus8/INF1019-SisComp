#include <stdio.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define OPENMODE (O_RDONLY)
#define FIFO "minhaFifo"
int main (void)
{
   int fpFIFO;
   char ch;
   if (access(FIFO, F_OK) == -1)
   {
                  if (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0)
                  {
                            fprintf (stderr, "Erro ao criar FIFO %s\n", FIFO);
                            return -1;
}
   }
   puts ("Abrindo FIFO");
   if ((fpFIFO = open (FIFO, OPENMODE)) < 0)
   {
                  fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
                  return -2;
   }
   puts ("Começando a ler...");
   while (read (fpFIFO, &ch, sizeof(ch)) > 0)
                  putchar (ch);
   puts ("Fim da leitura");
   close (fpFIFO);
return 0;
}
