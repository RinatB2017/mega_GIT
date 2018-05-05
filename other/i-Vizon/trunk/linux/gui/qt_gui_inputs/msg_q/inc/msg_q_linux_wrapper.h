#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>

/// Message structure
/**
 *  struct yourMsgName
 *  {
 *      log int msgType;
 *      datatype feild1, feild2, ...;
 *  }
 *  Usage:
 *  youMsgName obj;
 *  obj.msgType = 1;
 *  and fill your dat in feilds
 */

class MessageQ
{
public:
    MessageQ(key_t yourMsgID);
    ~MessageQ();

    void receiveMsg(void* buffer, size_t size);
    int sendMsg(void* buffer, size_t msgSize);
protected:
private:
    int _msgID;
    int _msgToRcv;
};


