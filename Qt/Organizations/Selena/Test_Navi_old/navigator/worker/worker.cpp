//--------------------------------------------------------------------------------
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "proto_NMEA_0183.hpp"
//--------------------------------------------------------------------------------
#include "serialbox.hpp"
#include "receiver.hpp"
#include "worker.hpp"
#include "tester.hpp"
//--------------------------------------------------------------------------------
Worker::Worker(QWidget *parent, SerialBox *serial) :
    QWidget(parent),
    parent(parent),
    serial(serial),
    proto(0)
{
    init();
}
//--------------------------------------------------------------------------------
void Worker::connect_log(void)
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
void Worker::log(const QString &data)
{
    qDebug() << "Worker::log" << data;
}
//--------------------------------------------------------------------------------
void Worker::input_raw_data(QByteArray data)
{
    int n;
    bool found;

    for(n=0; n<data.length(); n++)
        queue.enqueue(data.at(n));
    found = false;
    while(queue.isEmpty() == false)
    {
        if(queue.value(0) == '$')
        {
            found = true;
            break;
        }
        else
        {
            queue.removeAt(0);
        }
    }
    if(found == false)
        return;

    found = false;
    for(n=0; n<queue.size(); n++)
    {
        if(queue.value(n) == '\n')
        {
            found = true;
            break;
        }
    }
    if(found == false)
        return;

    clear_data.clear();
    while(queue.isEmpty() == false)
    {
        clear_data.append(queue.at(0));
        if(queue.value(0) == '\n')
        {
            queue.clear();
            break;
        }
        queue.removeAt(0);
    }
    input_clear_data(clear_data.data());
}
//--------------------------------------------------------------------------------
void Worker::input_clear_data(const QByteArray &data)
{
    // emit info(data);
    int err = proto->check_message(data.data());
    if(err != E_NO_ERROR)
    {
        proto->print_error(data, err);
        return;
    }
#if 0
    // proto->print_variable();
    // proto->test_cheksum();
    emit info(QString("%1")
               .arg(proto->get_latitude_string()));
    emit info(QString("%1")
               .arg(proto->get_longitude_string()));
    emit info(QString("%1:%2:%3")
               .arg(proto->get_observation_hour())
               .arg(proto->get_observation_min())
               .arg(proto->get_observation_sec()));
#endif
}
//--------------------------------------------------------------------------------
void Worker::init(void)
{
    connect_log();
    init_protocol();
    init_tester();
}
//--------------------------------------------------------------------------------
void Worker::init_tester(void)
{
    receiver = new Receiver(this, serial);
    connect(receiver, SIGNAL(output(QByteArray)), this, SLOT(input_raw_data(QByteArray)));

#ifdef TEST_NAVI
    tester = new Tester(this);
    connect(tester, SIGNAL(output(QByteArray)), receiver, SLOT(input(QByteArray)));
#endif
}
//--------------------------------------------------------------------------------
void Worker::init_protocol(void)
{
    emit info("init protocol NMEA-0183");
    proto = new Proto_NMEA_0183(this);

    connect(proto, SIGNAL(output_latitude_string(QString)), this, SIGNAL(output_latitude_string(QString)));
    connect(proto, SIGNAL(output_longitude_string(QString)), this, SIGNAL(output_longitude_string(QString)));
    connect(proto, SIGNAL(output_observation(QString)), this, SIGNAL(output_observation(QString)));
}
//--------------------------------------------------------------------------------
