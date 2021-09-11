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
#include "ui_AD9106_box.h"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <hidapi/hidapi.h>
#endif
#ifdef Q_OS_WIN
#   include "hidapi_win/hidapi.h"
#endif
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "qxmlputget.h"
//--------------------------------------------------------------------------------
#include "AD9106_box.hpp"
#include "ad9106.hpp"
//--------------------------------------------------------------------------------
#ifdef GRAPHER
#   include "curvebox.hpp"
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
AD9106_Box::AD9106_Box(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::AD9106_Box)
{
    init();;
}
//--------------------------------------------------------------------------------
AD9106_Box::~AD9106_Box()
{
#ifdef GRAPHER
    if(curve) curve->deleteLater();
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void AD9106_Box::init(void)
{
    ui->setupUi(this);

#ifdef TEST
    createTestBar();
#endif
    init_sl_registers();

    //---
    ui->sb_DDS_TW->setRange(100, 20000000);
    //---
    ui->dsb_DAC_DGAIN_xp->setRange(0, 2.5);
    ui->dsb_DAC_DGAIN_xn->setRange(0, 2.5);
    ui->dsb_DAC_DGAIN_yp->setRange(0, 2.5);
    ui->dsb_DAC_DGAIN_yn->setRange(0, 2.5);
    //---
    //TODO
    ui->sb_DDS_TW->setValue(100000);
    ui->dsb_DAC_DGAIN_xp->setValue(2.0);
    ui->dsb_DAC_DGAIN_xn->setValue(2.0);
    ui->dsb_DAC_DGAIN_yp->setValue(2.0);
    ui->dsb_DAC_DGAIN_yn->setValue(2.0);
    //---
    ui->dsb_DAC_DOF_xp->setRange(-2.5, 2.5);
    ui->dsb_DAC_DOF_xn->setRange(-2.5, 2.5);
    ui->dsb_DAC_DOF_yp->setRange(-2.5, 2.5);
    ui->dsb_DAC_DOF_yn->setRange(-2.5, 2.5);
    //---
    ui->dsb_DDS_PW_xp->setRange(0, 180);
    ui->dsb_DDS_PW_xn->setRange(0, 180);
    ui->dsb_DDS_PW_yp->setRange(0, 180);
    ui->dsb_DDS_PW_yn->setRange(0, 180);
    //---

    //---
    ui->dsb_DAC_DGAIN_xp->setDecimals(2);
    ui->dsb_DAC_DGAIN_xn->setDecimals(2);
    ui->dsb_DAC_DGAIN_yp->setDecimals(2);
    ui->dsb_DAC_DGAIN_yn->setDecimals(2);
    //---
    ui->dsb_DAC_DOF_xp->setDecimals(2);
    ui->dsb_DAC_DOF_xn->setDecimals(2);
    ui->dsb_DAC_DOF_yp->setDecimals(2);
    ui->dsb_DAC_DOF_yn->setDecimals(2);
    //---
    ui->dsb_DDS_PW_xp->setDecimals(2);
    ui->dsb_DDS_PW_xn->setDecimals(2);
    ui->dsb_DDS_PW_yp->setDecimals(2);
    ui->dsb_DDS_PW_yn->setDecimals(2);
    //---

    //---
    ui->dsb_DAC_DGAIN_xp->setSingleStep(0.01);
    ui->dsb_DAC_DGAIN_xn->setSingleStep(0.01);
    ui->dsb_DAC_DGAIN_yp->setSingleStep(0.01);
    ui->dsb_DAC_DGAIN_yn->setSingleStep(0.01);
    //---
    ui->dsb_DAC_DOF_xp->setSingleStep(0.01);
    ui->dsb_DAC_DOF_xn->setSingleStep(0.01);
    ui->dsb_DAC_DOF_yp->setSingleStep(0.01);
    ui->dsb_DAC_DOF_yn->setSingleStep(0.01);
    //---
    ui->dsb_DDS_PW_xp->setSingleStep(0.01);
    ui->dsb_DDS_PW_xn->setSingleStep(0.01);
    ui->dsb_DDS_PW_yp->setSingleStep(0.01);
    ui->dsb_DDS_PW_yn->setSingleStep(0.01);
    //---

#if 0
    AD9106 *ad9106 = new AD9106();
    emit info(QString("len %1").arg(ad9106->get_cnt_registers()));
    ad9106->deleteLater();

    reg_t r;
    bool ok = false;
    ok = ad9106->get_reg("SPICONFIG", &r);
    if(ok)
    {
        emit info(QString("name %1").arg(r.name));
        emit info(QString("address 0x%1").arg(r.address, 0, 16));
        emit info(QString("default_value 0x%1").arg(r.default_value, 0, 16));
    }
    else
    {
        emit error("Ошибка: get_reg");
    }
#endif
    //---
    connect(ui->btn_open,       SIGNAL(clicked(bool)),  this,   SLOT(btn_open_click()));
    //connect(ui->btn_read_all,   SIGNAL(clicked(bool)),  this,   SLOT(btn_read_all_click()));
    connect(ui->btn_close,      SIGNAL(clicked(bool)),  this,   SLOT(btn_close_click()));
    //---
    connect(ui->btn_ApplySettings,  SIGNAL(clicked(bool)),  this,   SLOT(btn_ApplySettings_click()));
    connect(ui->btn_StopGeneration, SIGNAL(clicked(bool)),  this,   SLOT(btn_StopGeneration_click()));
    connect(ui->btn_ManualReset,    SIGNAL(clicked(bool)),  this,   SLOT(btn_ManualReset_click()));
    //---
    connect(ui->btn_read_xml,       SIGNAL(clicked(bool)),  this,   SLOT(btn_read_xml_click()));
    //connect(ui->btn_convert_xml,    SIGNAL(clicked(bool)),  this,   SLOT(btn_convert_xml_click()));
    //---
    connect(ui->btn_set_values,     SIGNAL(clicked(bool)),  this,   SLOT(btn_set_values_click()));
    //---

#ifdef GRAPHER
    curve = new CurveBox(1024, 0, 0xFFFF, "заголовок", "axis X", "axis Y", this);
    ui->grapher_layout->addWidget(curve);
#endif

    ui->tw_main->clear();
    read_xml();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_open_click(void)
{
     dev_open();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_read_all_click(void)
{
    dev_read_all_registers();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_close_click(void)
{
    dev_close();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_ApplySettings_click(void)
{
    ApplySettings();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_StopGeneration_click(void)
{
    StopGeneration();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_ManualReset_click(void)
{
    ManualReset();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_read_xml_click(void)
{
    read_xml();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_convert_xml_click(void)
{
    convert_xml();
}
//--------------------------------------------------------------------------------
void AD9106_Box::btn_set_values_click(void)
{
    set_values();
}
//--------------------------------------------------------------------------------
void AD9106_Box::init_sl_registers(void)
{
    sl_registers.clear();
    sl_registers << "SPICONFIG";
    sl_registers << "POWERCONFIG";
    sl_registers << "CLOCKCONFIG";
    sl_registers << "REFADJ";
    sl_registers << "DAC4AGAIN";
    sl_registers << "DAC3AGAIN";
    sl_registers << "DAC2AGAIN";
    sl_registers << "DAC1AGAIN";
    sl_registers << "DACxRANGE";
    sl_registers << "DAC4RSET";
    sl_registers << "DAC3RSET";
    sl_registers << "DAC2RSET";
    sl_registers << "DAC1RSET";
    sl_registers << "CALCONFIG";
    sl_registers << "COMPOFFSET";
    sl_registers << "RAMUPDATE";
    sl_registers << "PAT_STATUS";
    sl_registers << "PAT_TYPE";
    sl_registers << "PATTERN_DLY";
    sl_registers << "DAC4DOF";
    sl_registers << "DAC3DOF";
    sl_registers << "DAC2DOF";
    sl_registers << "DAC1DOF";
    sl_registers << "WAV4_3CONFIG";
    sl_registers << "WAV2_1CONFIG";
    sl_registers << "PAT_TIMEBASE";
    sl_registers << "PAT_PERIOD";
    sl_registers << "DAC4_3PATx";
    sl_registers << "DAC2_1PATx";
    sl_registers << "DOUT_START_DLY";
    sl_registers << "DOUT_CONFIG";
    sl_registers << "DAC4_CST";
    sl_registers << "DAC3_CST";
    sl_registers << "DAC2_CST";
    sl_registers << "DAC1_CST";
    sl_registers << "DAC4_DGAIN";
    sl_registers << "DAC3_DGAIN";
    sl_registers << "DAC2_DGAIN";
    sl_registers << "DAC1_DGAIN";
    sl_registers << "SAW4_3CONFIG";
    sl_registers << "SAW2_1CONFIG";
    sl_registers << "DDS_TW32";
    sl_registers << "DDS_TW1";
    sl_registers << "DDS4_PW";
    sl_registers << "DDS3_PW";
    sl_registers << "DDS2_PW";
    sl_registers << "DDS1_PW";
    sl_registers << "TRIG_TW_SEL";
    sl_registers << "DDSx_CONFIG";
    sl_registers << "TWRAM_CONFIG";
    sl_registers << "START_DLY4";
    sl_registers << "START_ADDR4";
    sl_registers << "STOP_ADDR4";
    sl_registers << "DDS_CYC4";
    sl_registers << "START_DLY3";
    sl_registers << "START_ADDR3";
    sl_registers << "STOP_ADDR3";
    sl_registers << "DDS_CYC3";
    sl_registers << "START_DLY2";
    sl_registers << "START_ADDR2";
    sl_registers << "STOP_ADDR2";
    sl_registers << "DDS_CYC2";
    sl_registers << "START_DLY1";
    sl_registers << "START_ADDR1";
    sl_registers << "STOP_ADDR1";
    sl_registers << "DDS_CYC1";
    sl_registers << "CFG_ERROR";
}
//--------------------------------------------------------------------------------
void AD9106_Box::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &AD9106_Box::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void AD9106_Box::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef void (AD9106_Box::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void AD9106_Box::click(bool state)
{
    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    if(!btn) return;

    if(state)
    {
        btn->setStyleSheet("background:green");
    }
    else
    {
        btn->setStyleSheet("");
    }

    bool ok = false;
    int row = btn->property("row").toInt(&ok);
    if(!ok) return;

    int bit = btn->property("bit").toInt(&ok);
    if(!ok) return;

    QWidget *w = ui->tw_main->currentWidget();
    if(!w) return;

    QList<QTableWidget *> all_obj = w->findChildren<QTableWidget *>();
    foreach(QTableWidget *obj, all_obj)
    {
        QTableWidgetItem *item = obj->item(row, 1);
        if(item)
        {
            uint16_t value = static_cast<uint16_t>(item->text().toInt(&ok));
            if(ok)
            {
                if(state)
                {
                    value |= (1 << bit);
                }
                else
                {
                    value &= ~(1 << bit);
                }
                QTableWidgetItem *value_reg  = new QTableWidgetItem(QString("%1").arg(value));
                obj->setItem(row, 1, value_reg);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void AD9106_Box::read_xml(void)
{
    QString filename = ":/doc/RegistersProfiles_new.xml";
    bool ok = false;
    QString error_message;
    int error_line = 0;
    int error_column = 0;

    emit debug(QString("open file %1").arg(filename));

    QXmlGet *get = new QXmlGet();
    ok = get->load(filename, &error_message, &error_line, &error_column);
    if(!ok)
    {
        QString temp_str = QString(tr("Файл %1 не загружен Ошибка: %2 line %3 column %4"))
                .arg(filename)
                .arg(error_message)
                .arg(error_line)
                .arg(error_column);
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }
    //---
    ui->tw_main->clear();
    //int tw_main_width = 0;
    //---
    do
    {
        ok = get->findNextAndDescend("Profile");
        if(ok)
        {
            ok = get->find("Name");
            if(ok)
            {
                QString name = get->getString();
                emit info(name);

                QPushButton *btn = new QPushButton();
                btn->setText("write registers");
                connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(dev_write_all_registers()));

                QWidget *w = new QWidget();
                QVBoxLayout *box = new QVBoxLayout;
                QTableWidget *table = new QTableWidget();
                table->setRowCount(sl_registers.count());
                table->setColumnCount(2+16);
                table->setHorizontalHeaderLabels(QStringList() << "register" << "value"
                                                 << "15" << "!4" << "13" << "12" << "11" << "10" << "09" << "08"
                                                 << "07" << "06" << "05" << "04" << "03" << "02" << "01" << "00");
                box->addWidget(table);
                box->addWidget(btn);
                w->setLayout(box);

                ok = get->findNextAndDescend("RegistersData");
                if(ok)
                {
                    int row = 0;
                    foreach (QString reg, sl_registers)
                    {
                        ok = get->find(reg);
                        if(ok)
                        {
                            QTableWidgetItem *name_reg  = new QTableWidgetItem(reg);
                            QTableWidgetItem *value_reg = new QTableWidgetItem(get->getString());

                            uint16_t value = static_cast<uint16_t>(value_reg->text().toInt(&ok));

                            int index = 2;  //TODO костыль
                            for(int n=15; n>=0; n--)
                            {
                                QToolButton *t_btn = new QToolButton();
                                t_btn->setCheckable(true);
                                t_btn->setProperty("row", row);
                                t_btn->setProperty("bit", n);

                                if(ok)
                                {
                                    bool flag = value & (1 << n);
                                    if(flag)
                                    {
                                        t_btn->setChecked(true);
                                        t_btn->setStyleSheet("background:green");
                                    }
                                }
                                connect(t_btn, SIGNAL(clicked(bool)), this, SLOT(click(bool)));
                                table->setCellWidget(row, index, t_btn);
                                index++;
                            }
                            table->setItem(row, 0, name_reg);
                            table->setItem(row, 1, value_reg);
                            row++;
                        }
                    }
                    ok = get->rise();
                    //ok = get->rise();
                }
                table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
                table->setMinimumWidth(table->sizeHint().width());
                table->setEditTriggers(QTableWidget::NoEditTriggers);
                //table->setFixedWidth(table->width());
                ui->tw_main->addTab(w, name);

                //tw_main_width = w->width();
            }
        }
    } while(ok);
    //---
//    if(tw_main_width == 0) tw_main_width = 1000;
//    ui->tw_main->setFixedWidth(tw_main_width);
    ui->tw_main->setFixedSize(sizeHint());
    //---
}
//--------------------------------------------------------------------------------
void AD9106_Box::test(void)
{
    emit info("Test()");
    bool ok = false;
    float temperature = 0;
    double voltage = 0;

    if(dev == nullptr)
    {
        emit error("handle not created");
        return;
    }

    ok = ReadTemperature(&temperature);
    if(ok)
    {
        emit info(QString("temp %1").arg(static_cast<double>(temperature)));
    }

    ok = ReadVoltage(ADC_GetVoltage::AVCC, &voltage);
    if(ok)
    {
        emit info(QString("AVCC %1").arg(voltage));
    }

    ok = ReadVoltage(ADC_GetVoltage::AVSS, &voltage);
    if(ok)
    {
        emit info(QString("AVSS %1").arg(voltage));
    }

    ok = ReadVoltage(ADC_GetVoltage::BIAS, &voltage);
    if(ok)
    {
        emit info(QString("BIAS %1").arg(voltage));
    }

    ok = ReadVoltage(ADC_GetVoltage::VCCIN, &voltage);
    if(ok)
    {
        emit info(QString("VCCIN %1").arg(voltage));
    }

    emit info("test_0: OK");
}
//--------------------------------------------------------------------------------
void AD9106_Box::dev_open(void)
{
#define MAX_STR 255

    int res;
    wchar_t wstr[MAX_STR];
    uint16_t VID = 0xc251;
    uint16_t PID = 0x2301;

    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(VID, PID);
    cur_dev = devs;
    if(cur_dev == nullptr)
    {
        messagebox_critical("Ошибка", "AD9106 не найден!");
        return;
    }
    while (cur_dev)
    {
        emit info("Device Found");
        emit info(QString("  type: %1:%2 ").arg(cur_dev->vendor_id).arg(cur_dev->product_id));
        emit info(QString("  path: %1").arg(cur_dev->path));
        emit info(QString("  serial_number: %1").arg(QString::fromWCharArray(cur_dev->serial_number)));
        emit info("");
        emit info(QString("  Manufacturer: %1").arg(QString::fromWCharArray(cur_dev->manufacturer_string)));
        emit info(QString("  Product:      %1").arg(QString::fromWCharArray(cur_dev->product_string)));
        emit info("");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    int cnt_err = 0;
    while(dev == nullptr)
    {
        dev = hid_open(VID, PID, nullptr);
        if(dev == nullptr)
        {
            cnt_err++;
        }
        if(cnt_err > 9)
        {
            emit error("hid_open not open!");
            return;
        }
    }

    // Read the Manufacturer String
    res = hid_get_manufacturer_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Manufacturer String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_manufacturer_string");
    }

    // Read the Product String
    res = hid_get_product_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Product String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_product_string");
    }

    // Read the Serial Number String
    res = hid_get_serial_number_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Serial Number String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_serial_number_string");
    }
}
//--------------------------------------------------------------------------------
void AD9106_Box::dev_close(void)
{
    if(dev != nullptr)
    {
        hid_close(dev);
        dev = nullptr;
    }
}
//--------------------------------------------------------------------------------
void AD9106_Box::dev_read_all_registers(void)
{
    //TODO пока не надо
#if 0
    if(dev == nullptr)
    {
        emit error("handle not created");
        return;
    }

    bool ok = false;
    uint16_t data = 0;

    AD9106 *ad9106 = new AD9106();
    QStringList list;
    ad9106->get_sl_name_registers(&list);
    if(list.count() < 1) return;

    ui->tw_registers->setRowCount(list.count());
    ui->tw_registers->setColumnCount(2);

    int row = 0;
    foreach (QString reg, list)
    {
        ok = AD9106_read(reg, &data);
        if(ok)
        {
            QTableWidgetItem *name_reg  = new QTableWidgetItem(QString("%1").arg(reg));
            QTableWidgetItem *value_reg = new QTableWidgetItem(QString("%1").arg(data));

            ui->tw_registers->setItem(row, 0, name_reg);
            ui->tw_registers->setItem(row, 1, value_reg);
            row++;
        }
    }

    ui->tw_registers->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tw_registers->setMinimumWidth(ui->tw_registers->sizeHint().width());

    ad9106->deleteLater();
#endif
}
//--------------------------------------------------------------------------------
void AD9106_Box::dev_write_all_registers(void)
{
    if(dev == nullptr)
    {
        emit error("handle not created");
        return;
    }

    QWidget *w = ui->tw_main->currentWidget();
    if(w == nullptr)
    {
        return;
    }

    QList<QTableWidget *> all_obj = w->findChildren<QTableWidget *>();
    foreach(QTableWidget *obj, all_obj)
    {
        for(int row=0; row<obj->rowCount(); row++)
        {
            QTableWidgetItem *item0 = obj->item(row, 0);
            QTableWidgetItem *item1 = obj->item(row, 1);

            if(item0 && item1)
            {
                bool ok = AD9106_write(item0->text(), static_cast<uint16_t>(item1->text().toInt()));
                if(!ok)
                {
                    emit error(QString("error write %1").arg(item0->text()));
                    return;
                }
            }
        }
    }
    emit info("Запись регистров: OK");
}
//--------------------------------------------------------------------------------
bool AD9106_Box::AD9106_read(QString name_reg, uint16_t *data)
{
    if(dev == nullptr)
    {
        emit error("handle not created");
        return false;
    }
    if(name_reg.isEmpty())
    {
        emit error("name register is empty");
        return false;
    }

    bool ok = false;
    reg_t r;
    AD9106 *ad9106 = new AD9106();
    ok = ad9106->get_reg(name_reg, &r);
    ad9106->deleteLater();
    if(!ok)
    {
        emit error("Register not found!");
        return false;
    }
    int res = 0;

    memset(output_buf, 0, sizeof(output_buf));
    answer_t *i_packet = reinterpret_cast<answer_t *>(output_buf);
    i_packet->cmd = 0x8f;
    i_packet->addr = r.address;
    i_packet->num = 1;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }

    memset(output_buf, 0, sizeof(output_buf));
    res = hid_get_feature_report(dev, output_buf, sizeof(output_buf));

    answer_t *o_packet = reinterpret_cast<answer_t *>(output_buf);
    *data = o_packet->data;
    return true;
}
//--------------------------------------------------------------------------------
bool AD9106_Box::AD9106_write(QString name_reg, uint16_t data)
{
    if(dev == nullptr)
    {
        emit error("handle not created");
        return false;
    }
    if(name_reg.isEmpty())
    {
        emit error("name register is empty");
        return false;
    }

    emit debug(QString("%1:%2")
               .arg(name_reg)
               .arg(data));

    bool ok = false;
    reg_t r;
    AD9106 *ad9106 = new AD9106();
    ok = ad9106->get_reg(name_reg, &r);
    ad9106->deleteLater();
    if(!ok)
    {
        emit error("Register not found!");
        return false;
    }
    int res = 0;

    memset(output_buf, 0, sizeof(output_buf));
    answer_t *i_packet = reinterpret_cast<answer_t *>(output_buf);
    i_packet->cmd = 0x85;
    i_packet->addr = r.address;
    i_packet->data = data;
    i_packet->num = 1;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool AD9106_Box::ReadTemperature(float *temperature)
{
    if(dev == nullptr)
    {
        emit error("handle not created");
        return false;
    }

    int res = 0;

    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x9A;
    output_buf[2] = 0x10;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }

    *temperature = static_cast<float>(output_buf[3] + (output_buf[4] << 8) / 100.0);
    return true;
}
//--------------------------------------------------------------------------------
bool AD9106_Box::ReadADC(uint8_t channel, uint16_t *data)
{
    if(dev == nullptr)
    {
        emit error("handle not created");
        return false;
    }

    int res = 0;

    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x9A;
    output_buf[2] = 0x20;
    output_buf[3] = channel;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return false;
    }

    *data = static_cast<uint16_t>(output_buf[4] + (output_buf[5] << 8));
    return true;
}
//--------------------------------------------------------------------------------
bool AD9106_Box::ReadVoltage(int channel, double *voltage)
{
    bool ok = false;
    uint16_t ADCData = 0;

    ok = ReadADC(static_cast<uint8_t>(channel), &ADCData);
    if(!ok) return false;

    switch(channel)
    {
    case ADC_GetVoltage::AVCC:
        *voltage = static_cast<double>(0.00226f * 2.0F * ADCData);
        break;

    case ADC_GetVoltage::AVSS:
        *voltage = static_cast<double>(0.00226f * 2.0f * ADCData);
        break;

    case ADC_GetVoltage::BIAS:
        *voltage = static_cast<double>(0.00226f * 16.0f * ADCData);
        break;

    case ADC_GetVoltage::VCCIN:
        *voltage = static_cast<double>(0.00226f * 8.0f * ADCData);
        break;

    default:
        ok = false;
        break;
    }

    return ok;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void AD9106_Box::ApplySettings(void)
{
    emit info("ApplySettings");

    //start
    if(dev == nullptr)
    {
        emit error("handle not created");
        return;
    }

    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x82;
    output_buf[2] = 0x90;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return;
    }
    emit info("Применение настроек: OK");
}
//--------------------------------------------------------------------------------
void AD9106_Box::ManualReset(void)
{
    emit info("ManualReset");

    //reset
    if(dev == nullptr)
    {
        emit error("handle not created");
        return;
    }

    int res = 0;

    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x82;
    output_buf[2] = 0x80;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return;
    }
    emit info("Сброс: OK");
}
//--------------------------------------------------------------------------------
void AD9106_Box::StopGeneration(void)
{
    emit info("StopGeneration");

    //stop
    if(dev == nullptr)
    {
        emit error("handle not created");
        return;
    }

    int res = 0;
    memset(output_buf, 0, sizeof(output_buf));

    output_buf[0] = 0x00;
    output_buf[1] = 0x82;
    output_buf[2] = 0x92;

    res = hid_send_feature_report(dev, output_buf, sizeof(output_buf));
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return;
    }
    emit info("Остановка генерации: OK");
}
//--------------------------------------------------------------------------------
void AD9106_Box::convert_xml(void)
{
    //---
    emit debug(QString("sl len = %1").arg(sl_registers.count()));
    //---

    bool ok = false;
    QString error_message;
    int error_line = 0;
    int error_column = 0;
    QString filename = "RegistersProfiles.xml";
    QString filename_new = "RegistersProfiles_new.xml";

    QXmlPut *put = new QXmlPut("ArrayOfProfile");

    QXmlGet *get = new QXmlGet();
    ok = get->load(filename, &error_message, &error_line, &error_column);
    if(!ok)
    {
        emit error(QString(tr("Файл %1 не загружен Ошибка: %2 line %3 column %4"))
                   .arg(filename)
                   .arg(error_message)
                   .arg(error_line)
                   .arg(error_column));
        return;
    }
    do
    {
        ok = get->findNextAndDescend("Profile");
        if(ok)
        {
            put->descend("Profile");
            ok = get->find("Name");
            if(ok)
            {
                QString name = get->getString();
                put->putString("Name", name);
                emit info(name);

                ok = get->findNextAndDescend("RegistersData");
                if(ok)
                {
                    int cnt = 0;
                    put->descend("RegistersData");
                    do
                    {
                        if(cnt <= sl_registers.count())
                        {
                            ok = get->findNext("unsignedShort");
                            if(ok)
                            {
                                put->putString(sl_registers.at(cnt), get->getString());
                                cnt++;
                            }
                        }
                        else
                        {
                            emit error(QString("sl count %1 != cnt %2")
                                       .arg(sl_registers.count())
                                       .arg(cnt));
                        }
                    } while(ok);
                    get->rise();
                    ok = get->rise();

                    put->rise();
                    put->rise();
                }
            }
        }
    } while(ok);

    put->save(filename_new);
    emit info("Конвертирование XML: OK");
}
//--------------------------------------------------------------------------------
void AD9106_Box::set_values(void)
{
    const float _Amplitude1mV = 7.142857142857143f;
    const float _Offset1mV = 28.0f;
    const float _Phase1deg = 11.0f;

    const uint64_t Dev_FCLK = 125000000; // тактовая частота DDS в Гц

    uint64_t DDS_TW = (uint64_t)(((float)ui->sb_DDS_TW->value()) / ((float)Dev_FCLK / (float)0x1000000));

    uint16_t DAC_DGAIN_XP = (uint16_t)(((float)ui->dsb_DAC_DGAIN_xp->value() * 1000.0f) * _Amplitude1mV);
    uint16_t DAC_DGAIN_XN = (uint16_t)(((float)ui->dsb_DAC_DGAIN_xn->value() * 1000.0f) * _Amplitude1mV);
    uint16_t DAC_DGAIN_YP = (uint16_t)(((float)ui->dsb_DAC_DGAIN_yp->value() * 1000.0f) * _Amplitude1mV);
    uint16_t DAC_DGAIN_YN = (uint16_t)(((float)ui->dsb_DAC_DGAIN_yn->value() * 1000.0f) * _Amplitude1mV);

    uint16_t DAC_DOF_XP = (uint16_t)(((float)ui->dsb_DAC_DOF_xp->value() * 1000.0f) * _Offset1mV);
    uint16_t DAC_DOF_XN = (uint16_t)(((float)ui->dsb_DAC_DOF_xn->value() * 1000.0f) * _Offset1mV);
    uint16_t DAC_DOF_YP = (uint16_t)(((float)ui->dsb_DAC_DOF_yp->value() * 1000.0f) * _Offset1mV);
    uint16_t DAC_DOF_YN = (uint16_t)(((float)ui->dsb_DAC_DOF_yn->value() * 1000.0f) * _Offset1mV);

    uint16_t DDS_PW_XP = (uint16_t)((float)ui->dsb_DDS_PW_xp->value() * _Phase1deg) << 4;
    uint16_t DDS_PW_XN = (uint16_t)((float)ui->dsb_DDS_PW_xn->value() * _Phase1deg) << 4;
    uint16_t DDS_PW_YP = (uint16_t)((float)ui->dsb_DDS_PW_yp->value() * _Phase1deg) << 4;
    uint16_t DDS_PW_YN = (uint16_t)((float)ui->dsb_DDS_PW_yn->value() * _Phase1deg) << 4;

    bool ok = false;
    QString reg;

    uint16_t DDS_TW32 = static_cast<uint16_t>(DDS_TW >> 8);            //41
    uint16_t DDS_TW1  = static_cast<uint16_t>((DDS_TW & 0xff) << 8);   //42

    // emit info(QString("DDS_TW %1").arg(DDS_TW));
    //DDS_TW32  41
    //DDS_TW1   42

    reg = "DDS_TW32"; ok = AD9106_write(reg, DDS_TW32); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DDS_TW1"; ok = AD9106_write(reg, DDS_TW1); if(!ok) emit error(QString("error write %1").arg(reg));

    reg = "DAC4_DGAIN"; ok = AD9106_write(reg, DAC_DGAIN_XP); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DAC3_DGAIN"; ok = AD9106_write(reg, DAC_DGAIN_XN); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DAC1_DGAIN"; ok = AD9106_write(reg, DAC_DGAIN_YP); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DAC2_DGAIN"; ok = AD9106_write(reg, DAC_DGAIN_YN); if(!ok) emit error(QString("error write %1").arg(reg));

    reg = "DAC4DOF"; ok = AD9106_write(reg, DAC_DOF_XP); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DAC3DOF"; ok = AD9106_write(reg, DAC_DOF_XN); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DAC1DOF"; ok = AD9106_write(reg, DAC_DOF_YP); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DAC2DOF"; ok = AD9106_write(reg, DAC_DOF_YN); if(!ok) emit error(QString("error write %1").arg(reg));

    reg = "DDS4_PW"; ok = AD9106_write(reg, DDS_PW_XP); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DDS3_PW"; ok = AD9106_write(reg, DDS_PW_XN); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DDS1_PW"; ok = AD9106_write(reg, DDS_PW_YP); if(!ok) emit error(QString("error write %1").arg(reg));
    reg = "DDS2_PW"; ok = AD9106_write(reg, DDS_PW_YN); if(!ok) emit error(QString("error write %1").arg(reg));

    emit info("Запись регистров: OK");
}
//--------------------------------------------------------------------------------
void AD9106_Box::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool AD9106_Box::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void AD9106_Box::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void AD9106_Box::save_setting(void)
{

}
//--------------------------------------------------------------------------------
void AD9106_Box::changeEvent(QEvent *event)
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
