#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Hello, world!";

    QTimer::singleShot(1000, &a, SLOT(quit()));

    return a.exec();
}
