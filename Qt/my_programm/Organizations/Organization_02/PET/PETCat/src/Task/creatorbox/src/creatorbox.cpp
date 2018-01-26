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

#include <QFileInfo>
#include <QSpinBox>
#include <QLabel>
#include <QFile>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
//--------------------------------------------------------------------------------
#include "ui_creatorbox.h"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mainwindow.hpp"
#include "creatorbox.hpp"
#include "defines.hpp"
#include "viewer.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
CreatorBox::CreatorBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::CreatorBox)
{
    init();

    load_QDoubleSpinBox("CreatorBox");
    load_QSpinBox("CreatorBox");
    load_QComboBox("CreatorBox");
}
//--------------------------------------------------------------------------------
CreatorBox::~CreatorBox()
{
    save_QDoubleSpinBox("CreatorBox");
    save_QSpinBox("CreatorBox");
    save_QComboBox("CreatorBox");

    delete ui;
}
//--------------------------------------------------------------------------------
void CreatorBox::init(void)
{
    ui->setupUi(this);

    init_widgets();

    block_wheel();
}
//--------------------------------------------------------------------------------
void CreatorBox::init_widgets(void)
{
    ui->sb_num_events->setRange(1e3, 1e9);
    ui->sb_num_events->setValue(1e6);
    //---
    ui->sb_channel_A->setRange(0, 0xFF);
    ui->sb_channel_B->setRange(0, 0xFF);
    ui->sb_max_delta->setRange(0, 1e6);
    //---
    ui->sb_PET_EVENT_FLAG_FAULT->setRange(0, 100);
    ui->sb_PET_EVENT_FLAG_TS_INVALID->setRange(0, 100);
    ui->sb_PET_EVENT_FLAG_GATE_OVERLAY->setRange(0, 100);
    ui->sb_PET_EVENT_FLAG_QDC_OVERFLOW->setRange(0, 100);
    ui->sb_PET_EVENT_FLAG_QDC_UNDERFLOW->setRange(0, 100);
    ui->sb_PET_EVENT_FLAG_ADC_OVERFLOW->setRange(0, 100);
    ui->sb_PET_EVENT_FLAG_ADC_UNDERFLOW->setRange(0, 100);
    ui->sb_PET_EVENT_FLAG_SYNC->setRange(0, 100);
    //---
    connect(ui->btn_create_dat_file, SIGNAL(clicked(bool)), this, SLOT(btn_create_dat_file_click()));
}
//--------------------------------------------------------------------------------
void CreatorBox::btn_create_dat_file_click(void)
{
    create_dat3_file();
}
//--------------------------------------------------------------------------------
void CreatorBox::changeEvent(QEvent *event)
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
void CreatorBox::create_dat3_file(void)
{
    QString filename;

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "cb_dlg");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("DAT3 (*.dat3)");
    dlg->setDefaultSuffix(tr("dat3"));
    dlg->selectFile("без имени");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    delete dlg;

    if(filename.isEmpty())
    {
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        emit error(QString("Файл %1 не открыт!").arg(filename));
        return;
    }

    pet_event event;
    Viewer *v = new Viewer();

    int brg = PET_ADDR_BRG_MIN;
    double xx = 0;
    double yy = 0;
    quint64 ts = 0;
    uint8_t flags = 0;
    int max_point = MAX_POINT;
    float temp = 0xFFFF / (max_point + 1);

    int max_n = ui->sb_num_events->value();
    QProgressDialog progress("Создание файла", "Отмена", 0, max_n);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    int inc = max_n / 100;
    if(inc == 0) inc = 1;

    uint8_t channel_A = ui->sb_channel_A->value();
    uint8_t channel_B = ui->sb_channel_B->value();
    int max_delta = ui->sb_max_delta->value();
    bool current_channel_A = false;

    //---
    int max_PET_EVENT_FLAG_FAULT            = (int)(ui->sb_PET_EVENT_FLAG_FAULT->value() * max_n / 100.0 + 0.5);
    int max_PET_EVENT_FLAG_TS_INVALID       = (int)(ui->sb_PET_EVENT_FLAG_TS_INVALID->value() * max_n / 100.0 + 0.5);
    int max_PET_EVENT_FLAG_GATE_OVERLAY     = (int)(ui->sb_PET_EVENT_FLAG_GATE_OVERLAY->value() * max_n / 100.0 + 0.5);
    int max_PET_EVENT_FLAG_QDC_OVERFLOW     = (int)(ui->sb_PET_EVENT_FLAG_QDC_OVERFLOW->value() * max_n / 100.0 + 0.5);
    int max_PET_EVENT_FLAG_QDC_UNDERFLOW    = (int)(ui->sb_PET_EVENT_FLAG_QDC_UNDERFLOW->value() * max_n / 100.0 + 0.5);
    int max_PET_EVENT_FLAG_ADC_OVERFLOW     = (int)(ui->sb_PET_EVENT_FLAG_ADC_OVERFLOW->value() * max_n / 100.0 + 0.5);
    int max_PET_EVENT_FLAG_ADC_UNDERFLOW    = (int)(ui->sb_PET_EVENT_FLAG_ADC_UNDERFLOW->value() * max_n / 100.0 + 0.5);
    int max_PET_EVENT_FLAG_SYNC             = (int)(ui->sb_PET_EVENT_FLAG_SYNC->value() * max_n / 100.0 + 0.5);
    //---
    emit debug(QString("max_n %1").arg(max_n));
    emit debug(QString("max_PET_EVENT_FLAG_FAULT %1").arg(max_PET_EVENT_FLAG_FAULT));
    emit debug(QString("max_PET_EVENT_FLAG_TS_INVALID %1").arg(max_PET_EVENT_FLAG_TS_INVALID));
    emit debug(QString("max_PET_EVENT_FLAG_GATE_OVERLAY %1").arg(max_PET_EVENT_FLAG_GATE_OVERLAY));
    emit debug(QString("max_PET_EVENT_FLAG_QDC_OVERFLOW %1").arg(max_PET_EVENT_FLAG_QDC_OVERFLOW));
    emit debug(QString("max_PET_EVENT_FLAG_QDC_UNDERFLOW %1").arg(max_PET_EVENT_FLAG_QDC_UNDERFLOW));
    emit debug(QString("max_PET_EVENT_FLAG_ADC_OVERFLOW %1").arg(max_PET_EVENT_FLAG_ADC_OVERFLOW));
    emit debug(QString("max_PET_EVENT_FLAG_ADC_UNDERFLOW %1").arg(max_PET_EVENT_FLAG_ADC_UNDERFLOW));
    emit debug(QString("max_PET_EVENT_FLAG_SYNC %1").arg(max_PET_EVENT_FLAG_SYNC));
    //---

    for(int n=0; n<max_n; n++)
    {
        //---
        if((n % inc) == 0)
        {
            QCoreApplication::processEvents();
            progress.setValue(n);
        }
        if (progress.wasCanceled())
        {
            messagebox_info("Информация", "Создание файла прервано");
            file.close();
            return;
        }
        //---
        if(ui->rb_circle->isChecked())
        {
            xx = 0x7FFF + double((double)0x5000)*qCos(double(n)*double(M_PI)/double(180.0));
            yy = 0x7FFF + double((double)0x5000)*qSin(double(n)*double(M_PI)/double(180.0));
            xx += ((rand() % 0x5000) - 0x2500);
            yy += ((rand() % 0x1000) - 0x500);

            event = v->get_point(brg, flags, xx, yy, ts);
        }
        //---
        if(ui->rb_grid->isChecked())
        {
            int x = (rand() % max_point);
            int y = (rand() % max_point);

#if 1
            int xx = (x * temp) + (temp / 2.0);
            int yy = (y * temp) + (temp / 2.0);
#else
            int xx = (x * temp);
            int yy = (y * temp);
#endif

            event = v->get_point(brg, flags, xx, yy, ts);
        }
        //---
        //TODO
        event.flags = 0;
        if(n < max_PET_EVENT_FLAG_FAULT)         event.flags |= PET_EVENT_FLAG_FAULT;
        if(n < max_PET_EVENT_FLAG_TS_INVALID)    event.flags |= PET_EVENT_FLAG_TS_INVALID;
        if(n < max_PET_EVENT_FLAG_GATE_OVERLAY)  event.flags |= PET_EVENT_FLAG_GATE_OVERLAY;
        if(n < max_PET_EVENT_FLAG_QDC_OVERFLOW)  event.flags |= PET_EVENT_FLAG_QDC_OVERFLOW;
        if(n < max_PET_EVENT_FLAG_QDC_UNDERFLOW) event.flags |= PET_EVENT_FLAG_QDC_UNDERFLOW;
        if(n < max_PET_EVENT_FLAG_ADC_OVERFLOW)  event.flags |= PET_EVENT_FLAG_ADC_OVERFLOW;
        if(n < max_PET_EVENT_FLAG_ADC_UNDERFLOW) event.flags |= PET_EVENT_FLAG_ADC_UNDERFLOW;
        if(n < max_PET_EVENT_FLAG_SYNC)          event.flags |= PET_EVENT_FLAG_SYNC;

        if(max_delta == 0)
        {
            event.ts = 0;
        }
        else
        {
            event.ts = rand() % max_delta;
        }
        current_channel_A = !current_channel_A;
        if(current_channel_A)
        {
            event.src = channel_A;
        }
        else
        {
            event.src = channel_B;
        }

        //---
        qint64 bytes = file.write((char *)&event, sizeof(struct pet_event));
        if(bytes != sizeof(struct pet_event))
        {
            emit error("error write");
            break;
        }
        ts++;
    }
    progress.setValue(max_n);
    file.close();

    QString temp_str = QString("Файл %1 успешно создан").arg(filename);
    emit info(temp_str);
    messagebox_info("Создание файла", temp_str);
}
//--------------------------------------------------------------------------------
