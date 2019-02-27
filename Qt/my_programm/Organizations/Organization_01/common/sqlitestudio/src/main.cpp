#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("DB Studios");
    a.setApplicationName("SQLite Studio");
    a.setApplicationVersion("1.0.0");
    SQLiteBrowser w;
    w.show();
    return a.exec();
}
