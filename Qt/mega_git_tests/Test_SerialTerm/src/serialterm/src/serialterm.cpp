//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
#include "serialterm.hpp"
#include <QDateTime>
#include <QFile>
#include <QGuiApplication>
#include <QClipboard>
#include <unistd.h>
#include <QDesktopServices>
#include <QKeyEvent>

#include "const.h"
//--------------------------------------------------------------------------------
SerialTerm::SerialTerm(QWidget *parent):
    QTermWidget(0, parent)
{
    //macro:
    macrothread = new QThread();
    macroworker = new macroWorker();

    macroworker->moveToThread(macrothread);
    connect(macroworker, SIGNAL(workRequested()), macrothread, SLOT(start()));
    connect(macrothread, SIGNAL(started()), macroworker, SLOT(doWork()));
    connect(macroworker, SIGNAL(finished()), macrothread, SLOT(quit()), Qt::DirectConnection);

    //TODO:keybinding
    connect(this, SIGNAL(sendData(const char*,int)), this, SLOT(on_sendData(const char*,int)));
    connect(this, SIGNAL(receivedData(QString)), this, SLOT(on_receivedData(QString)));
    connect(this, SIGNAL(termKeyPressed(QKeyEvent*)), this, SLOT(on_keyPressed(QKeyEvent*)));
}
//--------------------------------------------------------------------------------
SerialTerm::~SerialTerm()
{
    //script
#if SUPPORT_SCRIPT == 1
    worker->deleteLater();
    delete engine;
#endif
}
//--------------------------------------------------------------------------------
void SerialTerm::show_data(QByteArray data)
{
    QByteArray ndata;
    ndata = data;
    write(this->getPtySlaveFd(), ndata.data(), ndata.size());
}
//--------------------------------------------------------------------------------
void SerialTerm::slot_handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError)
    {
        //qDebug() << "===== error :" << serial->errorString();
        emit sig_updateActionBtnStatus(false);
    }
}
//--------------------------------------------------------------------------------
QString SerialTerm::getCurrentDateTimeString()
{
    QDateTime dateTime = dateTime.currentDateTime();
    QString dateTimeString = QString("\n[%1] ").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss"));
    return dateTimeString;
}
//--------------------------------------------------------------------------------
void SerialTerm::setScrollToBottom(bool bScrollToBottom)
{
    _bScrollToBottom = bScrollToBottom;
}
//--------------------------------------------------------------------------------
bool SerialTerm::getScrollToBottom()
{
    return _bScrollToBottom;
}
//--------------------------------------------------------------------------------
//macro relative functions
void SerialTerm::macroStart()
{
    macroStop();
    macroworker->requestWork();
}
//--------------------------------------------------------------------------------
void SerialTerm::macroStop()
{
    if (macroworker->isRunning())
    {
        // To avoid having two threads running simultaneously, the previous thread is aborted.
        macroworker->abort();
        macrothread->wait(); // If the thread is not running, this will immediately return.
    }
}
//--------------------------------------------------------------------------------
bool SerialTerm::isMacroRunning()
{
    return macroworker->isRunning();
}
//--------------------------------------------------------------------------------
Qt::HANDLE SerialTerm::getMacroThreadId()
{
    if (macroworker->isRunning())
    {
        return macrothread->currentThreadId();
    }
    else
    {
        return 0;
    }
}
//--------------------------------------------------------------------------------
void SerialTerm::activateUrl(const QUrl &url,
                             bool fromContextMenu)
{
    Q_UNUSED(url);
    Q_UNUSED(fromContextMenu);
}
//--------------------------------------------------------------------------------
