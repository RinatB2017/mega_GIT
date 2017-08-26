#include <QApplication>
#include "test.hpp"

#include <QMessageBox>
#include <QDebug>

#include "mainwindow.h"
#include "paintarea.h"

MyApplication::MyApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
}

bool MyApplication::notify(QObject* receiver, QEvent* event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (PaintArea::UninitializedData in)
    {
        QMessageBox::critical(0, "Error", QString("%1").arg(in.description));
    }
    catch (int x)
    {
        QMessageBox::critical(0, "Error", QString("%1").arg(x));
    }
    catch(MainWindow::ExError e)
    {
        QMessageBox::critical(0, "Error", QString("ExError: %1").arg(e.info));
    }
    catch(...)
    {
        QMessageBox::critical(0, "Error", "...");
    }

    return false;
}
