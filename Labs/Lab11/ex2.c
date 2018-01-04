#include<stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
void main(void) {
	char *fd = "./sc/a/teste1.txt";
	struct stat *buf;
	buf = malloc(sizeof(struct stat));
	stat(fd, buf);
	int size = buf->st_size;
	printf("%d\n",size);
	int permissions = buf->st_mode;
	printf("%d\n",permissions);
	int accesstime = buf->st_atime;
	printf("%d\n",accesstime);
	int permissionschanged = buf->st_ctime;
	printf("%d\n",permissionschanged);
	int device = buf->st_uid;
	printf("%d\n",device);
	free(buf);
}
