#include <QCoreApplication>
#include <QDebug>

#include "main.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    C *test = new C();
    qDebug() << test->get_x();

    return a.exec();
}
