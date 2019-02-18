//--------------------------------------------------------------------------------
#ifndef TERMINAL_H
#define TERMINAL_H
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QObject>
#include <QDateTime>
#include <QString>
//--------------------------------------------------------------------------------
class SerialDeviceEnumerator;
class AbstractSerial;
class QByteArray;
//--------------------------------------------------------------------------------
class Terminal : public QObject
{
    Q_OBJECT

public:
    explicit Terminal(const QString &filename, 
                      QWidget *parent = nullptr);
    ~Terminal();

private:
    SerialDeviceEnumerator *enumerator;
    AbstractSerial *serial;

    QString device;
    QString baud_rate;
    QString data_bits;
    QString parity;
    QString stop_bits;
    QString flow;

    void init(const QString &filename);
    void init_serial(void);
    void read_ini(const QString &filename);

signals:
    void getdata(const QByteArray &data);

    void procCtsChanged(bool);
    void procDsrChanged(bool);
    void procRingChanged(bool);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

public slots:
    void send(const QByteArray &data);

private slots:
    void procSerialDataReceive();
    void getStatus(const QString &status, QDateTime current);

protected:

};
//--------------------------------------------------------------------------------
#endif
