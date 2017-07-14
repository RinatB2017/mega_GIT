//--------------------------------------------------------------------------------
#include <QString>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QFile>
//--------------------------------------------------------------------------------
#include "tester.hpp"
//--------------------------------------------------------------------------------
#define TIME_IN_MSEC 300
//--------------------------------------------------------------------------------
Tester::Tester(QWidget *parent) :
    QWidget(parent),
    parent(parent),
    index(0),
    timer(0)
{
    init();
}
//--------------------------------------------------------------------------------
void Tester::connect_log(void)
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
void Tester::log(const QString &data)
{
    qDebug() << "Tester::log" << data;
}
//--------------------------------------------------------------------------------
void Tester::init(void)
{
    connect_log();
    load(":/navi_full.log");
    init_timer();
}
//--------------------------------------------------------------------------------
void Tester::init_timer(void)
{
    if(send_data.isEmpty())
    {
        emit error("no data");
        return;
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(TIME_IN_MSEC);
}
//--------------------------------------------------------------------------------
void Tester::tick(void)
{
    if(index < send_data.count()) index++;
    else index = 0;
    emit output(send_data.at(index));
}
//--------------------------------------------------------------------------------
void Tester::send_next(void)
{
    if(index < send_data.count()) index++;
    else index = 0;
    emit output(send_data.at(index));
}
//--------------------------------------------------------------------------------
int Tester::load(const QString &filename)
{
    QList<QByteArray> temp_data;
    QByteArray data;
    QByteArray append_data;

    temp_data.clear();
    send_data.clear();
    emit info(QString("load(%1)").arg(filename));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit error("ERROR_FILE_NOT_OPEN");
        return ERROR_FILE_NOT_OPEN;
    }
    data = file.readAll();
    temp_data = data.split(0x0A);
    emit info(QString("count %1").arg(temp_data.count()));
    foreach(data, temp_data)
    {
        append_data.clear();
        append_data.append(data);
        append_data.append((char)0x0D);
        append_data.append((char)0x0A);
        send_data.append(append_data);
        // emit debug(QString("%1").arg(temp.data()));
    }

    return NO_ERROR_ADV;
}
//--------------------------------------------------------------------------------
void Tester::mega_test(void)
{
    if(send_data.isEmpty())
    {
        emit error("no data");
        return;
    }
    emit info(QString("count %1").arg(send_data.count()));
    emit debug(send_data.at(0).toHex());
}
//--------------------------------------------------------------------------------
