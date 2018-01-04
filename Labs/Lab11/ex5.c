#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

void main(void) {
    chmod("/sc/a/teste1.txt",0707);
}
