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
#include "ipv6_wo_port.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
IPV6_wo_port::IPV6_wo_port(QWidget *parent) :
    MyWidget(parent)
{
    a = new HexSpinBox(this);
    b = new HexSpinBox(this);
    c = new HexSpinBox(this);
    d = new HexSpinBox(this);
    e = new HexSpinBox(this);
    f = new HexSpinBox(this);

    a->setObjectName("IPV6_wo_port_a");
    b->setObjectName("IPV6_wo_port_b");
    c->setObjectName("IPV6_wo_port_c");
    d->setObjectName("IPV6_wo_port_d");
    e->setObjectName("IPV6_wo_port_e");
    f->setObjectName("IPV6_wo_port_f");

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(0);
    hbox->setSpacing(0);
    hbox->addWidget(a);
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(b);
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(c);
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(d);
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(e);
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(f);
    setLayout(hbox);

    connect(a,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(b,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(c,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(d,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(e,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));
    connect(f,  SIGNAL(valueChanged(int)),  this,   SLOT(work()));

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
IPV6_wo_port::~IPV6_wo_port()
{

}
//--------------------------------------------------------------------------------
void IPV6_wo_port::setObjectName(const QString &name)
{
    a->setObjectName(QString("%1_IPV6_wo_port_a").arg(name));
    b->setObjectName(QString("%1_IPV6_wo_port_b").arg(name));
    c->setObjectName(QString("%1_IPV6_wo_port_c").arg(name));
    d->setObjectName(QString("%1_IPV6_wo_port_d").arg(name));
    e->setObjectName(QString("%1_IPV6_wo_port_e").arg(name));
    f->setObjectName(QString("%1_IPV6_wo_port_f").arg(name));

    MyWidget::setObjectName(name);
}
//--------------------------------------------------------------------------------
void IPV6_wo_port::work(void)
{
    emit s_get_url(get_url());
}
//--------------------------------------------------------------------------------
QUrl IPV6_wo_port::get_url(void)
{
    QUrl url;
    url.setHost(QString("%1:%2:%3:%4:%5:%6")
                .arg(a->value())
                .arg(b->value())
                .arg(c->value())
                .arg(d->value())
                .arg(e->value())
                .arg(f->value()));
    url.setPort(DEFAULT_IPV6_PORT);

    return url;
}
//--------------------------------------------------------------------------------
void IPV6_wo_port::set_url(QUrl url)
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
}
//--------------------------------------------------------------------------------
bool IPV6_wo_port::no_exit(void)
{
    return false;
}
//--------------------------------------------------------------------------------
void IPV6_wo_port::updateText(void)
{

}
//--------------------------------------------------------------------------------
