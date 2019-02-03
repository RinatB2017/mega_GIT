//--------------------------------------------------------------------------------
#ifndef PARPORT_H
#define PARPORT_H
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_UNIX
#include <linux/ppdev.h>
#endif
//--------------------------------------------------------------------------------
#define BIT0  0x01
#define BIT1  0x02
#define BIT2  0x04
#define BIT3  0x08
#define BIT4  0x10
#define BIT5  0x20
#define BIT6  0x40
#define BIT7  0x80
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
//--------------------------------------------------------------------------------
#include <windef.h>
//--------------------------------------------------------------------------------
//Functions exported from DLL.
//For easy inclusion is user projects.
//Original InpOut32 function support
typedef void  _stdcall (*Out32)(short PortAddress, short data);
typedef short _stdcall (*Inp32)(short PortAddress);

//My extra functions for making life easy
typedef bool _stdcall (*IsInpOutDriverOpen)();      //Returns TRUE if the InpOut driver was opened successfully
typedef bool _stdcall (*IsXP64Bit)();               //Returns TRUE if the OS is 64bit (x64) Windows.

//DLLPortIO function support
typedef UCHAR _stdcall (*DlPortReadPortUchar) (USHORT port);
typedef void  _stdcall (*DlPortWritePortUchar)(USHORT port, UCHAR Value);

typedef USHORT _stdcall (*DlPortReadPortUshort) (USHORT port);
typedef void   _stdcall (*DlPortWritePortUshort)(USHORT port, USHORT Value);

typedef ULONG _stdcall (*DlPortReadPortUlong)(ULONG port);
typedef void  _stdcall (*DlPortWritePortUlong)(ULONG port, ULONG Value);

//WinIO function support (Untested and probably does NOT work - esp. on x64!)
typedef PBYTE _stdcall (*MapPhysToLin)(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle);
typedef bool  _stdcall (*UnmapPhysicalMemory)(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr);
typedef bool  _stdcall (*GetPhysLong)(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
typedef bool  _stdcall (*SetPhysLong)(PBYTE pbPhysAddr, DWORD dwPhysVal);
#endif
//--------------------------------------------------------------------------------
class Parport : public QObject
{
    Q_OBJECT
public:
    explicit Parport(QObject *parent = 0);
    //---
    bool open_parport(void);
    void close_parport(void);
    bool is_open(void);
    //---
    void set_read_on();
    void set_read_off();
    //---
    void setData(unsigned char data);
    void setData0(bool state);
    void setData1(bool state);
    void setData2(bool state);
    void setData3(bool state);
    void setData4(bool state);
    void setData5(bool state);
    void setData6(bool state);
    void setData7(bool state);
    //---
    unsigned char getData(void);
    bool getData0(void);
    bool getData1(void);
    bool getData2(void);
    bool getData3(void);
    bool getData4(void);
    bool getData5(void);
    bool getData6(void);
    bool getData7(void);
    //---
    void setStatusError(bool state);
    void setStatusSelect(bool state);
    void setStatusPaperOut(bool state);
    void setStatusACK(bool state);
    void setStatusBusy(bool state);
    //---
    bool getStatusError(void);
    bool getStatusSelect(void);
    bool getStatusPaperOut(void);
    bool getStatusACK(void);
    bool getStatusBusy(void);
    //---
    void setSelectIn(bool state);
    void setInit(bool state);
    void setStrobe(bool state);
    void setAutoFt(bool state);
    //---
    bool getSelectIn(void);
    bool getInit(void);
    bool getStrobe(void);
    bool getAutoFt(void);
    //---
    void setDataDir(unsigned char data);
    //---
    unsigned char get_control(void);
    void set_control(unsigned char data);
    //---
    unsigned char get_status(void);
    void set_status(unsigned char data);
    //---
    void set_all(void);
    void clr_all(void);
    //---
    void send_LED_on(unsigned char data);
    void send_LED_off(unsigned char data);

signals:
    void info(const QString  &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void log(const QString &data);

private:
    QWidget *parent;
    int fd, mode;
    bool port_is_open;
    QString parport_filename;
    QString lib_filename;
    int address_LPT;
    unsigned char state;

    void connect_log(void);
    void load_setting(void);
    //---
    void write_data(unsigned char data);
    unsigned char read_data(void);
    //---

#ifdef Q_OS_UNIX
    struct ppdev_frob_struct frob;
#endif

#ifdef Q_OS_WIN
    Out32 _Out32;
    Inp32 _Inp32;
    IsInpOutDriverOpen _IsInpOutDriverOpen;
    IsXP64Bit _IsXP64Bit;
    DlPortReadPortUchar _DlPortReadPortUchar;
    DlPortWritePortUchar _DlPortWritePortUchar;
    DlPortReadPortUshort _DlPortReadPortUshort;
    DlPortWritePortUshort _DlPortWritePortUshort;
    DlPortReadPortUlong _DlPortReadPortUlong;
    DlPortWritePortUlong _DlPortWritePortUlong;
    MapPhysToLin _MapPhysToLin;
    UnmapPhysicalMemory _UnmapPhysicalMemory;
    GetPhysLong _GetPhysLong;
    SetPhysLong _SetPhysLong;
#endif
};
//--------------------------------------------------------------------------------
#endif
