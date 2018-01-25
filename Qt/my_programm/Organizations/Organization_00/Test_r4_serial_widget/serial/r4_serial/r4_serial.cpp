#include "r4_serial.h"

const int Dev_Index = 0;
const int Speed_Index = 1;
const int Data_Index = 2;
const int Stop_Index = 3;
const int Parity_Index = 4;
const int Stream_Index = 5;
const int Rec_Index = 6;

#define ON_TEXT     "Вкл"
#define OFF_TEXT    "Выкл"
#define ODD_TEXT    "Чет"
#define ENB_TEXT    "Нечет"
#define SOFT_TEXT   "Прогр"
#define HARD_TEXT   "Аппар"


R4Serial::R4Serial()
{
    getDefaultSettings(&settings);
    descriptor = -1;
}

int R4Serial::setSettings(const R4SerialSettings* ptr_settings, bool open/*=false*/)
{
    if (!ptr_settings) return -1;
    settings = *ptr_settings;
    if (!open) return 0;
    return open_device();
}

int R4Serial::getSettings(R4SerialSettings* ptr_settings, bool req /*=false*/)
{
    if (!ptr_settings) return -1;
    if (req) get_device_info();
    *ptr_settings = settings;
    return 0;
}

void R4Serial::get_device_info()
{
    struct termios ter;

    int fd = descriptor;
    // --- obtain the current settings
    tcgetattr(fd, &ter);
    // --- get speed
    speed_t speed = cfgetispeed( &ter );
    switch( speed )
    {
    case B2400: settings.speed = 2400; break;
    case B4800: settings.speed = 4800; break;
    case B9600: settings.speed = 9600; break;
    case B19200: settings.speed = 19200; break;
    case B38400: settings.speed = 38400; break;
    case B57600: settings.speed = 57600; break;
    case B115200: settings.speed = 115200; break;
    case B230400: settings.speed = 230400; break;
    case B460800: settings.speed = 460800; break;
    default: settings.speed = 1200; break;
    }
    // --- get the number of data bits
    if ( ( ter.c_cflag & CSIZE ) ==  CS5 ) settings.data = 5;
    else if ( ( ter.c_cflag & CSIZE ) == CS6 ) settings.data = 6;
    else if ( ( ter.c_cflag & CSIZE ) == CS7 ) settings.data = 7;
    else settings.data = 8;
    // --- get the number stop bits
    if ( ter.c_cflag & CSTOPB ) settings.stop = 2;
    else settings.stop = 1;
    // --- get a parity
    if ( ter.c_cflag & PARENB ) settings.parity = Parity_Enb;
    else if ( ter.c_cflag & PARODD ) settings.parity = Parity_Odd;
    else settings.parity = Parity_Off;
    // --- stream control
    if ( ter.c_cflag & CRTSCTS ) settings.stream = Stream_Hard;
    else if  ( ter.c_iflag & ( IXON | IXOFF | IXANY ) )
        settings.stream = Stream_Soft;
    else
        settings.stream = Stream_Off;
    // --- read
    if ( ter.c_cflag & CREAD ) settings.read = Read_On;
    else settings.read = Read_Off;
}

void R4Serial::close_device()
{
    close(descriptor);
}

int R4Serial::open_device()
{
    // --- get a descriptor
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    int fd = open( settings.device.toAscii().data(), O_RDWR | O_NDELAY );
#else
    int fd = open( settings.device.toLocal8Bit().data(), O_RDWR | O_NDELAY );
#endif
    if ( fd < 0 )
        return -2;
    struct termios ter;
    // --- obtain the current settings
    tcgetattr( fd, &ter );
    // --- set the default settings
    cfmakeraw( &ter );
    // --- set speed
    int speed_val = settings.speed;
    speed_t speed;
    if (speed_val <= 1200) speed = B1200;
    else if (speed_val <= 2400) speed = B2400;
    else if (speed_val <= 4800) speed = B4800;
    else if (speed_val <= 9600) speed = B9600;
    else if (speed_val <= 19200) speed = 19200;
    else if (speed_val <= 38400) speed = B38400;
    else if (speed_val <= 57600) speed = B57600;
    else if (speed_val <= 115200) speed = B115200;
    else if (speed_val <= 230400) speed = B230400;
    else  speed = B460800;
    cfsetispeed( &ter, speed );
    cfsetospeed( &ter, speed );
    // --- set the number of data bits
    ter.c_cflag &= ~CSIZE;
    switch (settings.data)
    {
    case 5: ter.c_cflag |= CS5; break;
    case 6: ter.c_cflag |= CS6; break;
    case 7: ter.c_cflag |= CS7; break;
    case 8: ter.c_cflag |= CS8; break;
    default:
        return -3;
        break;
    }
    // --- set the number of stop bits
    switch(settings.stop)
    {
    // -- 1 stop bit
    case 1: ter.c_cflag &= ~CSTOPB; break;
        // --- 2 stop bit
    case 2: ter.c_cflag |= CSTOPB; break;
    default:
        return -4;
        break;
    }
    // --- set a sign of parity
    switch(settings.parity)
    {
    // --- parity
    case Parity_Enb:
        ter.c_cflag |= PARENB; ter.c_cflag &= ~PARODD;
        break;
        // --- odd
    case Parity_Odd:
        ter.c_cflag &= ~PARENB; ter.c_cflag |= PARODD;
        break;
        // --- off
    case Parity_Off:
        ter.c_cflag &= ~PARENB; ter.c_cflag &= ~PARODD;
        break;
    default:
        return -5;
        break;
    }
    // set the stream control
    switch (settings.stream)
    {
    // --- program control
    case Stream_Soft:
        ter.c_cflag &= ~CRTSCTS;
        ter.c_iflag |= ( IXON | IXOFF | IXANY );
        break;
        // --- hard control
    case Stream_Hard:
        ter.c_cflag |= CRTSCTS;
        ter.c_iflag &= ~( IXON | IXOFF | IXANY );
        break;
        // --- off
    case Stream_Off:
        ter.c_cflag &= ~CRTSCTS;
        ter.c_iflag &= ~( IXON | IXOFF | IXANY );
        break;
    default:
        return -6;
        break;
    }
    // --- read on/off
    switch(settings.read)
    {
    // --- on
    case Read_On: ter.c_cflag |= CREAD; break;
        // --- off
    case Read_Off: ter.c_cflag &= ~CREAD; break;
    default:
        return -7;
        break;
    }

    //???
    ter.c_cc[VTIME] = 1;
    ter.c_cc[VMIN] = 1;
    // --- apply settings
    tcsetattr( fd, TCSANOW, &ter );
    descriptor = fd;
    return descriptor;
}

