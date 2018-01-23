#include "mainwindow.h"
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/icon"));

    //Add Splash Screen
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/mariosplash.png"));
    splash->show();

    MainWindow mainWin;
    mainWin.setSize();

    QTimer::singleShot(4000,splash,SLOT(close()));
    QTimer::singleShot(4000,&mainWin,SLOT(show()));

    return a.exec();
}
