#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "argc" << argc;
    for(int n=0; n<argc; n++)
    {
        qDebug() << "[" << argv[n] << "]";
    }

    return a.exec();
}
