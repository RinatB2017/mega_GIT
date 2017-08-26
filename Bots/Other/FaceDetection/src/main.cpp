//--------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------
#include <QApplication>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "cmainwindow.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CMainWindow w;
    //w.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //w.setWindowFlags(Qt::Dialog |
    //                 Qt::WindowTitleHint |
    //                 Qt::WindowSystemMenuHint);
    w.show();

    return a.exec();
}
//--------------------------------------------------------------------------------
