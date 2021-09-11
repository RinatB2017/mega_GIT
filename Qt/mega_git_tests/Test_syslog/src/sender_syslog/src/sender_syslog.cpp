/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "sender_syslog.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Sender_syslog::Sender_syslog(const QString &title,
                             int default_src,
                             int default_level,
                             QWidget *parent)
    : QToolBar(title, parent),
      def_src(default_src),
      def_level(default_level)
{
    setObjectName(title);
    init();;
}
//--------------------------------------------------------------------------------
Sender_syslog::~Sender_syslog()
{

}
//--------------------------------------------------------------------------------
void Sender_syslog::init(void)
{
    cb_level = new QComboBox();
    cb_level->addItem("LOG_EMERG",      LOG_EMERG);
    cb_level->addItem("LOG_ALERT",      LOG_ALERT);
    cb_level->addItem("LOG_CRIT",       LOG_CRIT);
    cb_level->addItem("LOG_ERR",        LOG_ERR);
    cb_level->addItem("LOG_WARNING",    LOG_WARNING);
    cb_level->addItem("LOG_NOTICE",     LOG_NOTICE);
    cb_level->addItem("LOG_INFO",       LOG_INFO);
    cb_level->addItem("LOG_DEBUG",      LOG_DEBUG);

    for(int n=0; n<cb_level->count(); n++)
    {
        int lvl = cb_level->itemData(n, Qt::UserRole).toInt();
        if(lvl == def_level)
        {
            cb_level->setCurrentIndex(n);
        }
    }

    btn_start = new QPushButton();
    btn_stop = new QPushButton();

    btn_start->setText("start");
    btn_stop->setText("stop");

    sb_src = new QSpinBox();
    sb_src->setValue(def_src);

    sb_interval = new QSpinBox();
    sb_interval->setRange(100, 10000);

    le_message = new QLineEdit();
    le_message->setText(objectName());

    addWidget(new QLabel("level"));
    addWidget(cb_level);
    addWidget(new QLabel("src"));
    addWidget(sb_src);
    addWidget(new QLabel("interval, ms"));
    addWidget(sb_interval);
    addWidget(new QLabel("message"));
    addWidget(le_message);
    addWidget(btn_start);
    addWidget(btn_stop);

    connect(btn_start,  SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));

    init_timer();
}
//--------------------------------------------------------------------------------
void Sender_syslog::init_timer(void)
{
    timer = new QTimer(this);
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(update()));
}
//--------------------------------------------------------------------------------
void Sender_syslog::start(void)
{
    if(timer)
    {
        timer->start(sb_interval->value());
#ifdef QT_DEBUG
        qDebug() << "start";
#endif
    }
}
//--------------------------------------------------------------------------------
void Sender_syslog::stop(void)
{
    if(timer)
    {
        timer->stop();
#ifdef QT_DEBUG
        qDebug() << "stop";
#endif
    }
}
//--------------------------------------------------------------------------------
void Sender_syslog::update(void)
{
    int lvl = cb_level->itemData(cb_level->currentIndex(), Qt::UserRole).toInt();
    int src = sb_src->value();
    QString message = le_message->text();

    emit syslog(QDateTime::currentDateTime(),
                lvl,
                src,
                message);
}
//--------------------------------------------------------------------------------
