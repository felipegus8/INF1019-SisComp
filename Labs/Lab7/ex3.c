#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define FIFO "minhaFifo"

int main() {
	int fifo = 0, fifo2 = 0,pid1,pid2;
	char mensagem[45];

	if (access (FIFO, F_OK) == -1) {
		if (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0) {
			fprintf (stderr, "Erro ao criar FIFO %s\n", FIFO);
			return -1;
		}
	}

	if ((pid1 = fork()) == 0) {
    //Filho 1
		if ((fifo = open (FIFO, O_WRONLY)) < 0) {
			fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
			return -2;
		}
		strcpy(mensagem, "Filho 1 escreveu na FIFO ");
		write (fifo, mensagem, strlen (mensagem));

		return 0;
	}
	else {
		 if ((pid2 = fork()) == 0) {
       //Filho 2
			if ((fifo = open (FIFO, O_WRONLY)) < 0) {
				fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
				return -2;
			}
			strcpy(mensagem, "Filho 2 escreveu na FIFO ");
			write (fifo, mensagem, strlen (mensagem));

			return 0;
		}
		// Pai
		else {
			if ((fifo2 = open (FIFO, O_RDONLY)) < 0) {
				fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
				return -2;
			}
			int status;
			waitpid (pid1, &status, 0);
			waitpid (pid2, &status, 0);
			while (read (fifo2, mensagem, 100) > 0) {
				printf ("%s\n", mensagem);
			}
		}
	}


	close (fifo);

	return 0;
}
