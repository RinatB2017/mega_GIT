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
#include "ipv6.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
IPV6::IPV6(QWidget *parent) :
    MyWidget(parent)
{
    a = new HexSpinBox(this);
    b = new HexSpinBox(this);
    c = new HexSpinBox(this);
    d = new HexSpinBox(this);
    e = new HexSpinBox(this);
    f = new HexSpinBox(this);

    a->setObjectName("IPV6_a");
    b->setObjectName("IPV6_b");
    c->setObjectName("IPV6_c");
    d->setObjectName("IPV6_d");
    e->setObjectName("IPV6_e");
    f->setObjectName("IPV6_f");

    port = new QSpinBox(this);

    port->setObjectName("IPV6_port");

    port->setRange(0, 0xFFFF);
    port->setToolTip("Порт");

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
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(e);
    hbox->addWidget(new QLabel("."));
    hbox->addWidget(f);
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(port);
    setLayout(hbox);

    connect(a,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(b,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(c,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(d,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(e,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(f,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(port,   SIGNAL(valueChanged(int)),  this,   SLOT(work()));

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
IPV6::~IPV6()
{

}
//--------------------------------------------------------------------------------
void IPV6::setObjectName(const QString &name)
{
    a->setObjectName(QString("%1_IPV6_a").arg(name));
    b->setObjectName(QString("%1_IPV6_b").arg(name));
    c->setObjectName(QString("%1_IPV6_c").arg(name));
    d->setObjectName(QString("%1_IPV6_d").arg(name));
    e->setObjectName(QString("%1_IPV6_e").arg(name));
    f->setObjectName(QString("%1_IPV6_f").arg(name));

    MyWidget::setObjectName(name);
}
//--------------------------------------------------------------------------------
void IPV6::work(void)
{
    emit s_get_url(get_url());
}
//--------------------------------------------------------------------------------
QUrl IPV6::get_url(void)
{
    QUrl url;
    url.setHost(QString("%1.%2.%3.%4.%5.%6")
                .arg(a->value())
                .arg(b->value())
                .arg(c->value())
                .arg(d->value())
                .arg(c->value())
                .arg(d->value()));
    url.setPort(port->value());

    return url;
}
//--------------------------------------------------------------------------------
void IPV6::set_url(QUrl url)
{
    QString host = url.host();
    QStringList sl = host.split(".");
    if(sl.count() == 6)
    {
        a->setValue(QString(sl[0]).toInt());
        b->setValue(QString(sl[1]).toInt());
        c->setValue(QString(sl[2]).toInt());
        d->setValue(QString(sl[3]).toInt());
        e->setValue(QString(sl[4]).toInt());
        f->setValue(QString(sl[5]).toInt());
    }
    port->setValue(url.port());
}
//--------------------------------------------------------------------------------
bool IPV6::no_exit(void)
{
    return false;
}
//--------------------------------------------------------------------------------
void IPV6::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool IPV6::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void IPV6::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void IPV6::save_setting(void)
{

}
//--------------------------------------------------------------------------------
