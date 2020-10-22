#ifndef CONST
#define CONST

#include <QtGlobal>
#include <QDir>

#define MYORGDOMAIN "coolshou.idv"

#define MYWEBURL "http://www.coolshou.idv.tw"
#define MYSOURCEURL "https://github.com/coolshou/qtvt.git"
#define MYRELEASEURL "https://api.github.com/repos/coolshou/qtvt/releases/latest"

#define MYPAYPAL "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JGSAZPUGJZKTC"

#if defined(Q_OS_WIN)
    //Win32
    #define MAX_USERNAME 256
    #define AUTOSTARTFOLDER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#elif defined(Q_OS_LINUX)
    //Linux
    #define DESKTOPFILE "qtvt.desktop"
    #define AUTOSTARTFOLDER ".config" + QDir::separator() + "autostart"
    #define INSTALLDESKTOPFILE  "/usr/share/applications/qtvt.desktop"
#else
    #error("Not supported SYSTEM")
#endif

#include <QSerialPort>

Q_DECLARE_METATYPE(QSerialPort::DataBits)
Q_DECLARE_METATYPE(QSerialPort::Parity)
Q_DECLARE_METATYPE(QSerialPort::StopBits)
Q_DECLARE_METATYPE(QSerialPort::FlowControl)

struct OSInfo
{
    QString cpuArch;
    QString productType;
    QString productVersion;
    QString account;
};

#endif // CONST

