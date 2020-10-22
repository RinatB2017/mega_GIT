#include "serialterm.hpp"
#include <QDateTime>
#include <QFile>
#include <QGuiApplication>
#include <QClipboard>
#include <unistd.h>
#include <QDesktopServices>
#include <QKeyEvent>

#include "const.h"

SerialTerm::SerialTerm(QWidget *parent,
                       QString comName,
                       QSettings *settings):
    QTermWidget(0, parent),
    mComName(comName)
{
    mSettings = settings;

    if(mSettings == nullptr)
    {
        mSettings = new QSettings();
    }

    serial = new QSerialPort(this);
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(slot_handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readDataFromSerial()));
    connect(serial, SIGNAL(baudRateChanged(qint32,QSerialPort::Directions)),
            this, SLOT(slot_baudRateChanged(qint32,QSerialPort::Directions)));
    apply_setting();

    //connect(this, SIGNAL(sig_DataReady(QByteArray)), this, SLOT(writeDataToSerial(QByteArray)));
    //macro:
    macrothread = new QThread();
    macroworker = new macroWorker();

    macroworker->moveToThread(macrothread);
    //connect(macroworker, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
    connect(macroworker, SIGNAL(workRequested()), macrothread, SLOT(start()));
    connect(macrothread, SIGNAL(started()), macroworker, SLOT(doWork()));
    connect(macroworker, SIGNAL(finished()), macrothread, SLOT(quit()), Qt::DirectConnection);

    //TODO:keybinding
    connect(this, SIGNAL(sendData(const char*,int)), this, SLOT(on_sendData(const char*,int)));
    connect(this, SIGNAL(receivedData(QString)), this, SLOT(on_receivedData(QString)));
    connect(this, SIGNAL(termKeyPressed(QKeyEvent*)), this, SLOT(on_keyPressed(QKeyEvent*)));
}

SerialTerm::~SerialTerm()
{
    /*    //macro
    macroworker->abort();
    macrothread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    //macrothread->deleteLater();
    delete macrothread;
    delete macroworker;
    */
    delete serial;
    //script
#if SUPPORT_SCRIPT == 1
    worker->deleteLater();
    delete engine;
#endif
}

void SerialTerm::readDataFromSerial()
{
    QByteArray ndata;
    QByteArray data = serial->readAll();
    //remove local echo
    if (lastSend.length()>0)
    {
        //qDebug() << "going to remove: " << lastSend << " len:" << lastSend.length();
        ndata = data.remove(0,lastSend.length());
        lastSend.clear();
    }
    else
    {
        ndata = data;
    }

    if (_bLogDatetime)
    {
        QString dateTimeString = getCurrentDateTimeString();
        ndata.replace(QString("\n"), dateTimeString.toLatin1());
    }
    if (_bLogEnable)
    {
        logToFile(ndata);
    }
    //qDebug() << "readDataFromSerial:" << ndata;
    //show in qtermwidget
    qDebug() << "ndata: " << ndata;
    write(this->getPtySlaveFd(), ndata.data(), ndata.size());
}

//write QByteArray to serial
void SerialTerm::writeDataToSerial(const QByteArray &data)
{
    //if (bLogEnable) {
    //    logToFile(data);
    //}
    if (isOpen())
    {
        serial->write(data);
    }
    else
    {
        qDebug()<< "open serial first!";
    }
}

void SerialTerm::slot_handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError)
    {
        //qDebug() << "===== error :" << serial->errorString();
        emit sig_updateActionBtnStatus(false);
        emit sig_updateStatus(serial->errorString());
    }
}

void SerialTerm::slot_baudRateChanged(qint32 baudRate,QSerialPort::Directions directions)
{
    if (serial->isOpen())
    {
        closeSerialPort();
        qDebug() << "TODO: baudRate change:" << baudRate;
        bool bOpen = openSerialPort();
        this->setEnabled(bOpen);
    }
    Q_UNUSED(baudRate);
    Q_UNUSED(directions);
}

