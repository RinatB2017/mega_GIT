//--------------------------------------------------------------------------------
#include <QObject>
#include <QSettings>
#include <QWidget>
#include <QLibrary>
#include <QString>
//--------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
//--------------------------------------------------------------------------------
#ifdef Q_OS_UNIX
#include <sys/io.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/parport.h>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#define PARPORT_CONTROL_STROBE    0x1
#define PARPORT_CONTROL_AUTOFD    0x2
#define PARPORT_CONTROL_INIT      0x4
#define PARPORT_CONTROL_SELECT    0x8

#define PARPORT_STATUS_ERROR      0x8
#define PARPORT_STATUS_SELECT     0x10
#define PARPORT_STATUS_PAPEROUT   0x20
#define PARPORT_STATUS_ACK        0x40
#define PARPORT_STATUS_BUSY       0x80
#endif
//--------------------------------------------------------------------------------
#define OBSOLETE__IOW _IOW
//--------------------------------------------------------------------------------
#include "parport.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Parport::Parport(QObject *parent) :
    QObject(parent),
    parent((QWidget *)parent),
    fd(0)
{
    connect_log();
    state = 0;
    port_is_open = false;
    load_setting();
}
//--------------------------------------------------------------------------------
void Parport::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void Parport::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#endif
}
//================================================================================
bool Parport::open_parport()
{
    port_is_open = false;

    emit info("open_parport: Parport is started!");
#ifdef Q_OS_UNIX
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    fd = open(parport_filename.toAscii().data(), O_RDWR);
#else
    fd = open(parport_filename.toLocal8Bit().data(), O_RDWR);
#endif
    if (fd == -1)
    {
        emit error(QString("open_parport: %1 not open!").arg(parport_filename));
        return false;
    }
    //-----
    if (ioctl(fd,PPCLAIM))
    {
        close(fd);
        emit error("open_parport: PPCLAIM");
        return false;
    }
    //-----
    mode = IEEE1284_MODE_COMPAT;

    //    mode = IEEE1284_MODE_NIBBLE;
    //    mode = IEEE1284_MODE_BYTE;
    //    mode = IEEE1284_MODE_COMPAT;
    //    mode = IEEE1284_MODE_BECP;
    //    mode = IEEE1284_MODE_ECP;
    //    mode = IEEE1284_MODE_ECPRLE;
    //    mode = IEEE1284_MODE_ECPSWE;
    //    mode = IEEE1284_MODE_EPP;
    //    mode = IEEE1284_MODE_EPPSL;
    //    mode = IEEE1284_MODE_EPPSWE;
    //    mode = IEEE1284_DEVICEID;
    //    mode = IEEE1284_EXT_LINK;

    if (ioctl(fd, PPNEGOT, &mode))
    {
        close (fd);
        emit error("open_parport: PPNEGOT");
        return false;
    }
#endif //Q_OS_UNIX
#ifdef Q_OS_WIN
    bool temp;

    QLibrary lib(lib_filename);
    temp = lib.load();
    if(!temp)
    {
        emit error(QString("Error: %1 not loaded").arg(lib_filename));
        return false;
    }
    _IsInpOutDriverOpen = (IsInpOutDriverOpen) lib.resolve("IsInpOutDriverOpen");
    if (!_IsInpOutDriverOpen)
    {
        emit error("Error load IsInpOutDriverOpen");
        return false;
    }
    _Out32 = (Out32) lib.resolve("Out32");
    if (!_IsInpOutDriverOpen)
    {
        emit error("Error load Out32");
        return false;
    }
    _Inp32 = (Inp32) lib.resolve("Inp32");
    if (!_Inp32)
    {
        emit error("Error load Inp32");
        return false;
    }
    _IsXP64Bit = (IsXP64Bit) lib.resolve("IsXP64Bit");
    if (!_IsXP64Bit)
    {
        emit error("Error load IsXP64Bit");
        return false;
    }
    _DlPortReadPortUchar = (DlPortReadPortUchar) lib.resolve("DlPortReadPortUchar");
    if (!_DlPortReadPortUchar)
    {
        emit error("Error load DlPortReadPortUchar");
        return false;
    }
    _DlPortWritePortUchar = (DlPortWritePortUchar) lib.resolve("DlPortWritePortUchar");
    if (!_DlPortWritePortUchar)
    {
        emit error("Error load DlPortWritePortUchar");
        return false;
    }
    _DlPortReadPortUshort = (DlPortReadPortUshort) lib.resolve("DlPortReadPortUshort");
    if (!_DlPortReadPortUshort)
    {
        emit error("Error load DlPortReadPortUshort");
        return false;
    }
    _DlPortWritePortUshort = (DlPortWritePortUshort) lib.resolve("DlPortWritePortUshort");
    if (!_DlPortWritePortUshort)
    {
        emit error("Error load DlPortWritePortUshort");
        return false;
    }
    _DlPortReadPortUlong = (DlPortReadPortUlong) lib.resolve("DlPortReadPortUlong");
    if (!_DlPortReadPortUlong)
    {
        emit error("Error load DlPortReadPortUlong");
        return false;
    }
    _DlPortWritePortUlong = (DlPortWritePortUlong) lib.resolve("DlPortWritePortUlong");
    if (!_DlPortWritePortUlong)
    {
        emit error("Error load DlPortWritePortUlong");
        return false;
    }
    _MapPhysToLin = (MapPhysToLin) lib.resolve("MapPhysToLin");
    if (!_MapPhysToLin)
    {
        emit error("Error load MapPhysToLin");
        return false;
    }
    _UnmapPhysicalMemory = (UnmapPhysicalMemory) lib.resolve("UnmapPhysicalMemory");
    if (!_UnmapPhysicalMemory)
    {
        emit error("Error load UnmapPhysicalMemory");
        return false;
    }
    _GetPhysLong = (GetPhysLong) lib.resolve("GetPhysLong");
    if (!_GetPhysLong)
    {
        emit error("Error load GetPhysLong");
        return false;
    }
    _SetPhysLong = (SetPhysLong) lib.resolve("SetPhysLong");
    if (!_SetPhysLong)
    {
        emit error("Error load SetPhysLong");
        return false;
    }
#endif //Q_OS_WIN

    port_is_open = true;
    return true;
}
//--------------------------------------------------------------------------------
void Parport::close_parport()
{
#ifdef Q_OS_UNIX
    ioctl (fd, PPRELEASE);
    close(fd);
#endif
}
//--------------------------------------------------------------------------------
bool Parport::is_open()
{
    return port_is_open;
}
//================================================================================
void Parport::write_data(unsigned char data)
{
    if(!port_is_open)
    {
        emit error("write_data: port not open!");
        return;
    }

    //    emit error(QString("write_data = %1").arg(data));
    set_read_off();
#ifdef Q_OS_UNIX
    ioctl(fd, PPWDATA, &data);
    fflush(stdout);
#endif
#ifdef Q_OS_WIN
    _Out32(address_LPT, data);
#endif
}
//--------------------------------------------------------------------------------
unsigned char Parport::read_data(void)
{
    unsigned char data;
    if(!port_is_open)
    {
        emit error("read_data: port not open!");
        return 0;
    }

    //    emit error(QString("read_data = %1").arg(data));
    set_read_on();
#ifdef Q_OS_UNIX
    ioctl(fd, PPRDATA, &data);
#endif
#ifdef Q_OS_WIN
    data = _Inp32(address_LPT);
#endif

    return data;
}
//================================================================================
void Parport::set_control(unsigned char data)
{
    if(!port_is_open)
    {
        emit error("set_control: port not open!");
        return;
    }

#ifdef Q_OS_UNIX
    ioctl(fd, PPWCONTROL, &data);
    fflush(stdout);
#endif
#ifdef Q_OS_WIN
    _Out32(address_LPT+2, data);
#endif
}
//--------------------------------------------------------------------------------
unsigned char Parport::get_control()
{
    if(!port_is_open)
    {
        emit error("get_control: port not open!");
        return 0;
    }

    unsigned char temp = 0;
#ifdef Q_OS_UNIX
    ioctl(fd, PPRCONTROL, &temp);
#endif
#ifdef Q_OS_WIN
    temp = _Inp32(address_LPT+2);
#endif

    return temp;
}
//================================================================================
void Parport::set_status(unsigned char data)
{
    if(!port_is_open)
    {
        emit error("set_status: port not open!");
        return;
    }

    emit debug("не полностью реализовано");
#ifdef Q_OS_UNIX
    ioctl(fd, PPWSTATUS, &data);
    fflush(stdout);
#endif
#ifdef Q_OS_WIN
    _Out32(address_LPT+1, data);
#endif
}
//--------------------------------------------------------------------------------
unsigned char Parport::get_status()
{
    if(!port_is_open)
    {
        emit error("get_status: port not open!");
        return 0;
    }

    unsigned char temp = 0;

#ifdef Q_OS_UNIX
    ioctl(fd, PPRSTATUS, &temp);
    //    ioctl(fd, PPRELEASE);
    //    ioctl(fd, PPCLAIM);
#endif
#ifdef Q_OS_WIN
    temp = _Inp32(address_LPT+1);
#endif

    return temp;
}
//--------------------------------------------------------------------------------
void Parport::setDataDir(unsigned char data)
{
    if(state == data) return;
    state = data;

#ifdef FAKE_DATADIR
    return;
#endif
#ifdef Q_OS_UNIX
    ioctl(fd, PPDATADIR, &data);
    fflush(stdout);
#endif
#ifdef Q_OS_WIN
    unsigned char temp;
    temp = _Inp32(address_LPT+2);
    if(data == 0xFF)
    {
        temp = temp | BIT5;
    }
    else
    {
        temp = temp & (~BIT5);
    }
    _Out32(address_LPT+2, temp);
#endif
}
//================================================================================
void Parport::set_read_on()
{
    setDataDir(0xFF);
}
//--------------------------------------------------------------------------------
void Parport::set_read_off()
{
    setDataDir(0);
}
//================================================================================
void Parport::setData(unsigned char data)
{
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData0(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT0;
    else data = data & (~BIT0);
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData1(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT1;
    else data = data & (~BIT1);
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData2(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT2;
    else data = data & (~BIT2);
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData3(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT3;
    else data = data & (~BIT3);
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData4(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT4;
    else data = data & (~BIT4);
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData5(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT5;
    else data = data & (~BIT5);
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData6(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT6;
    else data = data & (~BIT6);
    write_data(data);
}
//--------------------------------------------------------------------------------
void Parport::setData7(bool state)
{
    unsigned char data;
    data = read_data();
    if(state) data = data | BIT7;
    else data = data & (~BIT7);
    write_data(data);
}
//================================================================================
unsigned char Parport::getData(void)
{
    return read_data();
}
//--------------------------------------------------------------------------------
bool Parport::getData0(void)
{
    unsigned char data;
    data = read_data();
    return (data & 1);
}
//--------------------------------------------------------------------------------
bool Parport::getData1(void)
{
    unsigned char data;
    data = read_data();
    return (data & (1 << 1));
}
//--------------------------------------------------------------------------------
bool Parport::getData2(void)
{
    unsigned char data;
    data = read_data();
    return (data & (1 << 2));
}
//--------------------------------------------------------------------------------
bool Parport::getData3(void)
{
    unsigned char data;
    data = read_data();
    return (data & (1 << 3));
}
//--------------------------------------------------------------------------------
bool Parport::getData4(void)
{
    unsigned char data;
    data = read_data();
    return (data & (1 << 4));
}
//--------------------------------------------------------------------------------
bool Parport::getData5(void)
{
    unsigned char data;
    data = read_data();
    return (data & (1 << 5));
}
//--------------------------------------------------------------------------------
bool Parport::getData6(void)
{
    unsigned char data;
    data = read_data();
    return (data & (1 << 6));
}
//--------------------------------------------------------------------------------
bool Parport::getData7(void)
{
    unsigned char data;
    data = read_data();
    return (data & (1 << 7));
}
//================================================================================
void Parport::setStatusError(bool state)
{
    unsigned char status;
    status = get_status();
    if(!state) status = status | PARPORT_STATUS_ERROR;
    else status = status & (~PARPORT_STATUS_ERROR);
    set_status(status);
}
//--------------------------------------------------------------------------------
void Parport::setStatusSelect(bool state)
{
    unsigned char status;
    status = get_status();
    if(!state) status = status | PARPORT_STATUS_SELECT;
    else status = status & (~PARPORT_STATUS_SELECT);
    set_status(status);
}
//--------------------------------------------------------------------------------
void Parport::setStatusPaperOut(bool state)
{
    unsigned char status;
    status = get_status();
    if(!state) status = status | PARPORT_STATUS_PAPEROUT;
    else status = status & (~PARPORT_STATUS_PAPEROUT);
    set_status(status);
}
//--------------------------------------------------------------------------------
void Parport::setStatusACK(bool state)
{
    unsigned char status;
    status = get_status();
    if(!state) status = status | PARPORT_STATUS_ACK;
    else status = status & (~PARPORT_STATUS_ACK);
    set_status(status);
}
//--------------------------------------------------------------------------------
void Parport::setStatusBusy(bool state)
{
    unsigned char status;
    status = get_status();
    if(!state) status = status | PARPORT_STATUS_BUSY;
    else status = status & (~PARPORT_STATUS_BUSY);
    set_status(status);
}
//--------------------------------------------------------------------------------
bool Parport::getStatusError(void)
{
    unsigned char status;
    status = get_status();
    return (status & PARPORT_STATUS_ERROR);
}
//--------------------------------------------------------------------------------
bool Parport::getStatusSelect(void)
{
    unsigned char status;
    status = get_status();
    return (status & PARPORT_STATUS_SELECT);
}
//--------------------------------------------------------------------------------
bool Parport::getStatusPaperOut(void)
{
    unsigned char status;
    status = get_status();
    return (status & PARPORT_STATUS_PAPEROUT);
}
//--------------------------------------------------------------------------------
bool Parport::getStatusACK(void)
{
    unsigned char status;
    status = get_status();
    return (status & PARPORT_STATUS_ACK);
}
//--------------------------------------------------------------------------------
bool Parport::getStatusBusy(void)
{
    unsigned char status;
    status = get_status();
    return (status & PARPORT_STATUS_BUSY);
}
//================================================================================
void Parport::setSelectIn(bool state)
{
    if(!port_is_open)
    {
        emit error("setSelectIn: port not open!");
        return;
    }

    unsigned char temp = get_control();
    if(state) temp  = temp & (~PARPORT_CONTROL_SELECT);
    else      temp  = temp | PARPORT_CONTROL_SELECT;
    set_control(temp);
}
//--------------------------------------------------------------------------------
void Parport::setInit(bool state)
{
    if(!port_is_open)
    {
        emit error("setInit: port not open!");
        return;
    }

    unsigned char temp = get_control();
    if(state) temp  = temp | PARPORT_CONTROL_INIT;
    else      temp  = temp & (~PARPORT_CONTROL_INIT);
    set_control(temp);
}
//--------------------------------------------------------------------------------
void Parport::setStrobe(bool state)
{
    if(!port_is_open)
    {
        emit error("setStrobe: port not open!");
        return;
    }

    unsigned char temp = get_control();
    if(state) temp  = temp & (~PARPORT_CONTROL_STROBE);
    else      temp  = temp | PARPORT_CONTROL_STROBE;
    set_control(temp);
}
//--------------------------------------------------------------------------------
void Parport::setAutoFt(bool state)
{
    if(!port_is_open)
    {
        emit error("setAutoFt: port not open!");
        return;
    }

    unsigned char temp = get_control();
    if(state) temp  = temp & (~PARPORT_CONTROL_AUTOFD);
    else      temp  = temp | PARPORT_CONTROL_AUTOFD;
    set_control(temp);
}
//================================================================================
bool Parport::getSelectIn(void)
{
    unsigned char temp = get_control();
    return (temp & PARPORT_CONTROL_SELECT);
}
//--------------------------------------------------------------------------------
bool Parport::getInit(void)
{
    unsigned char temp = get_control();
    return (temp & PARPORT_CONTROL_INIT);
}
//--------------------------------------------------------------------------------
bool Parport::getStrobe(void)
{
    unsigned char temp = get_control();
    return (temp & PARPORT_CONTROL_STROBE);
}
//--------------------------------------------------------------------------------
bool Parport::getAutoFt(void)
{
    unsigned char temp = get_control();
    return (temp & PARPORT_CONTROL_AUTOFD);
}
//================================================================================
void Parport::set_all()
{
    if(!port_is_open)
    {
        emit error("set_all: port not open!");
        return;
    }

    write_data(0xFF);
    setInit(1);
    setSelectIn(1);
    setStrobe(1);
    setAutoFt(1);
}
//--------------------------------------------------------------------------------
void Parport::clr_all()
{
    if(!port_is_open)
    {
        emit error("clr_all: port not open!");
        return;
    }

    write_data(0x00);
    setInit(0);
    setSelectIn(0);
    setStrobe(0);
    setAutoFt(0);
}
//--------------------------------------------------------------------------------
void Parport::send_LED_on(unsigned char data)
{
    unsigned char temp;
    if(!port_is_open)
    {
        emit error("send_LED_on: port not open!");
        return;
    }

    temp = read_data() | data;
    write_data(temp);
}
//--------------------------------------------------------------------------------
void Parport::send_LED_off(unsigned char data)
{
    unsigned char temp;
    if(!port_is_open)
    {
        emit error("send_LED_off: port not open!");
        return;
    }

    temp = read_data() | data;
    write_data(temp);
}
//--------------------------------------------------------------------------------
void Parport::load_setting(void)
{
    bool ok;
    QString temp;
    QSettings setting("settings.ini", QSettings::IniFormat);
    parport_filename = setting.value("parport/Parport_filename", "/dev/parport0").toString();
    lib_filename = setting.value("parport/Lib_filename", "inpout32.dll").toString();
    temp = setting.value("parport/AddressLPT", "0x378").toString();
    address_LPT = temp.toInt(&ok, 16);
    if(!ok) address_LPT = 0x378;
}
//--------------------------------------------------------------------------------
