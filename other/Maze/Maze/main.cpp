#include "mainwindow.h"
#include <QApplication>
#include <QTime>
#include <QLibraryInfo>
#include <QTranslator>

int main(int argc, char *argv[])
{
    // для рандома
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    QApplication a(argc, argv);

    // русский язык в контекстном меню
    QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(),
                QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