void SerialTerm::on_receivedData(const QString &data)
{
    //TODO: log to file/do macro
    //TODO: send only \r, \r will still add in? why
    qDebug() << "on_receivedData:" << data;
}

void SerialTerm::on_sendData(const char* data,int len)
{
    QByteArray b = QByteArray(data,len);
    //record sended data for later remove!!
    lastSend = b;
    serial->write(b);
}

void SerialTerm::on_keyPressed(QKeyEvent *keyEvent)
{
    qDebug() << "on_keyPressed:" << keyEvent->text();
    //TODO: special key combind press event
    if (keyEvent->matches(QKeySequence::Copy))
    {
        //ctrl+c => send ctrl+c to break procress running
        const char ctrlC = 0x03;
        this->sendData(&ctrlC,sizeof(ctrlC)/sizeof(char));
        //this->copyClipboard();
    }
    else if (keyEvent->matches(QKeySequence::Paste))
    {
        this->pasteClipboard();
    }
    else
    {
        //send all key to serial
        const char *t=  qPrintable(keyEvent->text());
        this->sendData(t,keyEvent->text().length());
    }
}

void SerialTerm::apply_setting()
{
    mSettings->beginGroup(mComName);
    serial->setPortName(mSettings->value("name").toString());
    serial->setBaudRate(mSettings->value("baudRate").toInt());
    serial->setDataBits(mSettings->value("dataBits").value<QSerialPort::DataBits>());
    serial->setParity(mSettings->value("parity").value<QSerialPort::Parity>());
    serial->setStopBits(mSettings->value("stopBits").value<QSerialPort::StopBits>());
    serial->setFlowControl(mSettings->value("flowControl").value<QSerialPort::FlowControl>());

    this->setScrollBarPosition(ScrollBarPosition::ScrollBarRight);
    //color theme
    this->setColorScheme(mSettings->value("colorScheme").toString());
    QFont f;
    f.setFamily(mSettings->value("fontFamily").toString());
    //f.setFamily("Noto Sans CJK TC");
    f.setPointSize(mSettings->value("fontSize").toInt());
    //f.setBold(true);
    this->setTerminalFont(f);

    this->setEnabled(true);
    //this->setLocalEchoEnabled(mSettings->value("localEchoEnabled").toBool());
    //console
    //this->setMaximumBlockCount(mSettings->value("maxBlockCount").toInt());
    //this->setScrollToBottom(mSettings->value("scrollToBottom").toBool());

    //log
    this->setLogDatetime(mSettings->value("logDateTime").toBool());
    this->setLogEnable(mSettings->value("logEnable").toBool());
    this->setLogFilename(mSettings->value("logFilename").toString());
    mSettings->endGroup();
}

void SerialTerm::setLogDatetime(bool bLogDatetime)
{
    _bLogDatetime = bLogDatetime;
}

void SerialTerm::setLogEnable(bool bLogEnable)
{
    _bLogEnable = bLogEnable;
}

void SerialTerm::setLogFilename(const QString &filename)
{
    _sLogFilename = filename;
}

bool SerialTerm::isOpen()
{
    return serial->isOpen();
}

QString SerialTerm::get_name()
{
    return mComName;
}

QString SerialTerm::get_status()
{
    if (isOpen())
    {
        mSettings->beginGroup(mComName);
        QString res = tr("Connected to %1 : %2, %3, %4, %5, %6")
                .arg(mSettings->value("name").toString())
                .arg(mSettings->value("baudRate").toString())
                .arg(mSettings->value("dataBits").toString())
                .arg(mSettings->value("parity").toString())
                .arg(mSettings->value("stopBits").toString())
                .arg(mSettings->value("flowControl").toString());
        mSettings->endGroup();
        return res;
    }
    return "";
}

void SerialTerm::setPortName(const QString &name)
{
    serial->setPortName(name);
}

