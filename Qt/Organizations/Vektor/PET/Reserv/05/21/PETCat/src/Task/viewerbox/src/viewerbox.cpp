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
#include <QProgressDialog>
#include <QInputDialog>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>

#include <QPicture>
#include <QPixmap>
#include <QImage>

#include <QtMath>
#include <QDebug>

#include <QFileInfo>
#include <QSpinBox>
#include <QLabel>
#include <QFile>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
//--------------------------------------------------------------------------------
#include "ui_viewerbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "viewerbox.hpp"
#include "defines.hpp"
#include "viewer.hpp"
//--------------------------------------------------------------------------------
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
ViewerBox::ViewerBox(QString file_auto_open, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ViewerBox)
{
    init();

    load_QDoubleSpinBox("ViewerBox");
    load_QSpinBox("ViewerBox");
    load_QComboBox("ViewerBox");

    if(cb_metadata == nullptr)
    {
        return;
    }

    //TODO костыль
    if(file_auto_open.isEmpty() == false)
    {
        //emit debug(QString("viewer [%1]").arg(file_auto_open));
        for(int n=0; n<cb_metadata->count(); n++)
        {
            cb_metadata->setCurrentIndex(n);
            if(cb_metadata->currentText() == file_auto_open)
            {
                break;
            }
        }
        show_event_dat3();
    }
}
//--------------------------------------------------------------------------------
ViewerBox::~ViewerBox()
{
    save_QDoubleSpinBox("ViewerBox");
    save_QSpinBox("ViewerBox");
    save_QComboBox("ViewerBox");

    delete ui;
}
//--------------------------------------------------------------------------------
void ViewerBox::init(void)
{
    ui->setupUi(this);

    init_widgets();
}
//--------------------------------------------------------------------------------
void ViewerBox::init_widgets(void)
{
    cb_metadata = new MyComboBox(this);
    ui->metadata_layout->addWidget(cb_metadata);
    refresh_metadata();

    //---
    QStringList sl;
    sl << "Источник события";
    sl << "Флаги события";
    sl << "Метка времени - целая часть";
    sl << "Метка времени - дробная часть";
    sl << "X+";
    sl << "X-";
    sl << "Y+";
    sl << "Y-";
    //---
    ui->tw_viewer->setColumnCount(8);
    ui->tw_viewer->setHorizontalHeaderLabels(sl);
    //---
    ui->tw_viewer->resizeColumnsToContents();
    //---
    ui->sb_num_event->setRange(0, INT32_MAX);
    ui->sb_count_event->setRange(1, 100);
    //---
    connect(ui->btn_show_event,     SIGNAL(clicked(bool)),  this,   SLOT(show_event_dat3()));
    //connect(ui->btn_show_second,    SIGNAL(clicked(bool)),  this,   SLOT(show_second()));
}
//--------------------------------------------------------------------------------
void ViewerBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void ViewerBox::show_second(void)
{
    QString filename;
    QString temp_str;
    convert_metadata(cb_metadata->currentText(),
                     &filename);

    QFile *file = new QFile(filename);
    if(!file)
    {
        emit debug("error new QFile");
        return;
    }
    if(!file->exists())
    {
        temp_str = QString("Файл %1 не найден!").arg(filename);
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }
    if(!file->open(QIODevice::ReadOnly))
    {
        temp_str = QString("Файл %1 не открыт!").arg(filename);
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }
    //---
    QFileInfo fi(filename);
    //---
    l_event_src.clear();
    qint64 max_num_events = fi.size() / sizeof(struct pet_event);
    emit info(QString("Общее количество событий %1").arg(max_num_events));
    //---
    pet_event_t event;
    //---
    QProgressDialog progress("Подсчет количества секунд", "Отмена", 0, max_num_events, this);
    progress.setFixedWidth(MESSAGEBOX_WIDTH);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    int inc = max_num_events / 100;
    int index = 0;
    progress.setValue(index);
    for(int n=0; n<max_num_events; n++)
    {
        if((index % inc) == 0)
        {
            QCoreApplication::processEvents();
            progress.setValue(index);
        }
        if (progress.wasCanceled())
        {
            file->close();

            temp_str = "Анализ данных прерван";
            emit error(temp_str);
            MyWidget::messagebox_info("Информация", temp_str);
            return;
        }
        index ++;

        quint64 bytes = file->read((char *)&event, sizeof(event));
        if(bytes != sizeof(event))
        {
            emit error("Ошибка чтения файла");
            file->close();
            return;
        }
        add_src(event.src, event.ts);
    }
    progress.setValue(max_num_events);
    //---
    file->close();
    //---
    QString temp;
    foreach (event_src_t e_src, l_event_src)
    {
        temp.append(QString("Источник события %1 количество секунд %2\n")
                    .arg(e_src.src)
                    .arg(e_src.cnt_ts));
    }
    //---
    messagebox_info("Информация", temp);
    emit info(temp);
}
//--------------------------------------------------------------------------------
void ViewerBox::add_src(uint8_t src, quint32 ts)
{
    for(int index = 0; index < l_event_src.count(); index++)
    {
        if(l_event_src[index].src == src)
        {
            if(ts < l_event_src[index].old_ts)
            {
                l_event_src[index].cnt_ts++;
            }
            l_event_src[index].old_ts = ts;
            return;
        }
    }

    event_src_t e_src;
    e_src.src = src;
    e_src.old_ts = ts;
    e_src.cnt_ts = 1;

    l_event_src.append(e_src);
}
//--------------------------------------------------------------------------------
void ViewerBox::show_event(int pos, pet_event event)
{
    QTableWidgetItem *item_src = new QTableWidgetItem();
    QTableWidgetItem *item_flags = new QTableWidgetItem();
    QTableWidgetItem *item_ts = new QTableWidgetItem();
    QTableWidgetItem *item_ts_fract = new QTableWidgetItem();
    QTableWidgetItem *item_xp = new QTableWidgetItem();
    QTableWidgetItem *item_xn = new QTableWidgetItem();
    QTableWidgetItem *item_yp = new QTableWidgetItem();
    QTableWidgetItem *item_yn = new QTableWidgetItem();

    item_src->setText(QString("%1").arg(event.src));
    QString temp = QString("%1").arg(event.flags, 2, 16, QChar('0')).toUpper();
    item_flags->setText(QString("0x%1").arg(temp));
    item_ts->setText(QString("%1").arg(event.ts));
    item_ts_fract->setText(QString("%1").arg(event.ts_fract));
    item_xp->setText(QString("%1").arg(event.xp));
    item_xn->setText(QString("%1").arg(event.xn));
    item_yp->setText(QString("%1").arg(event.yp));
    item_yn->setText(QString("%1").arg(event.yn));

    ui->tw_viewer->setItem(pos, 0, item_src);
    ui->tw_viewer->setItem(pos, 1, item_flags);
    ui->tw_viewer->setItem(pos, 2, item_ts);
    ui->tw_viewer->setItem(pos, 3, item_ts_fract);
    ui->tw_viewer->setItem(pos, 4, item_xp);
    ui->tw_viewer->setItem(pos, 5, item_xn);
    ui->tw_viewer->setItem(pos, 6, item_yp);
    ui->tw_viewer->setItem(pos, 7, item_yn);
}
//--------------------------------------------------------------------------------
void ViewerBox::show_event_dat3(void)
{
    QString filename;
    QString temp_str;
    convert_metadata(cb_metadata->currentText(),
                     &filename);

    QFile *file = new QFile(filename);
    if(!file)
    {
        emit debug("error new QFile");
        return;
    }
    if(!file->exists())
    {
        temp_str = QString("Файл %1 не найден!").arg(filename);
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }
    if(!file->open(QIODevice::ReadOnly))
    {
        temp_str = QString("Файл %1 не открыт!").arg(filename);
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }

    qint64 pos = ui->sb_num_event->value() * sizeof(struct pet_event);
    bool ok = file->seek(pos);
    if(!ok)
    {
        temp_str = "Неверный номер события";
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }

    pet_event_t event;
    //---
    ui->tw_viewer->setRowCount(ui->sb_count_event->value());    //TODO
    for(int n=0; n<ui->sb_count_event->value(); n++)
    {
        QByteArray ba = file->read(sizeof(struct pet_event));
        if(ba.size() != sizeof(struct pet_event))
        {
            temp_str = "Ошибка чтения файла";
            emit error(temp_str);
            messagebox_critical("Ошибка", temp_str);
            return;
        }
        try
        {
            memcpy((char *)&event, ba.data(), sizeof(pet_event_t));
            show_event(n, event);
        }
        catch(...)
        {
            return;
        }
    }
    //---
    ui->tw_viewer->resizeColumnsToContents();
    ui->tw_viewer->verticalHeader()->setDefaultSectionSize ( ui->tw_viewer->verticalHeader()->minimumSectionSize () );
    //---
}
//--------------------------------------------------------------------------------
bool ViewerBox::convert_metadata(QString name, QString *result)
{
    MetaData *md = new MetaData();
    connect(md, SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(md, SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    QStringList sl;
    bool ok = false;
    QFileInfo fi;
    sl.clear();
    fi.setFile(name);
    ok = md->find_data(fi.baseName(), &sl);
    if(ok)
    {
        emit debug("-----");
        foreach (QString name, sl)
        {
            *result = name;
            disconnect(md);
            md->deleteLater();
            return true;
        }
        emit debug("-----");
    }
    disconnect(md);
    md->deleteLater();
    return false;
}
//--------------------------------------------------------------------------------
void ViewerBox::refresh_metadata(void)
{
    MetaData *md = new MetaData();
    connect(md, SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(md, SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));
    md->load_data(cb_metadata);

    disconnect(md);
    md->deleteLater();
}
//--------------------------------------------------------------------------------
