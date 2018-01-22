//#include "sbglass.h"
#include <www.h>
#include <sbcontrol.h>
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    www testWidget;
    SBControl control(&testWidget);
    testWidget.show();
    control.show();
    return app.exec();
}
