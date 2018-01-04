#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
  struct dirent **namelist;
  int n;

  n = scandir("./sc", &namelist, NULL, alphasort);


    while (n--) {
      if(strcmp("teste5.txt", namelist[n]->d_name) == 0) {
        char string[16] = "teste alteracao";
        char read_file[16];
        int fd2 = open(".//sc/teste5.txt", O_RDONLY, 0777);
        int fd3 = open("./sc/teste5.txt", O_WRONLY, 0777);
        write(fd3, string, 16 * sizeof(char));
        read(fd2, read_file, 16);
        printf("%s\n", read_file);
        exit(1);
      }
      free(namelist[n]);
}
    free(namelist);
}
