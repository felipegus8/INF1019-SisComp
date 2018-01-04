#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

int main()
{
        int file=0;
        char buffer[17];
        file=open("./sc/a/teste1.txt",O_RDONLY);

        read(file,buffer,17);
        printf("%s\n",buffer);

        if(lseek(file,6,SEEK_SET) < 0) return 1;

        read(file,buffer,17);
        printf("%s\n",buffer);

        return 0;
}
