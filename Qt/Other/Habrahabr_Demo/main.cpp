#include "MainWindow.h"
#include <QApplication>

#include "PresenterComPort.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    PresenterComPort *presenterComPort = new PresenterComPort();
    presenterComPort->appendView(&w);

    w.show();

    return a.exec();
}
