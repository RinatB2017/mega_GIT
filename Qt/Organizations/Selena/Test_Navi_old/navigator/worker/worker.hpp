//--------------------------------------------------------------------------------
#ifndef WORKER_HPP
#define WORKER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QQueue>
//--------------------------------------------------------------------------------
class Proto_NMEA_0183;
class QWidget;
//--------------------------------------------------------------------------------
class SerialBox;
class Receiver;
class Tester;
//--------------------------------------------------------------------------------
class Worker : public QWidget
{
    Q_OBJECT
public:
    explicit Worker(QWidget *parent = 0, SerialBox *serial = 0);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void output_latitude_string(const QString &data);
    void output_longitude_string(const QString &data);
    void output_observation(const QString &data);

public slots:
    void input_raw_data(QByteArray data);

private slots:
    void log(const QString &data);

private:
    QWidget *parent;
    SerialBox *serial;

    Proto_NMEA_0183 *proto;

    Receiver *receiver;
#ifdef TEST_NAVI
    Tester *tester;
#endif

    QQueue<unsigned char> queue;
    QByteArray clear_data;

    void connect_log(void);

    void init(void);
    void init_protocol(void);
    void init_tester(void);
    
    void input_clear_data(const QByteArray &data);
};
//--------------------------------------------------------------------------------
#endif // WORKER_HPP
