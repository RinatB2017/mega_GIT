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
#include <QTreeWidgetItem>
#include <QProgressDialog>
#include <QTreeWidget>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QToolBar>
#include <QPixmap>
#include <QTime>
#include <QMenu>

#include <QCursor>
#include <QMenu>
#include <QAction>

#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "b588_powersupply.hpp"
#include "b588_packets.hpp"

#include "b588_toolbar.hpp"
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "b588.hpp"
#include "ui_b588.h"
#include "crc.h"
#include "defines.hpp"
#include "B588_fram.hpp"
//--------------------------------------------------------------------------------
#include "qhexedit.h"
//--------------------------------------------------------------------------------
B588::B588(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B588)
{
    init();
}
//--------------------------------------------------------------------------------
B588::~B588()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void B588::create_hex_views(void)
{
    he = new QHexEdit(this);

    he_user_U = new QHexEdit(this);
    he_user_I = new QHexEdit(this);

    he_factory_U = new QHexEdit(this);
    he_factory_I = new QHexEdit(this);

    he->setToolTip("he");
    he_user_U->setToolTip("user_U");
    he_user_I->setToolTip("user_I");
    he_factory_U->setToolTip("factory_U");
    he_factory_I->setToolTip("factory_I");

    QHBoxLayout *hbox_user = new QHBoxLayout();
    hbox_user->addWidget(he_user_U);
    hbox_user->addWidget(he_user_I);

    QHBoxLayout *hbox_factory = new QHBoxLayout();
    hbox_factory->addWidget(he_factory_U);
    hbox_factory->addWidget(he_factory_I);

    connect(he, SIGNAL(dataChanged()), this, SLOT(dataChanged()));

    he_user_U->setContextMenuPolicy(Qt::CustomContextMenu);
    he_user_I->setContextMenuPolicy(Qt::CustomContextMenu);
    he_factory_U->setContextMenuPolicy(Qt::CustomContextMenu);
    he_factory_I->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(he_user_U, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popup_user_U(QPoint)));
    connect(he_user_I, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popup_user_I(QPoint)));
    connect(he_factory_U, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popup_factory_U(QPoint)));
    connect(he_factory_I, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popup_factory_I(QPoint)));

    ui->hex_layout->addLayout(hbox_user);
    ui->hex_layout->addLayout(hbox_factory);
    ui->hex_layout->addWidget(he);
}
//--------------------------------------------------------------------------------
void B588::popup_user_U(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *calc_crc16_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                             QObject::tr("calc CRC16(user_U)"),
                                             this);
    calc_crc16_action->setObjectName("popup_user_U");

    popup_menu->addAction(calc_crc16_action);
    connect(calc_crc16_action,  SIGNAL(triggered()), this, SLOT(calc_crc16_user_U()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void B588::popup_user_I(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *calc_crc16_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                             QObject::tr("calc CRC16(user_I)"),
                                             this);
    calc_crc16_action->setObjectName("popup_user_I");

    popup_menu->addAction(calc_crc16_action);
    connect(calc_crc16_action,  SIGNAL(triggered()), this, SLOT(calc_crc16_user_I()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void B588::popup_factory_U(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *calc_crc16_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                             QObject::tr("calc CRC16(factory_U)"),
                                             this);
    calc_crc16_action->setObjectName("popup_factory_U");

    popup_menu->addAction(calc_crc16_action);
    connect(calc_crc16_action,  SIGNAL(triggered()), this, SLOT(calc_crc16_factory_U()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void B588::popup_factory_I(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *calc_crc16_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                             QObject::tr("calc CRC16(factory_I)"),
                                             this);
    calc_crc16_action->setObjectName("popup_factory_I");

    popup_menu->addAction(calc_crc16_action);
    connect(calc_crc16_action,  SIGNAL(triggered()), this, SLOT(calc_crc16_factory_I()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void B588::calc_crc16_user_U(void)
{
#if 0
    uint8_t data[0xFFFF] = { 0 };
    uint16_t len = he_user_U->data()->length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_user_U->data()->at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(user_U) 0x%1").arg(crc16, 0, 16));

    for(int n=0; n<len; n++)
    {
        emit info(QString("0x%1").arg(data[n], 0, 16));
    }
#endif
}
//--------------------------------------------------------------------------------
void B588::calc_crc16_user_I(void)
{
#if 0
    uint8_t data[1024] = { 0 };
    uint16_t len = he_user_I->data()->length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_user_I->data()->at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(user_I) 0x%1").arg(crc16, 0, 16));
#endif
}
//--------------------------------------------------------------------------------
void B588::calc_crc16_factory_U(void)
{
#if 0
    uint8_t data[1024] = { 0 };
    uint16_t len = he_factory_U->data()->length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_factory_U->data()->at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(factory_U) 0x%1").arg(crc16, 0, 16));
#endif
}
//--------------------------------------------------------------------------------
void B588::calc_crc16_factory_I(void)
{
#if 0
    uint8_t data[1024] = { 0 };
    uint16_t len = he_factory_I->data()->length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_factory_I->data()->at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(factory_I) 0x%1").arg(crc16, 0, 16));
#endif
}
//--------------------------------------------------------------------------------
void B588::init(void)
{
    ui->setupUi(this);

    powersupply = new Powersupply_B588();

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        connect(powersupply, SIGNAL(info(QString)),    mw, SIGNAL(info(QString)));
        connect(powersupply, SIGNAL(debug(QString)),   mw, SIGNAL(debug(QString)));
        connect(powersupply, SIGNAL(error(QString)),   mw, SIGNAL(error(QString)));
        connect(powersupply, SIGNAL(trace(QString)), mw, SIGNAL(trace(QString)));
    }

    ui->btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->sb_address->setMinimum(1);
    ui->sb_address->setMaximum(255);

    //
    create_hex_views();
    //

    connect(ui->btn_find_devices, SIGNAL(pressed()), this, SLOT(find_devices()));

    block_interface(true);
    createTestBar();
    createPowerSupplyBar();

    ui->btn_find_devices->setDisabled(false);
    ui->cb_ignore_error->setDisabled(false);
    ui->sb_address->setDisabled(false);

    ui->treeWidget->setHeaderLabel("data");
}
//--------------------------------------------------------------------------------
void B588::dataChanged(void)
{
#if 0
    uint16_t address = he->cursorPos();
    uint8_t data = he->data().at(address);
    emit trace(QString("address 0x%1 data 0x%2")
                 .arg(address, 0, 16)
                 .arg(data, 0, 16));
    bool ok = save_data(address, data);
    if(!ok)
    {
        emit error(QString("address 0x%1 is bad!")
                   .arg(address, 0, 16));
    }
#endif
}
//--------------------------------------------------------------------------------
void B588::find_devices(void)
{
    bool ok = false;

    ui->btn_find_devices->setEnabled(false);
    ui->sb_address->setEnabled(false);
    ui->cb_ignore_error->setEnabled(false);

    emit info(tr("Поиск устройств"));

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_ignore_bad_cmd(ui->cb_ignore_error->isChecked());

    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        emit error("Источник питания Б5-88 НЕ найден");
        ui->btn_find_devices->setEnabled(true);
        ui->sb_address->setEnabled(true);
        ui->cb_ignore_error->setEnabled(true);
        return;
    }

    emit info(tr("Поиск устройств закончен"));
    block_interface(false);
}
//--------------------------------------------------------------------------------
void B588::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QPushButton *pb=qobject_cast<QPushButton *>(btn))
    {
        pb->setDisabled(state);
    }
    if (QToolButton *tb=qobject_cast<QToolButton *>(btn))
    {
        tb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void B588::block_interface(bool state)
{
    ui->btn_find_devices->setDisabled(state);
    ui->cb_ignore_error->setDisabled(state);
    ui->sb_address->setDisabled(state);
}
//--------------------------------------------------------------------------------
void B588::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar B5-88");
    testbar->setObjectName("toolbar_B588");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_read_fram = add_button(testbar,
                                            new QToolButton(this),
                                            QIcon(),
                                            "read FRAM",
                                            "read FRAM");
    QToolButton *btn_save_fram_to_file = add_button(testbar,
                                                    new QToolButton(this),
                                                    QIcon(),
                                                    "save FRAM to file",
                                                    "save FRAM to file");
    QToolButton *btn_save_data_to_fram = add_button(testbar,
                                                    new QToolButton(this),
                                                    QIcon(),
                                                    "save data to FRAM",
                                                    "save data to FRAM");
    QToolButton *btn_analize = add_button(testbar,
                                          new QToolButton(this),
                                          QIcon(),
                                          "analize",
                                          "analize");
    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       QIcon(),
                                       "test",
                                       "test");


    connect(btn_read_fram, SIGNAL(released()), this, SLOT(read_fram()));
    connect(btn_save_fram_to_file, SIGNAL(released()), this, SLOT(save_fram_to_file()));
    connect(btn_save_data_to_fram, SIGNAL(released()), this, SLOT(save_data_to_fram()));
    connect(btn_analize, SIGNAL(released()), this, SLOT(analize()));
    connect(btn_test, SIGNAL(released()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void B588::createPowerSupplyBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    B588_toolbar *psBar = new B588_toolbar(tr("powersupply B5-88"), this);
    psBar->setObjectName("ps_B588");
    mw->addToolBarBreak();
    mw->addToolBar(Qt::TopToolBarArea, psBar);

    connect(psBar, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));
}
//--------------------------------------------------------------------------------
void B588::save_fram_to_file(void)
{
    if(fram_data.isEmpty())
    {
        emit error("data is empty!");
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, tr("Save data"), ".", tr("Data Files (*.dat)"));
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(fram_data);
        file.close();
    }
}
//--------------------------------------------------------------------------------
void B588::save_data_to_fram(void)
{
    int btn = QMessageBox::question(this,
                                    "save data",
                                    tr("Сохранить данные в FRAM?"),
                                    QMessageBox::No | QMessageBox::Yes);
    if(btn != QMessageBox::Yes) return;

    block_this_button(true);
    emit info("save_data_to_fram begin");
    powersupply->set_state_silence(true);
    for(uint16_t address = 0; address < MAX_ADDRESS_FRAM_B588; address+=100)
    {
        int len = MAX_ADDRESS_FRAM_B588 - address;
        if(len > 100) len=100;
        if(len < 1) len=1;
        bool ok = powersupply->fram_write(address, len, fram_data.mid(address, len));
        if(!ok)
        {
            powersupply->print_last_error();
        }
    }
    powersupply->set_state_silence(false);

    he->setData(QHexEditData::fromMemory(fram_data));
    emit info(QString("save %1 bytes").arg(fram_data.size()));
    emit info("save_data_to_fram end");
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void B588::read_fram(void)
{
    block_this_button(true);
    emit info("read_fram begin");

    fram_data.clear();

    powersupply->set_state_silence(true);
    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, MAX_ADDRESS_FRAM_B588);
    pd->setWindowModality(Qt::WindowModal);
    pd->setFixedWidth(320);
    for(uint16_t address = 0; address < MAX_ADDRESS_FRAM_B588; address+=100)
    {
        pd->setValue(address);
        if(pd->wasCanceled())
        {
            break;
        }

        QByteArray data;
        data.clear();

        int len = MAX_ADDRESS_FRAM_B588 - address;
        if(len > 100) len=100;
        if(len < 1) len=1;
        bool ok = powersupply->fram_read(address, len, &data);
        if(!ok)
        {
            powersupply->print_last_error();
        }
        else
        {
            fram_data.append(data);
        }
    }
    pd->close();
    powersupply->set_state_silence(false);

    he->setData(QHexEditData::fromMemory(fram_data));
    emit info(QString("read %1 bytes").arg(fram_data.size()));

    uint16_t begin_user_U = offsetof(FRAM_ADDR_B588, user_points_U);
    uint16_t begin_user_I = offsetof(FRAM_ADDR_B588, user_points_I);
    uint16_t begin_factory_U = offsetof(FRAM_ADDR_B588, factory_points_U);
    uint16_t begin_factory_I = offsetof(FRAM_ADDR_B588, factory_points_I);
    uint16_t size_user_U = sizeof(FRAM_ADDR_B588::user_points_U) + 2; //+ CRC16
    uint16_t size_user_I = sizeof(FRAM_ADDR_B588::user_points_I) + 2; //+ CRC16
    uint16_t size_factory_U = sizeof(FRAM_ADDR_B588::factory_points_U) + 2; //+ CRC16
    uint16_t size_factory_I = sizeof(FRAM_ADDR_B588::factory_points_I) + 2; //+ CRC16

    QByteArray ba_user_U = fram_data.mid(begin_user_U, size_user_U);
    QByteArray ba_user_I = fram_data.mid(begin_user_I, size_user_I);
    QByteArray ba_factory_U = fram_data.mid(begin_factory_U, size_factory_U);
    QByteArray ba_factory_I = fram_data.mid(begin_factory_I, size_factory_I);

    he_user_U->setData(QHexEditData::fromMemory(ba_user_U));
    he_user_I->setData(QHexEditData::fromMemory(ba_user_I));
    he_factory_U->setData(QHexEditData::fromMemory(ba_factory_U));
    he_factory_I->setData(QHexEditData::fromMemory(ba_factory_I));

    emit info("read_fram end");
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void B588::add_user_points_U(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *user_points_U   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    user_points_U->setText(0, "user_points_U");

    QTreeWidgetItem *points_U   = new QTreeWidgetItem(user_points_U, QTreeWidgetItem::Type);
    points_U->setText(0, "points");
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_U, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->user_points_U[n]));
    }
    QTreeWidgetItem *item_crc16_U   = new QTreeWidgetItem(user_points_U, QTreeWidgetItem::Type);
    item_crc16_U->setText(0, QString("user CRC16_U %1")
                          .arg(fram->user_points_crc16_U, 0, 16));
}
//--------------------------------------------------------------------------------
void B588::add_user_points_I(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *user_points_I   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    user_points_I->setText(0, "user_points_I");

    QTreeWidgetItem *points_I   = new QTreeWidgetItem(user_points_I, QTreeWidgetItem::Type);
    points_I->setText(0, "points");
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_I, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->user_points_I[n]));
    }
    QTreeWidgetItem *item_crc16_I   = new QTreeWidgetItem(user_points_I, QTreeWidgetItem::Type);
    item_crc16_I->setText(0, QString("user CRC16_I %1")
                          .arg(fram->user_points_crc16_I, 0, 16));
}
//--------------------------------------------------------------------------------
void B588::add_factory_points_U(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *factory_points_U   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    factory_points_U->setText(0, "factory_points_U");

    QTreeWidgetItem *points_U   = new QTreeWidgetItem(factory_points_U, QTreeWidgetItem::Type);
    points_U->setText(0, "points");
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_U, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->factory_points_U[n]));
    }
    QTreeWidgetItem *item_crc16_U   = new QTreeWidgetItem(factory_points_U, QTreeWidgetItem::Type);
    item_crc16_U->setText(0, QString("factory CRC16_U %1")
                          .arg(fram->factory_points_crc16_U, 0, 16));
}
//--------------------------------------------------------------------------------
void B588::add_factory_points_I(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *factory_points_I   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    factory_points_I->setText(0, "factory_points_I");

    QTreeWidgetItem *points_I   = new QTreeWidgetItem(factory_points_I, QTreeWidgetItem::Type);
    points_I->setText(0, "points");
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_I, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->factory_points_I[n]));
    }
    QTreeWidgetItem *item_crc16_I   = new QTreeWidgetItem(factory_points_I, QTreeWidgetItem::Type);
    item_crc16_I->setText(0, QString("factory CRC16_I %1")
                          .arg(fram->factory_points_crc16_I, 0, 16));
}
//--------------------------------------------------------------------------------
void B588::add_profile_points(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *profiles   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    profiles->setText(0, "profiles");
    for(int profile=0; profile<MAX_PROFILE_B588; profile++)
    {
        QTreeWidgetItem *pr = new QTreeWidgetItem(profiles, QTreeWidgetItem::Type);
        pr->setText(0, QString("profile %1").arg(profile));
        for(int point=0; point<MAX_POINT_B588; point++)
        {
            QTreeWidgetItem *p = new QTreeWidgetItem(pr, QTreeWidgetItem::Type);
            QTreeWidgetItem *p_voltage = new QTreeWidgetItem(p, QTreeWidgetItem::Type);
            QTreeWidgetItem *p_current = new QTreeWidgetItem(p, QTreeWidgetItem::Type);
            QTreeWidgetItem *p_time = new QTreeWidgetItem(p, QTreeWidgetItem::Type);

            p->setText(0, QString("point %1").arg(point));
            p_voltage->setText(0, QString("voltage %1").arg(fram->profiles[profile].point[point].voltage));
            p_current->setText(0, QString("current %1").arg(fram->profiles[profile].point[point].current));
            p_time->setText(0, QString("time %1").arg(fram->profiles[profile].point[point].time));
        }
        QTreeWidgetItem *r = new QTreeWidgetItem(profiles, QTreeWidgetItem::Type);
        r->setText(0, QString("repeat_count %1").arg(fram->profiles[profile].repeat_count));
    }
}
//--------------------------------------------------------------------------------
void B588::add_Value_K_Zero_U(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *value_U   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    value_U->setText(0, "Value_K_Zero_U");
    QTreeWidgetItem *item_new_Value_ADC_Zero_U  = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_Delta_U = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_MIN_U   = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_MAX_U   = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    item_new_Value_ADC_Zero_U->setText(0, QString("new_Value_ADC_Zero %1").arg(fram->Value_K_Zero_U.Value_ADC_Zero));
    item_new_Value_ADC_Delta_U->setText(0, QString("new_Value_ADC_Delta %1").arg(fram->Value_K_Zero_U.Value_ADC_Delta));
    item_new_Value_DAC_MIN_U->setText(0, QString("new_Value_DAC_MIN %1").arg(fram->Value_K_Zero_U.Value_DAC_MIN));
    item_new_Value_DAC_MAX_U->setText(0, QString("new_Value_DAC_MAX %1").arg(fram->Value_K_Zero_U.Value_DAC_MAX));
}
//--------------------------------------------------------------------------------
void B588::add_Value_K_Zero_I(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *value_I   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    value_I->setText(0, "Value_K_Zero_I");
    QTreeWidgetItem *item_new_Value_ADC_Zero_I  = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_Delta_I = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_MIN_I   = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_MAX_I   = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    item_new_Value_ADC_Zero_I->setText(0, QString("new_Value_ADC_Zero %1").arg(fram->Value_K_Zero_I.Value_ADC_Zero));
    item_new_Value_ADC_Delta_I->setText(0, QString("new_Value_ADC_Delta %1").arg(fram->Value_K_Zero_I.Value_ADC_Delta));
    item_new_Value_DAC_MIN_I->setText(0, QString("new_Value_DAC_MIN %1").arg(fram->Value_K_Zero_I.Value_DAC_MIN));
    item_new_Value_DAC_MAX_I->setText(0, QString("new_Value_DAC_MAX %1").arg(fram->Value_K_Zero_I.Value_DAC_MAX));
}
//--------------------------------------------------------------------------------
void B588::add_Bright(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Bright %1")
                  .arg(fram->Bright));
}
//--------------------------------------------------------------------------------
void B588::add_Sound(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Sound %1").arg(fram->Sound));
}
//--------------------------------------------------------------------------------
void B588::add_Adr_GPIB(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Adr_GPIB %1").arg(fram->Adr_GPIB));
}
//--------------------------------------------------------------------------------
void B588::add_Count_Last_U_I(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Count_Last_U_I %1").arg(fram->Count_Last_U_I));
}
//--------------------------------------------------------------------------------
void B588::add_Save_Counts_U_I(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Save_Counts_U_I %1").arg(fram->Save_Counts_U_I));
}
//--------------------------------------------------------------------------------
void B588::add_Speed_UART(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Speed_UART %1").arg(fram->Speed_UART));
}
//--------------------------------------------------------------------------------
void B588::add_Adr_ModBus(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Adr_ModBus %1").arg(fram->Adr_ModBus));
}
//--------------------------------------------------------------------------------
void B588::add_moto_time(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("moto_time %1")
                  .arg(fram->moto_time));
}
//--------------------------------------------------------------------------------
void B588::add_Year_Factory(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Year_Factory %1")
                  .arg(fram->Year_Factory));
}
//--------------------------------------------------------------------------------
void B588::add_Month_Factory(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Month_Factory %1")
                  .arg(fram->Month_Factory));
}
//--------------------------------------------------------------------------------
void B588::add_Serial_Factory(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("Serial_Factory %1")
                  .arg(fram->Serial_Factory));
}
//--------------------------------------------------------------------------------
void B588::add_factor_k(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("factor_k %1")
                  .arg(fram->factor_k));
}
//--------------------------------------------------------------------------------
void B588::add_disable_factor_k(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("disable_factor_k %1")
                  .arg(fram->disable_factor_k));
}
//--------------------------------------------------------------------------------
void B588::add_cal_temp(void)
{
    FRAM_ADDR_B588 *fram = (FRAM_ADDR_B588 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->treeWidget, QTreeWidgetItem::Type);
    item->setText(0, QString("cal_temp %1")
                  .arg(fram->cal_temp));
}
//--------------------------------------------------------------------------------
void B588::analize(void)
{
    if(fram_data.isEmpty())
    {
        emit error("data is empty!");
        return;
    }

    ui->treeWidget->clear();

    add_user_points_U();
    add_user_points_I();

    add_factory_points_U();
    add_factory_points_I();

    add_profile_points();

    add_Value_K_Zero_U();
    add_Value_K_Zero_I();

    add_Bright();
    add_Sound();
    add_Adr_GPIB();
    add_Count_Last_U_I();
    add_Save_Counts_U_I();
    add_Speed_UART();
    add_Adr_ModBus();
    add_moto_time();
    add_Year_Factory();
    add_Month_Factory();
    add_Serial_Factory();

    add_factor_k();
    add_disable_factor_k();
    add_cal_temp();
}
//--------------------------------------------------------------------------------
bool B588::save_data(uint16_t address, uint8_t data)
{
    if(address > fram_data.size())
    {
        return false;
    }

    bool ok = powersupply->fram_write_8(address, data);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void B588::test(void)
{
    emit info("Test begin");

#if 1
    uint16_t year = 2020;
    uint8_t month = 6;
    uint16_t serno = 666;
    bool ok = powersupply->set_date_and_serno(year,month, serno);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
#endif

#if 0
    uint8_t data = 0;
    bool ok = false;
    powersupply->set_state_silence(true);
    for(int address=0; address<10; address++)
    {
        data = 0;
        ok = powersupply->fram_read_8(address, &data);
        if(!ok)
        {
            powersupply->print_last_error();
        }
        else
        {
            emit info(QString("address %1 data %2 (0x%3)")
                      .arg(address)
                      .arg(data)
                      .arg(data, 0, 16));
        }
    }
    powersupply->set_state_silence(false);
#endif

#if 0
    uint16_t data = 0;
    bool ok = false;
    powersupply->set_state_silence(true);
    for(int address=0; address<120; address+=2)
    {
        data = 0;
        ok = powersupply->fram_read_16(address, &data);
        if(!ok)
        {
            powersupply->print_last_error();
        }
        else
        {
            emit info(QString("address %1 data %2 (0x%3)")
                      .arg(address)
                      .arg(data)
                      .arg(data, 0, 16));
        }
    }
    powersupply->set_state_silence(false);
#endif

#if 0
    uint8_t ID = 0;
    bool ok = powersupply->get_ID(&ID);
    if(!ok)
    {
        powersupply->print_last_error();
    }
    else
    {
        emit info(QString("ID 0x%1").arg(ID, 0, 16));
    }
#endif

#if 0
    powersupply->test();
#endif

#if 0
    uint8_t data[61 * 2] = { 0xAA };
    uint16_t crc16 = CRC::powersupply_crc16(data, 61 * 2);
    emit info(QString("crc16 0x%1").arg(crc16, 0, 16));

#endif

#if 0
    uint16_t address = offsetof(FRAM_ADDR, Serial_Factory);
    uint16_t xxx = 0;
    xxx += sizeof(FRAM_ADDR::user_points_U);
    xxx += sizeof(FRAM_ADDR::user_points_crc16_U);
    xxx += sizeof(FRAM_ADDR::user_points_I);
    xxx += sizeof(FRAM_ADDR::user_points_crc16_I);
    xxx += sizeof(FRAM_ADDR::factory_points_U);
    xxx += sizeof(FRAM_ADDR::factory_points_crc16_U);
    xxx += sizeof(FRAM_ADDR::factory_points_I);
    xxx += sizeof(FRAM_ADDR::factory_points_crc16_I);
    xxx += sizeof(FRAM_ADDR::profiles);
    xxx += sizeof(FRAM_ADDR::Value_K_Zero_U);
    xxx += sizeof(FRAM_ADDR::Value_K_Zero_I);
    xxx += sizeof(FRAM_ADDR::Bright);
    xxx += sizeof(FRAM_ADDR::Sound);
    xxx += sizeof(FRAM_ADDR::Adr_GPIB);
    xxx += sizeof(FRAM_ADDR::Count_Last_U_I);
    xxx += sizeof(FRAM_ADDR::Save_Counts_U_I);
    xxx += sizeof(FRAM_ADDR::Speed_UART);
    xxx += sizeof(FRAM_ADDR::Adr_ModBus);
    xxx += sizeof(FRAM_ADDR::moto_time);
    xxx += sizeof(FRAM_ADDR::Year_Factory);
    xxx += sizeof(FRAM_ADDR::Month_Factory);


    emit info(QString("xxx %1").arg(xxx));
    emit info(QString("address Serial_Factory %1").arg(address));
#endif

#if 0
    bool ok = powersupply->test();
    if(!ok)
    {
        powersupply->print_last_error();
    }
#endif

#if 0
    QByteArray ba;
    for(int n=0; n<(62*2); n++) ba.append((char)0x00);

    QHexEdit *he = new QHexEdit();
    he->setData(ba);
    he->setMinimumSize(640, 480);
    he->setCursorPosition(5);
    he->setHighlightingColor(Qt::red);
    he->setHighlighting(true);

    he->show();
#endif

#if 0
    powersupply->run_dialog_0x27();
#endif

    emit info("Test end");
}
//--------------------------------------------------------------------------------
void B588::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