void SerialTerm::setBaudRate(qint32 baudRate)
{
    serial->setBaudRate(baudRate);
}

void SerialTerm::setDataBits(QSerialPort::DataBits dataBits)
{
    serial->setDataBits(dataBits);
}

void SerialTerm::setParity(QSerialPort::Parity parity)
{
    serial->setParity(parity);
}

void SerialTerm::setStopBits(QSerialPort::StopBits stopBits)
{
    serial->setStopBits(stopBits);
}

void SerialTerm::setFlowControl(QSerialPort::FlowControl flowControl)
{
    serial->setFlowControl(flowControl);
}

bool SerialTerm::openSerialPort()
{
    //apply_setting();
    if (serial->open(QIODevice::ReadWrite))
    {
        emit sig_updateActionBtnStatus(true);
        emit sig_updateStatus(get_status());
        return true;
    }
    else
    {
        return false;
    }
}

void SerialTerm::closeSerialPort()
{
    if (isOpen())
        serial->close();
    this->setEnabled(false);
    emit sig_updateActionBtnStatus(false);
    emit sig_updateStatus(tr("Disconnected"));
}

//write line of data to serial
void SerialTerm::s_write(const QByteArray &data)
{
    writeDataToSerial(data);
}

//write line of data to serial
void SerialTerm::writeln(const QByteArray &data)
{
    writeDataToSerial(data);
    writeDataToSerial("\r\n");
}

QString SerialTerm::getCurrentDateTimeString()
{
    QDateTime dateTime = dateTime.currentDateTime();
    QString dateTimeString = QString("\n[%1] ").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss"));
    return dateTimeString;
}

QString SerialTerm::getLogFileName()
{
    return _sLogFilename;
}

void SerialTerm::logToFile(QByteArray log)
{
    QString myString(log);
    logToFile(myString);
}

void SerialTerm::logToFile(const QString &lineToBelogged)
{
    //   QMutexLocker locker(&m_lineLoggerMutex);

    QFile f(getLogFileName());
    //doRollLogsIfNeeded(static_cast<qint64>(f.size() + lineToBelogged.length()));

    // Do not open in append mode but seek() to avoid warning for unseekable
    // devices, note that if open is made with WriteOnly without Append, the
    // file gets truncated
    if (!f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream out(stdout);
        out << "CANNOT OPEN LOG FILE: " << getLogFileName();
        return;
    }
    // seek() does nothing on sequential devices, this is in essence what QFile
    // does when Append flag is set in open() but without warning (on Qt 4.8.3)
    // However, Qt 4.8.1 issues the warning, so check it explicitly
    if (!f.isSequential())
    {
        f.seek(f.size());
    }

    QTextStream out(&f);
    out << lineToBelogged;
}

void SerialTerm::setScrollToBottom(bool bScrollToBottom)
{
    _bScrollToBottom = bScrollToBottom;
}

bool SerialTerm::getScrollToBottom()
{
    return _bScrollToBottom;
}

//macro relative functions
void SerialTerm::macroStart()
{
    macroStop();
    macroworker->requestWork();
    /*
    if (!macrothread->isRunning())
    {
        //macrothread->reset();
        //macrothread->start();
    }
    */
}

void SerialTerm::macroStop()
{
    if (macroworker->isRunning())
    {
        // To avoid having two threads running simultaneously, the previous thread is aborted.
        macroworker->abort();
        macrothread->wait(); // If the thread is not running, this will immediately return.
    }
}

bool SerialTerm::isMacroRunning()
{
    return macroworker->isRunning();
}

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

void SerialTerm::activateUrl(const QUrl &url,
                             bool fromContextMenu)
{
    /*TODO:   activateUrl
    if (QApplication::keyboardModifiers() & Qt::ControlModifier || fromContextMenu)
    {
        QDesktopServices::openUrl(url);
    }
    */
    Q_UNUSED(url);
    Q_UNUSED(fromContextMenu);
}
