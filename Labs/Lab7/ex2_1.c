#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#define FIFO "minhaFifo"

int main() {
	int fifo;
	char ch;

	if (access (FIFO, F_OK) == -1) {
		if (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0) {
			fprintf (stderr, "Erro ao criar FIFO %s\n", FIFO);
			return -1;
		}
	}
  printf ("\nAbrindo FIFO\n");
	if ((fifo = open (FIFO, O_RDONLY)) < 0) {
		fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
		return -2;
	}

	while (read (fifo, &ch, sizeof (ch)) > 0) {
    printf("\n");
		putchar (ch);
	}

	puts ("fim leitura");

	close (fifo);

	return 0;
}
