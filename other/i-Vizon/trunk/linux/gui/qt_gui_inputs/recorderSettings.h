#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <QMainWindow>
#include "msg_q/inc/msg_q_linux_wrapper.h"

namespace Ui {
class RecorderSettings;
}

#define MSGSZ  128

typedef enum _resolution
{
    R640X480 = 0b0000,
    R600X800 = 0b0001
}RESOLUTION;

typedef enum _status
{
    DVR_TRUE = 0b1,
    DVR_FALSE = 0b0
}STATUS;

struct msgBitFields
{
    unsigned long int signBit:1;
    unsigned long int startNPause:1;
    unsigned long int stop:1;
    unsigned long int resolution:4;
    unsigned long int frameChannels:3;
    unsigned long int videoFileType:3; // 1 + 1 + 4 + 3 + 3 = 12bits used of 64bits
};
struct DvrSettings
{
    long int msgType;   // Initialize it with 1
    msgBitFields flags;
};

enum _color_channel
{
    GRAY =0, RGB, BGR, YUV, RGBA, BGRA
};

class RecorderSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit RecorderSettings(QWidget *parent = 0);
    ~RecorderSettings();

private slots:

    void on_camIdTab1_activated(int index);


    void on_setTab1_clicked();
    void on_startTab1_clicked();
    void on_stopTab1_clicked();
    void on_colorChannelsTab1_activated(int index);

    void on_setTab1_clicked(bool checked);

private:
    Ui::RecorderSettings *ui;
    DvrSettings view;
    MessageQ* viewQ;
};

#endif // CAMERASETTINGS_H