QStringList R4Serial::getReadList() 
{ 
    return QStringList() << ON_TEXT
                         << OFF_TEXT;
}

QStringList R4Serial::getStreamList() 
{ 
    return QStringList() << SOFT_TEXT
                         << HARD_TEXT
                         << OFF_TEXT;
}

QStringList R4Serial::getSpeedList() 
{ 
    return QStringList() << "1200"
                         << "2400"
                         << "4800"
                         << "9600"
                         << "19200"
                         << "38400"
                         << "57600"
                         << "115200"
                         << "230400"
                         << "460800";
}

QStringList R4Serial::getParityList() 
{ 
    return QStringList() << ODD_TEXT
                         << ENB_TEXT
                         << OFF_TEXT;
}

int R4Serial::getDefaultDescriptor(const QString& device)
{
    R4SerialSettings s;
    getDefaultSettings(&s);
    s.device = device;
    R4Serial serial;
    return serial.setSettings(&s, true);
}

int R4Serial::getDefaultSettings(R4SerialSettings* ptr)
{
    if (!ptr) return -1;
    ptr->device = "/dev/ttyS0";
    ptr->speed = 9600;
    ptr->data = 8;
    ptr->stop = 1;
    ptr->parity = Parity_Off;
    ptr->stream = Stream_Off;
    ptr->read = Read_On;
    return 0;
}

int R4Serial::listToSettings(const QStringList& list, R4SerialSettings* ptr_settings)
{
    if (list.count() != 7 || !ptr_settings) return -1;
    ptr_settings->device = list.at(Dev_Index);
    ptr_settings->speed = list.at(Speed_Index).toInt();
    ptr_settings->data = list.at(Data_Index).toInt();
    ptr_settings->stop = list.at(Stop_Index).toInt();
    QString par = list.at(Parity_Index);
    if (par == ODD_TEXT) ptr_settings->parity = Parity_Odd;
    else if (par == ENB_TEXT) ptr_settings->parity = Parity_Enb;
    else if (par == OFF_TEXT) ptr_settings->parity = Parity_Off;
    QString strm = list.at(Stream_Index);
    if (strm == SOFT_TEXT) ptr_settings->stream = Stream_Soft;
    else if (strm == HARD_TEXT) ptr_settings->stream = Stream_Hard;
    else ptr_settings->stream = Stream_Off;
    if (list.at(Rec_Index) == OFF_TEXT) ptr_settings->read = Read_Off;
    else ptr_settings->read = Read_On;
    return 0;
}

int R4Serial::paramToSettings(const QStringList& list, R4SerialSettings* ptr_settings)
{
    if (list.count() != 7 || !ptr_settings) return -1;
    ptr_settings->device = list.at(Dev_Index);
    ptr_settings->speed = list.at(Speed_Index).toInt();
    ptr_settings->data = list.at(Data_Index).toInt();
    ptr_settings->stop = list.at(Stop_Index).toInt();
    bool isok;
    int par = list.at(Parity_Index).toInt(&isok);
    if (!isok) return -2;
    if (par == 0) ptr_settings->parity = Parity_Off;
    else if (par == 1)  ptr_settings->parity = Parity_Enb;
    else if (par == 2) ptr_settings->parity = Parity_Odd;
    else return -3;
    int strm = list.at(Stream_Index).toInt(&isok);
    if (!isok) return -4;
    if (strm == 1) ptr_settings->stream = Stream_Soft;
    else if (strm == 2) ptr_settings->stream = Stream_Hard;
    else if (strm == 0) ptr_settings->stream = Stream_Off;
    else return -5;
    int rec = list.at(Rec_Index).toInt(&isok);
    if  (!isok) return -6;
    if (rec == 0) ptr_settings->read = Read_Off;
    else if (rec == 1) ptr_settings->read = Read_On;
    else return -7;
    return 0;
}

int R4Serial::settingsToList(const R4SerialSettings* ptr, QStringList& list)
{
    if (!ptr) return -1;
    list << ptr->device;
    list << QString::number(ptr->speed);
    list << QString::number(ptr->data);
    list << QString::number(ptr->stop);
    if (ptr->parity == Parity_Odd) list << ODD_TEXT;
    else if (ptr->parity == Parity_Enb) list << ENB_TEXT;
    else list << OFF_TEXT;
    if (ptr->stream == Stream_Soft) list << SOFT_TEXT;
    else if (ptr->stream == Stream_Hard) list << HARD_TEXT;
    else list << OFF_TEXT;
    if (ptr->read == Read_On) list << ON_TEXT;
    else list << OFF_TEXT;
    return 0;
}
