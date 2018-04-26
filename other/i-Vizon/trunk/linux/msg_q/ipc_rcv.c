//IPC_msgq_rcv.c
 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE     128
 
void die(char *s)
{
  perror(s);
  exit(1);
}
 
typedef struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
} ;
 
 
main()
{
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;
 
    key = 1234;
 
    if ((msqid = msgget(key, 0666)) < 0)
      die("msgget()");
 
 
     //Receive an answer of message type 1.
    if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
      die("msgrcv");
 
    printf("%s\n", rcvbuffer.mtext);
    exit(0);
}

