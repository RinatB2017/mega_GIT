#include <iostream>
#include "../inc/msg_q_linux_wrapper.h"
using namespace std;

#define MSGSZ  128

typedef enum _resolution
{
    R640X480 = 0b0000,
    R600X800 = 0b0001
}RESOLUTION;

struct msgBitFields
{
    long int startNPause:1;
    long int stop:1;
    long int resolution:4;
    long int frameChannels:3;
    long int videoFileType:3; // 1 + 1 + 4 + 3 + 3 = 12bits used of 64bits
};
struct DvrSettings
{
    long int msgType;   // Initialize it with 1
    msgBitFields flags;
};

int main()
{
    cout << "We will simulate msgq send & receive here..." << endl;

    DvrSettings model;
    MessageQ modelQ(9471);
    size_t buflen;

//    cout<<"Fill the data\n";
//    view.msgType = 1;
//    view.flags.startNPause = true;
//    view.flags.stop = false;
//    view.flags.resolution = R600X800;
//    view.flags.frameChannels = 0b010;
//    view.flags.videoFileType = 0b010;

    cout<<"Receive the data\n";
    modelQ.receiveMsg(&model, sizeof(DvrSettings));

    cout<<"Check the received data: ";

    if(model.flags.resolution == R600X800)
        printf(" Success\n");
    else
        cout<<" Failure\n";
    return 0;
}



