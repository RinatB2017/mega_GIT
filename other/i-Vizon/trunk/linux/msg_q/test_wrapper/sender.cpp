#include <iostream>
#include "msg_q/inc/msg_q_linux_wrapper.h"
using namespace std;

typedef enum _resolution
{
    R640X480 = 0b0000,
    R600X800 = 0b0001
}RESOLUTION;

struct DvrSettings
{
    long int msgType;
    struct msgBitFields
    {
        long startNPause:1;
        long stop:1;
        long resolution:4;

    };
};

int main()
{
    cout << "Hello World!" << endl;
    MessageQ msgq(1234);
    return 0;
}

