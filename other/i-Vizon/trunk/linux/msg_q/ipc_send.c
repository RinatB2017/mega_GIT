//IPC_msgq_send.c
 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSIZE     128
 
void die(char *s)
{
  perror(s);
  exit(1);
}
 
struct msgbuf
{
    long int   mtype;
    char    mtext[MAXSIZE];
};
 
main()
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;
 
    key = 1234;
 
    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
      die("msgget");
 
    //Message Type
    sbuf.mtype = 1;
 
    printf("Enter a message to add to message queue : ");
    scanf("%[^\n]",sbuf.mtext);
    getchar();
 
    buflen = strlen(sbuf.mtext) + 1 ;
 
    if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
    {
        printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
        die("msgsnd");
    }
 
    else
        printf("Message Sent\n");
	
struct msgbuf rcvbuffer;
     //Receive an answer of message type 1.
    if (msgrcv(msqid, &rcvbuffer, buflen, 1, 0) < 0)
      die("msgrcv");
 
    printf("%s\n", rcvbuffer.mtext);
 
    exit(0);
}
