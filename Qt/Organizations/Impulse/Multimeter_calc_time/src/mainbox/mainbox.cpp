/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QComboBox>
#include <QDialog>
#include <QDebug>
#include <QTime>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    multimeter = new Multimeter_V786_2(this);

#ifdef FAKE
    createFakeBar();
#endif

    ui->sb_serno->setMinimum(1);
    ui->sb_serno->setMaximum(9999);

    ui->sb_year->setMinimum(2000);
    ui->sb_year->setMaximum(2020);

    ui->sb_address->setMinimum(1);
    ui->sb_address->setMaximum(255);

    ui->sb_channel->setMinimum(1);
    ui->sb_channel->setMaximum(15);

    ui->sb_repeat->setMinimum(1);
    ui->sb_repeat->setMaximum(100);

    ui->btn_find_multimeter->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    connect(ui->btn_find_multimeter, SIGNAL(clicked()), this, SLOT(search_devices()));
    connect(ui->btn_check_time, SIGNAL(clicked()), this, SLOT(measuring()));

    block_interface(true);
    ui->btn_find_multimeter->setEnabled(true);

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::search_devices(void)
{
    bool ok = false;

    emit info(tr("Поиск устройства"));

    QString port_name;
    ok = multimeter->find_device(&port_name);
    if(!ok) emit error("USB приемо-передатчик мультиметра В7-86/2 НЕ найден");

    if(ok)
    {
        block_interface(false);
    }
    emit info(tr("Поиск устройства закончен"));
}
//--------------------------------------------------------------------------------
void MainBox::measuring(void)
{
    if(multimeter->port_is_open() == false)
    {
        emit error("Порт мультиметра В7-86 не открыт!");
        return;
    }

    block_interface(true);
    
    multimeter->set_limit(0);
    multimeter->set_auto_limit(true);
    multimeter->set_deviation(0);
    multimeter->set_retention(0);

    multimeter->set_serno(ui->sb_serno->value());
    multimeter->set_year(ui->sb_year->value());
    multimeter->set_address(ui->sb_address->value());
    multimeter->set_channel(ui->sb_channel->value());
    multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
    multimeter->set_silence(true);

    bool ok = multimeter->remote_control_on();
    if(!ok)
    {
        multimeter->print_last_error();
        block_interface(false);
        return;
    }

    int time_full = 0;
    int time_radiochannel = 0;
    int time_measuring = 0;

    int a_time_full = 0;
    int a_time_radiochannel = 0;
    int a_time_measuring = 0;

    double value = 0;

    QTime full_time;
    QTime radiochannel_time;

    ui->lcd_time_full->display("---");
    ui->lcd_time_radiochannel->display("---");
    ui->lcd_time_measuring->display("---");

    int repeat = ui->sb_repeat->value();
    QString temp = ui->btn_check_time->text();
    for(int i=0; i<repeat; i++)
    {
        ui->btn_check_time->setText(QString("%1 >> %2").arg(i).arg(repeat));
        full_time.start();
        ok = multimeter->measuring_UDC(&value);
        if(!ok)
        {
            multimeter->print_last_error();
            break;
        }
        time_full = full_time.elapsed();

        radiochannel_time.start();
        ok = multimeter->get_state();
        if(!ok)
        {
            multimeter->print_last_error();
            break;
        }
        time_radiochannel = radiochannel_time.elapsed();

        time_measuring = time_full - time_radiochannel;

        a_time_full += time_full;
        a_time_radiochannel += time_radiochannel;
        a_time_measuring += time_measuring;
        time_measuring = time_full - time_radiochannel;
    }
    ui->btn_check_time->setText(temp);
    a_time_full /= repeat;
    a_time_radiochannel /= repeat;
    a_time_measuring /= repeat;

    ui->lcd_time_full->display(a_time_full);
    ui->lcd_time_radiochannel->display(a_time_radiochannel);
    ui->lcd_time_measuring->display(a_time_measuring);

    multimeter->remote_control_off();
    block_interface(false);
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void MainBox::createFakeBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *fakeBar = new QToolBar("fakebar");
    fakeBar->setObjectName("fakebar");
    mw->addToolBar(Qt::TopToolBarArea, fakeBar);

    QToolButton *btn_fake = add_button(fakeBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "fake",
                                       "fake");

    connect(btn_fake, SIGNAL(pressed()), this, SLOT(fake()));
}
#endif
//--------------------------------------------------------------------------------
#ifdef FAKE
void MainBox::fake(void)
{
    QSpinBox *serno = new QSpinBox();
    QSpinBox *year  = new QSpinBox();
    QSpinBox *address = new QSpinBox();
    QSpinBox *channel = new QSpinBox();

    serno->setMinimum(1);
    serno->setMaximum(9999);
    serno->setValue(multimeter->get_serno());

    year->setMinimum(2000);
    year->setMaximum(2020);
    year->setValue(multimeter->get_year());

    address->setMinimum(1);
    address->setMaximum(255);
    address->setValue(multimeter->get_address());

    channel->setMinimum(1);
    channel->setMaximum(15);
    channel->setValue(multimeter->get_channel());

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(new QLabel("serno"), 0, 0);
    grid->addWidget(serno, 0, 1);
    grid->addWidget(new QLabel("year"), 1, 0);
    grid->addWidget(year,  1, 1);
    grid->addWidget(new QLabel("address"), 2, 0);
    grid->addWidget(address, 2, 1);
    grid->addWidget(new QLabel("channel"), 3, 0);
    grid->addWidget(channel, 3, 1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(grid);
    vbox->addWidget(buttonBox);

    QDialog *dlg = new QDialog();
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));

    dlg->setLayout(vbox);
    dlg->setFixedSize(dlg->sizeHint());
    int btn = dlg->exec();
    if(btn != QDialog::Accepted) return;

#ifdef QT_DEBUG
    emit debug(QString("serno %1").arg(serno->value()));
    emit debug(QString("year %1").arg(year->value()));
    emit debug(QString("address %1").arg(address->value()));
    emit debug(QString("channel %1").arg(channel->value()));
#endif

    multimeter->set_fake_serno(serno->value());
    multimeter->set_fake_year(year->value());
    multimeter->set_fake_address(address->value());
    multimeter->set_fake_channel(channel->value());
}
#endif
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
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
void MainBox::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    qDebug() << "MainBox::closeEvent";
}
//--------------------------------------------------------------------------------
