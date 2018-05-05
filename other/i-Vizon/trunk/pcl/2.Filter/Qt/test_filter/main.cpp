#include "filterwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FilterWindow w;
    w.show();

    return a.exec();
}
