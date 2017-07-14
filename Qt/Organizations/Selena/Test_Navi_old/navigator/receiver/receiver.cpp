//--------------------------------------------------------------------------------
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "serialbox.hpp"
#include "receiver.hpp"
//--------------------------------------------------------------------------------
Receiver::Receiver(QWidget *parent, SerialBox *serial) :
    QWidget(parent),
    parent(parent),
    serial(serial)
{
    init();
}
//--------------------------------------------------------------------------------
void Receiver::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void Receiver::log(const QString &data)
{
    qDebug() << "Receiver::log" << data;
}
//--------------------------------------------------------------------------------
void Receiver::init(void)
{
    connect_log();

    connect(serial, SIGNAL(output(QByteArray)), this, SIGNAL(output(QByteArray)));
}
//--------------------------------------------------------------------------------
void Receiver::input(const QByteArray &data)
{
    if(serial) serial->input(data);
    // emit output(data);
}
//--------------------------------------------------------------------------------
