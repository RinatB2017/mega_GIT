//--------------------------------------------------------------------------------
#ifndef RECEIVER_HPP
#define RECEIVER_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QWidget>
//--------------------------------------------------------------------------------
class SerialBox;
//--------------------------------------------------------------------------------
class Receiver : public QWidget
{
    Q_OBJECT
public:
    explicit Receiver(QWidget *parent = 0, SerialBox *serial = 0);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void output(const QByteArray &);
    
public slots:
    void input(const QByteArray &data);

private slots:
    void log(const QString &data);

private:
    QWidget *parent;
    SerialBox *serial;

    void connect_log(void);
    void init(void);
    
};
//--------------------------------------------------------------------------------
#endif // TESTER_HPP
