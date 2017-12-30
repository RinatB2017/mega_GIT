#include "mainwindow.h"
#include "repraptor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Set some props in order to fetch and save QSettings
    QCoreApplication::setOrganizationName("NeoTheFox");
    QCoreApplication::setOrganizationDomain("https://github.com/NeoTheFox");
    QCoreApplication::setApplicationName("RepRaptor");
    QCoreApplication::setApplicationVersion(REPRAPTOR_VERSION);

#if 1
    //Light
    QPalette palette;

#ifdef Q_OS_LINUX
    palette.setBrush(QPalette::WindowText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor((QRgb)0x404040), Qt::SolidPattern));
#endif

#ifdef Q_OS_WIN
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(240, 240, 240),   Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(240, 240, 240),   Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(105, 105, 105),   Qt::SolidPattern));
#endif

    qApp->setPalette(palette);
#endif

    MainWindow w;
    w.show();

    return a.exec();
}
