//--------------------------------------------------------------------------------
#include <QDebug>
//--------------------------------------------------------------------------------
#include "proto_MNP_binary.hpp"
#include "qhexedit.h"
//--------------------------------------------------------------------------------
Proto_MNP_binary::Proto_MNP_binary(QWidget *parent) :
    QWidget(parent),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Proto_MNP_binary::connect_log(void)
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
void Proto_MNP_binary::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void Proto_MNP_binary::test(void)
{
    emit debug(QString("sizeof(data_binary) %1").arg(sizeof(data_binary)));

    QByteArray buff;
    buff.resize(sizeof(data_binary));
    memmove(buff.data(), &data_binary, sizeof(data_binary));

    view_bytearray(buff);
}
//--------------------------------------------------------------------------------
void Proto_MNP_binary::init(void)
{
    connect_log();

    data_binary.sync = 0x81FF;
    data_binary.id = 0;
    data_binary.size = 0;
    data_binary.reserv = 0;
    calc_checksum();
}
//--------------------------------------------------------------------------------
void Proto_MNP_binary::view_bytearray(const QByteArray &data)
{
    QHexEdit *hex = new QHexEdit;
    hex->setFixedSize(hex->size());
    hex->setWindowTitle(tr("data"));
    hex->setWindowIcon(QIcon(QLatin1String(":/mainwindow/computer.png")));
    hex->setReadOnly(true);
    hex->setData(data);
    hex->show();
}
//--------------------------------------------------------------------------------
bool Proto_MNP_binary::check_id(void)
{
    bool res;
    switch(data_binary.id)
    {
    case 3000:
    case 3001:
    case 3011:
    case 3002:
    case 3003:
    case 3006:
    case 2000:
    case 2200:
        res = true;
        break;

    default:
        res = false;
        break;
    }
    return res;
}
//--------------------------------------------------------------------------------
bool Proto_MNP_binary::check_checksum(void)
{
    int sum = data_binary.sync + data_binary.id + data_binary.size + data_binary.reserv + data_binary.checksum;
    if(sum != 0) return false;
    return true;
}
//--------------------------------------------------------------------------------
void Proto_MNP_binary::calc_checksum(void)
{
    int sum = data_binary.sync + data_binary.id + data_binary.size + data_binary.reserv;
    if(!sum) data_binary.checksum = 0;
    else data_binary.checksum = -sum;
}
//--------------------------------------------------------------------------------
