/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "ipv4_wo_port.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
IPV4_wo_port::IPV4_wo_port(QWidget *parent) :
    MyWidget(parent)
{
    a = new QSpinBox(this);
    b = new QSpinBox(this);
    c = new QSpinBox(this);
    d = new QSpinBox(this);

    a->setObjectName("IPV4_wo_port_a");
    b->setObjectName("IPV4_wo_port_b");
    c->setObjectName("IPV4_wo_port_c");
    d->setObjectName("IPV4_wo_port_d");

    a->setRange(0, 0xFF);
    b->setRange(0, 0xFF);
    c->setRange(0, 0xFF);
    d->setRange(0, 0xFF);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(0);
    hbox->setSpacing(0);
    hbox->addWidget(a);
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(b);
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(c);
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(d);
    setLayout(hbox);

    connect(a,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(b,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(c,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(d,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
IPV4_wo_port::~IPV4_wo_port()
{

}
//--------------------------------------------------------------------------------
void IPV4_wo_port::setObjectName(const QString &name)
{
    a->setObjectName(QString("%1_IPV4_wo_port_a").arg(name));
    b->setObjectName(QString("%1_IPV4_wo_port_b").arg(name));
    c->setObjectName(QString("%1_IPV4_wo_port_c").arg(name));
    d->setObjectName(QString("%1_IPV4_wo_port_d").arg(name));

    QWidget::setObjectName(name);
}
//--------------------------------------------------------------------------------
void IPV4_wo_port::work(void)
{
    emit s_get_url(get_url());
}
//--------------------------------------------------------------------------------
QUrl IPV4_wo_port::get_url(void)
{
    QUrl url;
    url.setHost(QString("%1.%2.%3.%4")
                .arg(a->value())
                .arg(b->value())
                .arg(c->value())
                .arg(d->value()));
    url.setPort(DEFAULT_IPV4_PORT);

    return url;
}
//--------------------------------------------------------------------------------
void IPV4_wo_port::set_url(QUrl url)
{
    QString host = url.host();
    QStringList sl = host.split(".");
    if(sl.count() == 4)
    {
        a->setValue(QString(sl[0]).toInt());
        b->setValue(QString(sl[1]).toInt());
        c->setValue(QString(sl[2]).toInt());
        d->setValue(QString(sl[3]).toInt());
    }
    else
    {
        emit error(QString("ip.count = %1").arg(sl.count()));
    }
}
//--------------------------------------------------------------------------------
void IPV4_wo_port::setReadOnly(bool state)
{
    a->setReadOnly(state);
    b->setReadOnly(state);
    c->setReadOnly(state);
    d->setReadOnly(state);
}
//--------------------------------------------------------------------------------
bool IPV4_wo_port::no_exit(void)
{
    return false;
}
//--------------------------------------------------------------------------------
void IPV4_wo_port::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool IPV4_wo_port::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void IPV4_wo_port::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void IPV4_wo_port::save_setting(void)
{

}
//--------------------------------------------------------------------------------
