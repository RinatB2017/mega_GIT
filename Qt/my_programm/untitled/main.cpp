#include <QCoreApplication>
#include <QLocalSocket>
#include <QDebug>
#include <QTimer>

class Sender : public QObject
{
public:
    static void send_data(const QString &text)
    {
        QLocalSocket *socket(new QLocalSocket());
    }
    void get_data(const QString &text)
    {
        qDebug() << text;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Sender *sender = new Sender();
    sender->send_data("test");
    delete sender;

    QTimer::singleShot(0, &app, SLOT(quit()));

    return app.exec();
}
