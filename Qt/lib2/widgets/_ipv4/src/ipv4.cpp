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
#include "ipv4.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
IPV4::IPV4(QWidget *parent) :
    MyWidget(parent)
{
    a = new QSpinBox(this);
    b = new QSpinBox(this);
    c = new QSpinBox(this);
    d = new QSpinBox(this);

    a->setObjectName("ipv4_a");
    b->setObjectName("ipv4_b");
    c->setObjectName("ipv4_c");
    d->setObjectName("ipv4_d");

    port = new QSpinBox(this);

    port->setObjectName("ipv4_port");

    a->setRange(0, 0xFF);
    b->setRange(0, 0xFF);
    c->setRange(0, 0xFF);
    d->setRange(0, 0xFF);

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
    hbox->addWidget(new QLabel(":"));
    hbox->addWidget(port);
    setLayout(hbox);

    load_widgets("IPV4");

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
IPV4::~IPV4()
{
    save_widgets("IPV4");
}
//--------------------------------------------------------------------------------
QUrl IPV4::get_url(void)
{
    QUrl url;
    url.setHost(QString("%1.%2.%3.%4")
                .arg(a->value())
                .arg(b->value())
                .arg(c->value())
                .arg(d->value()));
    url.setPort(port->value());

    return url;
}
//--------------------------------------------------------------------------------
void IPV4::set_url(QUrl url)
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
    port->setValue(url.port());
}
//--------------------------------------------------------------------------------
void IPV4::updateText(void)
{

}
//--------------------------------------------------------------------------------
