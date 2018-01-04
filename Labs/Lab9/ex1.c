#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>

#define MAXFILA 8
#define MSGMAX 50
#define ITEMS 64

typedef struct msgbuf {
  long    mtype;
  char    mtext[MSGMAX];
} message_buf;

int  msqid;

void produtor() {
	int i ;
	message_buf sbuf;
	size_t buf_size;
	for(i = 0; i < ITEMS; i++){
		//	sbuf.mtype = (i % 2) + 1;
			sprintf(sbuf.mtext, "Mensagem %d", i);
			buf_size = strlen(sbuf.mtext) + 1 ;
			if (msgsnd(msqid, &sbuf, buf_size, IPC_NOWAIT) < 0) {
				perror("msgsnd");
			}
			printf("O produtor produziu: %s\n", sbuf.mtext);
      sleep(1);
		}
}

void consumidor(int id){
	int i = 0;
	message_buf rbuf;
	while(i < ITEMS/2){
      sleep(2);
			if (msgrcv(msqid, &rbuf, MSGMAX, 0,MSG_NOERROR) < 0) {
        perror("msgrcv");
				printf("Erro no consumidor %d\n", id);
			}
			printf("O consumidor %d consumiu: %s\n",id, rbuf.mtext);
			i++;
	}
}

int main()
{
  int pid,pid2;
  key_t key = 2025;
	if ((msqid = msgget(key, IPC_CREAT | 0666 )) < 0) {
		perror("msgget");
		exit(1);
	}

  if ((pid = fork() == 0))  {
    //Filho
    consumidor(1);
  }
  else if ((pid2 = fork() == 0)) {
    consumidor(2);
  }
  else {
    produtor();
  }
	return 0;
}
