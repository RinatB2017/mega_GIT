#include "../inc/msg_q_linux_wrapper.h"

#define MAXSIZE 128

MessageQ::MessageQ(key_t yourMsgID)
{
        _msgID = msgget(yourMsgID, IPC_CREAT | 0666);
        if(_msgID < 0)
        {
            fprintf(stderr, "Message create failed with error num: %d", errno);
            exit(-1);
        }
        _msgToRcv = 0;
}

MessageQ::~MessageQ()
{
        if(msgctl(_msgID, IPC_RMID, 0) < 0)
        {
            fprintf(stderr, "Message delete failed with error num: %d", errno);
                        exit(-1);
        }
}

void MessageQ::receiveMsg(void* buffer, size_t msgSize)
{
    if(msgrcv(_msgID, buffer, msgSize, _msgToRcv, 0) < 0)
    {
        fprintf(stderr, "Message receive failed with error num: %d", errno);
        exit(-1);
    }
}

int MessageQ::sendMsg(void *buffer, size_t msgSize)
{
    if(msgsnd(_msgID, buffer, msgSize,   IPC_NOWAIT) < 0)
    {
        fprintf(stderr, "Message receive failed with error num: %d", errno);
        exit(-1);
    }
     return _msgID;
}



