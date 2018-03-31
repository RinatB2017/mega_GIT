#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <QWidget>
#include <QObject>
#include <QDebug>

class MyClass : public QObject
{
    Q_OBJECT

public slots:
    void cppSlot(const QString &msg)
    {
        qDebug() << "cppSlot:" << msg;
    }
};

#endif
