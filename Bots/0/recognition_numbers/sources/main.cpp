#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include "mainwindow.h"
#include "paintarea.h"

#include "test.hpp"

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MyApplication a(argc, argv);
    MainWindow w;
#if 1
    w.show();
    return a.exec();
#else
    try
    {
        w.show();
        return a.exec();
    }
    catch(PaintArea::UninitializedData in)
    {
        QMessageBox mes;
        mes.setText(in.description);
        mes.exec();
    }
    return a.exec();
#endif
}

