#ifndef SERIALTERM_HPP
#define SERIALTERM_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QWidget>
#include <QSerialPort>
#include <qtermwidget6/qtermwidget.h>
#include <QSettings>
#include <QThread>
#include <QDebug>

#include "macroWorker.h"
//--------------------------------------------------------------------------------
class SerialTerm : public QTermWidget
{
    Q_OBJECT
public:
    explicit SerialTerm(QWidget *parent = nullptr);
    virtual ~SerialTerm();

    void show_data(QByteArray data);

    void setScrollToBottom(bool bScrollToBottom);
    bool getScrollToBottom();
    //macro
    void macroStart();
    void macroStop();
    bool isMacroRunning();
    Qt::HANDLE getMacroThreadId();

signals:
    void sig_updateStatus(QString sMsg);
    void sig_updateActionBtnStatus(bool bStatus);
    void sig_DataReady(const QByteArray &data);

private slots:
    void activateUrl(const QUrl &url, bool fromContextMenu);
    void slot_handleError(QSerialPort::SerialPortError error);

private:
    QString getCurrentDateTimeString();
private:
    QByteArray lastSend;

    bool _bScrollToBottom;
    //macro
    /**
     * @brief Thread object which will let us manipulate the running thread
     */
    QThread *macrothread = nullptr;
    /**
     * @brief Object which contains methods that should be runned in another thread
     */
    macroWorker *macroworker = nullptr;
};
//--------------------------------------------------------------------------------
#endif // SERIALTERM_HPP
