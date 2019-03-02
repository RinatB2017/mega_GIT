#ifndef R4_SERIAL_H
#define R4_SERIAL_H

#include <QStringList>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

enum Parity { Parity_Off = 0, 
              Parity_Enb = 1,
              Parity_Odd = 2
            };
enum Stream { Stream_Off = 0,
              Stream_Soft = 1,
              Stream_Hard = 2
            };
enum Read { Read_Off = 0,
            Read_On = 1
          };

typedef struct 
{
    QString device;
    int speed;
    int data;
    int stop;
    Parity parity;
    Stream stream;
    Read read;
} R4SerialSettings;

class R4Serial
{
public:
    R4Serial();
    QStringList getReadList();
    QStringList getStreamList();
    QStringList getSpeedList();
    QStringList getParityList();
    int getDescriptor() { return descriptor; }
    int open_device();
    void close_device();
    void get_device_info();
    int setSettings(const R4SerialSettings*, bool = false);
    int getSettings(R4SerialSettings*, bool = false);
    static int getDefaultDescriptor(const QString&);
    static int getDefaultSettings(R4SerialSettings*);
    static int listToSettings(const QStringList&, R4SerialSettings*);
    static int paramToSettings(const QStringList&, R4SerialSettings*);
    static int settingsToList(const R4SerialSettings*, QStringList&);
private:
    int descriptor;
    R4SerialSettings settings;
};

#endif // _R4_SERIAL_H

