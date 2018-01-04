#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
void main(void) {
	char str[] = "Teste escrevendo"; 
	mkdir("./sc",0700);
	mkdir("./sc/a",0700);
	mkdir("./sc/b",0700);
	mkdir("./sc/c",0700);

	int filedesc = open("./sc/a/teste1.txt", O_WRONLY | O_CREAT,0777);
   	write(filedesc,str, sizeof(str));
    int filedesc2 = open("./sc/b/teste2.txt", O_WRONLY | O_CREAT,0777);
   	write(filedesc2,str, sizeof(str));
	int filedesc3 = open("./sc/c/teste3.txt", O_WRONLY | O_CREAT,0777);
   	write(filedesc3,str, sizeof(str));
}
