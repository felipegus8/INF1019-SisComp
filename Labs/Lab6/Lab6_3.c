#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd[2]; // descritor dos pipes

    pipe(fd);
    if(fork() == 0) {
        close(fd[0]);
        dup2(fd[1], 1);  // Redireciona stdout para o pipe de escrita
        system("ls");
    }
    else {
    close(fd[1]);
    dup2(fd[0], 0); // Redireciona o stdin para o pipe de leitura
    system("cat");
    }
    return 0;
}
