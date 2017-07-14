//--------------------------------------------------------------------------------
#ifndef TESTER_HPP
#define TESTER_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QWidget>
#include <QList>
//--------------------------------------------------------------------------------
enum {
    NO_ERROR_ADV = 0,
    ERROR_FILE_NOT_OPEN,
    ERROR_FILE_SIZE
};
//--------------------------------------------------------------------------------
class QByteArray;
class QString;
class QWidget;
class QTimer;
//--------------------------------------------------------------------------------
class Tester : public QWidget
{
    Q_OBJECT
public:
    explicit Tester(QWidget *parent = 0);
    void mega_test(void);
    void send_next(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void output(const QByteArray &);
    
public slots:

private slots:
    void log(const QString &data);
    void tick(void);

private:
    QWidget *parent;
    QList<QByteArray> send_data;
    int index;
    QTimer *timer;

    void connect_log(void);
    void init(void);
    void init_timer(void);
    int load(const QString &filename);
    
};
//--------------------------------------------------------------------------------
#endif // TESTER_HPP
