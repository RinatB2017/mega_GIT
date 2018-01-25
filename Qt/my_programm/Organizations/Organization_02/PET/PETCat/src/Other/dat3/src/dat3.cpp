/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QCoreApplication>
#include <QProgressDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QFileInfo>
#include <QObject>
//--------------------------------------------------------------------------------
#include "dat3.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
DAT3::DAT3(QObject *parent) : QObject(parent)
{
    cnt_read_events = 0;
}
//--------------------------------------------------------------------------------
bool DAT3::read_file(const QString filename)
{
    QString temp_str;
    if(filename.isEmpty())
    {
        MyWidget::messagebox_critical("Ошибка", "Не задано имя файла");
        return false;
    }

    QFile file(filename);
    if(!file.exists())
    {
        temp_str = QString("Файл %1 не найден!").arg(filename);
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
        return false;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        temp_str = QString("Файл %1 не открыт!").arg(filename);
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
        return false;
    }
    QFileInfo f_info(filename);
    if(f_info.size() % sizeof(struct pet_event))
    {
        temp_str = "Неверный формат файла (ошибка размера файла)";
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
        return false;
    }
    emit debug(QString("file %1 is open").arg(filename));

    //---
    int max_index = f_info.size() / MAX_EVENTS_BUF;
    if(max_index == 0) max_index = 1;
    QProgressDialog progress("Анализ данных", "Отмена", 0, max_index);
    progress.setFixedWidth(MESSAGEBOX_WIDTH);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    int index = 0;
    progress.setValue(index);
    int inc = max_index / 100;
    if(inc == 0) inc = 1;

    cnt_read_events = 0;
    while (!file.atEnd())
    {
        if((index % inc) == 0)
        {
            QCoreApplication::processEvents();
            progress.setValue(index);
        }
        if (progress.wasCanceled())
        {
            MyWidget::messagebox_info("Информация", "Анализ данных прерван");
            return false;
        }
        index ++;
        QByteArray ba = file.read(MAX_EVENTS_BUF);
        if(ba.size() > 0)
        {
            for(int n=0; n<ba.size(); n+=sizeof(pet_event_t))
            {
                pet_event_t event;
                memcpy((char *)&event, (char *)&ba.data()[n], sizeof(pet_event_t));
                emit send_event(event);
                cnt_read_events++;
            }
        }
    }

    progress.setValue(max_index);
    file.close();
    return true;
}
//--------------------------------------------------------------------------------
quint64 DAT3::get_cnt_read_events(void)
{
    return cnt_read_events;
}
//--------------------------------------------------------------------------------
