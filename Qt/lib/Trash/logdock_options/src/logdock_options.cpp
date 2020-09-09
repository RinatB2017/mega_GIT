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
#include "logdock_options.hpp"
#include "ui_logdock_options.h"
//--------------------------------------------------------------------------------
LogDock_options::LogDock_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDock_options)
{
    init();
}
//--------------------------------------------------------------------------------
LogDock_options::~LogDock_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
bool LogDock_options::setProperty(const char *name, const QVariant &value)
{
    Q_ASSERT(property(name).isValid() == true);

    QDialog::setProperty(name, value);
    return true;
}
//--------------------------------------------------------------------------------
void LogDock_options::init(void)
{
    ui->setupUi(this);

    connect(ui->btn_ok,     &QPushButton::clicked,  this,   &LogDock_options::accept);
    connect(ui->btn_cancel, &QPushButton::clicked,  this,   &LogDock_options::reject);

    //---
#if 0
    int id = 0;
    options.append({id++, &LogDock_options::get(ui->cb_show_error), &LogDock_options::set(ui->cb_show_error)});
#endif
    //---

    findCodecs();

#ifdef QT_DEBUG
    QTimer::singleShot(0, this, &LogDock_options::show_objectname);
#endif

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void LogDock_options::findCodecs(void)
{
    codecs.clear();
    foreach (int mib, QTextCodec::availableMibs())
    {
        QTextCodec *codec = QTextCodec::codecForMib(mib);
        codecs.append(codec->name());
    }
    std::sort(codecs.begin(), codecs.end());

    ui->cb_CodecForCStrings->addItems(codecs);
}
//--------------------------------------------------------------------------------
void LogDock_options::show_objectname(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName().isEmpty() == false)
        {
            if(obj->objectName().left(3) != "qt_")
            {
                obj->setToolTip(obj->objectName());
            }
        }
    }
}
//--------------------------------------------------------------------------------
void LogDock_options::set_readonly(bool value)
{
    ui->cb_readonly->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_color(bool value)
{
    ui->cb_color->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_noCRLF(bool value)
{
    ui->cb_noCRLF->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_datetime(bool value)
{
    ui->cb_add_datetime->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_show_error(bool value)
{
    ui->cb_show_error->setChecked(value);
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_readonly(void)
{
    return ui->cb_readonly->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_color(void)
{
    return ui->cb_color->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_noCRLF(void)
{
    return ui->cb_noCRLF->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_datetime(void)
{
    return ui->cb_add_datetime->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_show_error(void)
{
    return ui->cb_show_error->isChecked();
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_EMERG(bool value)
{
    ui->cb_EMERG->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_ALERT(bool value)
{
    ui->cb_ALERT->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_CRIT(bool value)
{
    ui->cb_CRIT->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_ERR(bool value)
{
    ui->cb_ERR->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_WARNING(bool value)
{
    ui->cb_WARN->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_NOTICE(bool value)
{
    ui->cb_NOTICE->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_INFO(bool value)
{
    ui->cb_INFO->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_DEBUG(bool value)
{
    ui->cb_DEBUG->setChecked(value);
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_EMERG(void)
{
    return ui->cb_EMERG->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_ALERT(void)
{
    return ui->cb_ALERT->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_CRIT(void)
{
    return ui->cb_CRIT->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_ERR(void)
{
    return ui->cb_ERR->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_WARNING(void)
{
    return ui->cb_WARN->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_NOTICE(void)
{
    return ui->cb_NOTICE->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_INFO(void)
{
    return ui->cb_INFO->isChecked();
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_DEBUG(void)
{
    return ui->cb_DEBUG->isChecked();
}
//--------------------------------------------------------------------------------
