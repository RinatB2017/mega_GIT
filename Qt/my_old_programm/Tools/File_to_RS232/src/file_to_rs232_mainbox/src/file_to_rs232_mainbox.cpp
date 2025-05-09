/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include "qwt_knob.h"
#include "ui_file_to_rs232_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "file_to_rs232_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(timer) delete timer;

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createSerial();
    createTimer();

    connect(ui->btn_load, SIGNAL(clicked()), this, SLOT(load_data()));
}
//--------------------------------------------------------------------------------
void MainBox::load_data(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter(tr("log files (*.log)"));
    dlg->setDefaultSuffix(tr("log"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        load_file(files.at(0));
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void MainBox::power_timer(bool state)
{
    if(state)
    {
        timer->start(ui->knob_Interval->value());
        ui->btnPower->setText(tr("stop"));
    }
    else
    {
        timer->stop();
        ui->btnPower->setText(tr("start"));
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTimer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));

    connect(ui->btnPower, SIGNAL(clicked(bool)), this, SLOT(power_timer(bool)));

    ui->knob_Interval->setUpperBound(1000);
    ui->knob_Interval->setScaleMaxMajor(10);
    ui->knob_Interval->setScaleMaxMinor(10);

    ui->knob_Interval->setValue(1000);

    connect(ui->knob_Interval, SIGNAL(valueChanged(double)), this, SLOT(change_timer_interval(double)));
}
//--------------------------------------------------------------------------------
void MainBox::createSerial(void)
{
    ui->serial_widget->set_caption("RS-232");
    connect(this,   SIGNAL(send_data(QString)), ui->serial_widget,  SLOT(input(QString)));
}
//--------------------------------------------------------------------------------
void MainBox::tick(void)
{
    QByteArray ba;
    QString temp;

    if(ui->listWidget->count() == 0) return;
    if(index > ui->listWidget->count()) index = 0;
    else index++;
    ui->listWidget->setCurrentRow(index);
    temp = ui->listWidget->currentItem()->text();
    emit debug(temp);

    ba.clear();
    ba.append(temp.toLatin1());
    switch(ui->cb_append->currentIndex())
    {
    case 0:
        break;

    case 1:
        ba.append((char)0x00);
        break;

    case 2:
        ba.append((char)0x0D);
        break;

    case 3:
        ba.append((char)0x0A);
        break;

    case 4:
        ba.append((char)0x0D);
        ba.append((char)0x0A);
        break;
    }
    emit send_data(ba);
}
//--------------------------------------------------------------------------------
void MainBox::change_timer_interval(double value)
{
    if(timer->isActive())
    {
        timer->stop();
        timer->start(value);
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_file(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit error(QString(tr("file %1 not open").arg(filename)));
        return;
    }
    QByteArray data = file.readAll();
    QList<QByteArray> temp_data = data.split(0x0A);

    emit debug(QString("load %1 string").arg(temp_data.count()));
    ui->listWidget->clear();
    foreach (QByteArray ba, temp_data)
    {
        ui->listWidget->addItem(ba);
    }
    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    int index;

    ui->retranslateUi(this);

    ui->btnPower->setText(ui->btnPower->isChecked() ? tr("stop") : tr("start"));

    index = ui->cb_append->currentIndex();
    if(index < 0) index = 0;
    ui->cb_append->clear();
    ui->cb_append->addItems(QStringList()
                            << "no add"
                            << "0x00"
                            << "0x0D"
                            << "0x0A"
                            << "0x0D 0x0A" );
    ui->cb_append->setCurrentIndex(index);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    if(ui->serial_widget->isOpen())
    {
        messagebox_critical("Ошибка", "Сначала закройте порт");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
