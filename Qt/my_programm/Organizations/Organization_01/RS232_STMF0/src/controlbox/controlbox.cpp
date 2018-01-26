/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QRadioButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLCDNumber>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QFrame>
#include <QTimer>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "serialbox5.hpp"
#include "controlbox.hpp"
#include "sleeper.h"
#include "ui_controlbox.h"
//--------------------------------------------------------------------------------
#define BUTTON_ON   "background:gray"
#define BUTTON_OFF  ""
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
union DATA_FLOAT {
    float value;
    struct {
        unsigned char a;
        unsigned char b;
        unsigned char c;
        unsigned char d;
    } bytes;
};
//--------------------------------------------------------------------------------
ControlBox::ControlBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ControlBox),
    parent(parent),
    display(0),
    timer(0)
{
    init();
}
//--------------------------------------------------------------------------------
ControlBox::~ControlBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void ControlBox::init(void)
{
    ui->setupUi(this);

    grapher = new GrapherBox(this);
    grapher->set_title("Мегаграфик");
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(-3, 3);

    grapher->add_curve(tr("dirty"));
    grapher->add_curve(tr("clean"));

    gain = 1.0f;
    correct_value = 1.0f;
    calibration_in_progress = false;

    create_timer();
    create_serial_widgets();

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout(add_gain_group());
    hbox->addWidget(add_other_widget());
    hbox->addWidget(add_calibration_widget());

    QHBoxLayout *d_hbox = new QHBoxLayout();
    d_hbox->addLayout(add_display_layout());
    d_hbox->addWidget(add_measure_group());

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(d_hbox, 1);
    vbox->addLayout(hbox, 1);
    vbox->addWidget(grapher, 100);

    QFrame *frame = new QFrame(this);
    frame->setLayout(vbox);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QHBoxLayout *hbox_main = new QHBoxLayout();

    QVBoxLayout *serial_vbox = new QVBoxLayout();
    serial_vbox->addWidget(stm32);
    serial_vbox->addWidget(calibrator);
    serial_vbox->addStretch();

    hbox_main->addLayout(serial_vbox);
    hbox_main->addWidget(frame);

    setLayout(hbox_main);
}
//--------------------------------------------------------------------------------
void ControlBox::test(void)
{
    float value = 0.5f;
    for(int n=0; n<10000; n++)
    {
        if((qrand() % 100) < 50)
            value += 0.1f; //(qrand() % 100) / 100;
        else
            value -= 0.1f; //(qrand() % 100) / 100;
        grapher->add_curve_data(0, value);
        grapher->add_curve_data(1, value + (qrand() % 1));
    }
}
//--------------------------------------------------------------------------------
void ControlBox::create_timer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
//--------------------------------------------------------------------------------
void ControlBox::create_serial_widgets(void)
{
    stm32 = new SerialBox5(this, "STM32F0 (USB1)", "STM32F0 (USB1)");
    calibrator = new SerialBox5(this, "H4-6", "H4-6");

    connect(this, SIGNAL(send_to_serial(QByteArray)), stm32, SLOT(input(QByteArray)));
    connect(stm32, SIGNAL(output(QByteArray)), this, SLOT(read_data_serialBox(QByteArray)));
    connect(calibrator, SIGNAL(output(QByteArray)), this, SLOT(read_data_calibratorBox(QByteArray)));
}
//--------------------------------------------------------------------------------
QFrame *ControlBox::add_other_widget(void)
{
    btn_CMD_TEST = new QPushButton("test");
    btn_CMD_INIT = new QPushButton("init");
    btn_CMD_GET_DATA = new QPushButton("data");
    btn_CMD_GET_DIRTY_DATA = new QPushButton("dirty data");

    int btn_widgth = 70;
    btn_CMD_TEST->setFixedWidth(btn_widgth);
    btn_CMD_INIT->setFixedWidth(btn_widgth);
    btn_CMD_GET_DATA->setFixedWidth(btn_widgth);
    btn_CMD_GET_DIRTY_DATA->setFixedWidth(btn_widgth);

    connect(btn_CMD_TEST, SIGNAL(clicked()), this, SLOT(cmd_TEST()));
    connect(btn_CMD_INIT, SIGNAL(clicked()), this, SLOT(cmd_INIT()));
    connect(btn_CMD_GET_DATA, SIGNAL(clicked()), this, SLOT(cmd_GET_DATA()));
    connect(btn_CMD_GET_DIRTY_DATA, SIGNAL(clicked()), this, SLOT(cmd_GET_DIRTY_DATA()));

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(btn_CMD_TEST,  0, 0);
    grid->addWidget(btn_CMD_INIT,  1, 0);
    grid->addWidget(btn_CMD_GET_DATA, 0, 1);
    grid->addWidget(btn_CMD_GET_DIRTY_DATA, 1, 1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();
    vbox->addLayout(grid);
    vbox->addStretch();

    QFrame *frame = new QFrame();
    frame->setLayout(vbox);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    return frame;
}
//--------------------------------------------------------------------------------
QGroupBox *ControlBox::add_measure_group(void)
{
    btnStart = new QPushButton("start");
    btnStop  = new QPushButton("stop");

    btnStart->setFixedWidth(50);
    btnStop->setFixedWidth(50);

    sbTimer = new QSpinBox();
    sbTimer->setMinimum(1);
    sbTimer->setMaximum(60);

    sbMaxTime = new QSpinBox();
    sbMaxTime->setMinimum(1);
    sbMaxTime->setMaximum(0xFFFF);
    sbMaxTime->setValue(60);

    cbUnlimTimer = new QCheckBox();
    connect(cbUnlimTimer, SIGNAL(toggled(bool)), this, SLOT(timer_unlimited(bool)));

    connect(btnStart, SIGNAL(clicked()), this, SLOT(measure_start()));
    connect(btnStop,  SIGNAL(clicked()), this, SLOT(measure_stop()));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel(tr("изм.")));
    hbox->addWidget(btnStart);
    hbox->addWidget(btnStop);
    hbox->addWidget(new QLabel(tr("сек.")));
    hbox->addWidget(sbTimer);
    hbox->addWidget(new QLabel(tr("max.")));
    hbox->addWidget(sbMaxTime);
    hbox->addWidget(new QLabel(tr("unl.")));
    hbox->addWidget(cbUnlimTimer);
    hbox->addStretch();

    //QGroupBox *group = new QGroupBox(tr("измерение"), this);
    QGroupBox *group = new QGroupBox(this);
    group->setLayout(hbox);

    btnStop->setStyleSheet(BUTTON_ON);

    return group;
}
//--------------------------------------------------------------------------------
void ControlBox::timer_unlimited(bool state)
{
    sbMaxTime->setEnabled(!state);
}
//--------------------------------------------------------------------------------
QGroupBox *ControlBox::add_calibration_widget(void)
{
    //return (double) VREF * ((double) value_adc / (double) 8388608.0f - (double) 1.0f) * 1000000.0f;

    btn_CALIBRATION_NULL         = new QPushButton(tr("0"));
    btn_CALIBRATION_GAIN_MAX_010 = new QPushButton("0.1");
    btn_CALIBRATION_GAIN_MAX_1   = new QPushButton("1");
    btn_CALIBRATION_GAIN_MAX_5   = new QPushButton("5");
    btn_CALIBRATION_GAIN_MAX_8   = new QPushButton("8");
    btn_CALIBRATION_GAIN_MAX_16  = new QPushButton("16");
    btn_CALIBRATION_GAIN_MAX_32  = new QPushButton("32");
    btn_CALIBRATION_GAIN_MAX_40  = new QPushButton("40");
    btn_CALIBRATION_GAIN_MAX_64  = new QPushButton("64");
    btn_CALIBRATION_GAIN_MAX_80  = new QPushButton("80");
    btn_CALIBRATION_GAIN_MAX_128 = new QPushButton("128");
    btn_CALIBRATION_GAIN_MAX_160 = new QPushButton("160");
    btn_CALIBRATION_GAIN_MAX_320 = new QPushButton("320");
    btn_CALIBRATION_GAIN_MAX_640 = new QPushButton("640");

    int button_width = 32;
    btn_CALIBRATION_NULL->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_010->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_1->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_5->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_8->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_16->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_32->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_40->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_64->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_80->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_128->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_160->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_320->setFixedWidth(button_width);
    btn_CALIBRATION_GAIN_MAX_640->setFixedWidth(button_width);

    connect(btn_CALIBRATION_NULL, SIGNAL(clicked()), this, SLOT(cmd_calibration_null()));
    connect(btn_CALIBRATION_GAIN_MAX_010, SIGNAL(clicked()), this, SLOT(cmd_calibration_010()));
    connect(btn_CALIBRATION_GAIN_MAX_1,   SIGNAL(clicked()), this, SLOT(cmd_calibration_1()));
    connect(btn_CALIBRATION_GAIN_MAX_5,   SIGNAL(clicked()), this, SLOT(cmd_calibration_5()));
    connect(btn_CALIBRATION_GAIN_MAX_8,   SIGNAL(clicked()), this, SLOT(cmd_calibration_8()));
    connect(btn_CALIBRATION_GAIN_MAX_16,  SIGNAL(clicked()), this, SLOT(cmd_calibration_16()));
    connect(btn_CALIBRATION_GAIN_MAX_32,  SIGNAL(clicked()), this, SLOT(cmd_calibration_32()));
    connect(btn_CALIBRATION_GAIN_MAX_40,  SIGNAL(clicked()), this, SLOT(cmd_calibration_40()));
    connect(btn_CALIBRATION_GAIN_MAX_64,  SIGNAL(clicked()), this, SLOT(cmd_calibration_64()));
    connect(btn_CALIBRATION_GAIN_MAX_80,  SIGNAL(clicked()), this, SLOT(cmd_calibration_80()));
    connect(btn_CALIBRATION_GAIN_MAX_128, SIGNAL(clicked()), this, SLOT(cmd_calibration_128()));
    connect(btn_CALIBRATION_GAIN_MAX_160, SIGNAL(clicked()), this, SLOT(cmd_calibration_160()));
    connect(btn_CALIBRATION_GAIN_MAX_320, SIGNAL(clicked()), this, SLOT(cmd_calibration_320()));
    connect(btn_CALIBRATION_GAIN_MAX_640, SIGNAL(clicked()), this, SLOT(cmd_calibration_640()));

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(btn_CALIBRATION_NULL,           0, 0);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_010,   0, 1);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_1,     0, 2);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_5,     0, 3);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_8,     0, 4);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_16,    0, 5);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_32,    0, 6);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_40,    1, 0);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_64,    1, 1);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_80,    1, 2);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_128,   1, 3);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_160,   1, 4);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_320,   1, 5);
    grid->addWidget(btn_CALIBRATION_GAIN_MAX_640,   1, 6);

    QGroupBox *group = new QGroupBox(tr("калибровка"), this);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    group->setLayout(vbox);

    return group;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_null(void)
{
    emit info("CMD_CALIBRATION_NULL");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_NULL, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_010(void)
{
    emit info("CMD_CALIBRATION_MAX_010");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_010, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_1(void)
{
    emit info("CMD_CALIBRATION_MAX_1");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_1, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_5(void)
{
    emit info("CMD_CALIBRATION_MAX_5");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_5, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_8(void)
{
    emit info("CMD_CALIBRATION_MAX_8");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_8, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_16(void)
{
    emit info("CMD_CALIBRATION_MAX_16");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_16, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_32(void)
{
    emit info("CMD_CALIBRATION_MAX_32");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_32, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_40(void)
{
    emit info("CMD_CALIBRATION_MAX_40");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_40, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_64(void)
{
    emit info("CMD_CALIBRATION_MAX_64");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_64, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_80(void)
{
    emit info("CMD_CALIBRATION_MAX_80");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_80, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_128(void)
{
    emit info("CMD_CALIBRATION_MAX_128");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_128, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_160(void)
{
    emit info("CMD_CALIBRATION_MAX_160");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_160, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_320(void)
{
    emit info("CMD_CALIBRATION_MAX_320");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_320, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_calibration_640(void)
{
    emit info("CMD_CALIBRATION_MAX_640");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_CALIBRATION_MAX_640, 500, true) == false)
    {
        emit error("Failed");
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
QHBoxLayout *ControlBox::add_display_layout(void)
{
    display = new QLCDNumber(this);
    display->setMinimumWidth(350);
    display->setMinimumHeight(48);
    display->setDigitCount(12);
    display->setSegmentStyle(QLCDNumber::Flat);
    display->setStyleSheet("background:black; color:lightgreen;");

    QLabel *label = new QLabel(this);
    label->setText("uV");
    label->setFont(QFont("Times", 22));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch();
    hbox->addWidget(display);
    hbox->addWidget(label);
    hbox->addStretch();

    return hbox;
}
//--------------------------------------------------------------------------------
QHBoxLayout *ControlBox::add_gain_group(void)
{
    QPushButton *btnSetGain = new QPushButton("set", this);
    QPushButton *btnGetGain = new QPushButton("get", this);

    connect(btnSetGain, SIGNAL(clicked()), this, SLOT(set_gain()));
    connect(btnGetGain, SIGNAL(clicked()), this, SLOT(get_gain()));

    QVBoxLayout *btn_box = new QVBoxLayout();
    btn_box->addWidget(btnSetGain);
    btn_box->addWidget(btnGetGain);

    btn_gain_010  = new QRadioButton("0.1");
    btn_gain_1    = new QRadioButton("1");
    btn_gain_5    = new QRadioButton("5");
    btn_gain_8    = new QRadioButton("8");
    btn_gain_16   = new QRadioButton("16");
    btn_gain_32   = new QRadioButton("32");
    btn_gain_40   = new QRadioButton("40");
    btn_gain_64   = new QRadioButton("64");
    btn_gain_80   = new QRadioButton("80");
    btn_gain_128  = new QRadioButton("128");
    btn_gain_160  = new QRadioButton("160");
    btn_gain_320  = new QRadioButton("320");
    btn_gain_640  = new QRadioButton("640");

    btn_gain_1->setChecked(true);

    QGridLayout *hbox = new QGridLayout();
    hbox->addWidget(btn_gain_010,   0, 0);
    hbox->addWidget(btn_gain_1,     0, 1);
    hbox->addWidget(btn_gain_5,     0, 2);
    hbox->addWidget(btn_gain_8,     0, 3);
    hbox->addWidget(btn_gain_16,    0, 4);
    hbox->addWidget(btn_gain_32,    0, 5);
    hbox->addWidget(btn_gain_40,    0, 6);
    hbox->addWidget(btn_gain_64,    1, 0);
    hbox->addWidget(btn_gain_80,    1, 1);
    hbox->addWidget(btn_gain_128,   1, 2);
    hbox->addWidget(btn_gain_160,   1, 3);
    hbox->addWidget(btn_gain_320,   1, 4);
    hbox->addWidget(btn_gain_640,   1, 5);

    QHBoxLayout *group_box = new QHBoxLayout();
    group_box->addLayout(hbox);
    group_box->addLayout(btn_box);

    QGroupBox *group = new QGroupBox(tr("усиление"), this);
    group->setLayout(group_box);

    QHBoxLayout *main_box = new QHBoxLayout();
    main_box->addWidget(group);

    return main_box;
}
//--------------------------------------------------------------------------------
void ControlBox::set_gain(void)
{
    if(btn_gain_010->isChecked())   cmd_SET_GAIN_010();
    if(btn_gain_1->isChecked())     cmd_SET_GAIN_1();
    if(btn_gain_5->isChecked())     cmd_SET_GAIN_5();
    if(btn_gain_8->isChecked())     cmd_SET_GAIN_8();
    if(btn_gain_16->isChecked())    cmd_SET_GAIN_16();
    if(btn_gain_32->isChecked())    cmd_SET_GAIN_32();
    if(btn_gain_40->isChecked())    cmd_SET_GAIN_40();
    if(btn_gain_64->isChecked())    cmd_SET_GAIN_64();
    if(btn_gain_80->isChecked())    cmd_SET_GAIN_80();
    if(btn_gain_128->isChecked())   cmd_SET_GAIN_128();
    if(btn_gain_160->isChecked())   cmd_SET_GAIN_160();
    if(btn_gain_320->isChecked())   cmd_SET_GAIN_320();
    if(btn_gain_640->isChecked())   cmd_SET_GAIN_640();
}
//--------------------------------------------------------------------------------
void ControlBox::get_gain(void)
{
    bool ok = false;
    int gain = cmd_GET_GAIN(&ok);
    if(ok)
    {
        switch (gain)
        {
        case CORRECTION_INDEX_010:  btn_gain_010->click(); break;
        case CORRECTION_INDEX_1:    btn_gain_1->click(); break;
        case CORRECTION_INDEX_5:    btn_gain_5->click(); break;
        case CORRECTION_INDEX_8:    btn_gain_8->click(); break;
        case CORRECTION_INDEX_16:   btn_gain_16->click(); break;
        case CORRECTION_INDEX_32:   btn_gain_32->click(); break;
        case CORRECTION_INDEX_40:   btn_gain_40->click(); break;
        case CORRECTION_INDEX_64:   btn_gain_64->click(); break;
        case CORRECTION_INDEX_80:   btn_gain_80->click(); break;
        case CORRECTION_INDEX_128:  btn_gain_128->click(); break;
        case CORRECTION_INDEX_160:  btn_gain_160->click(); break;
        case CORRECTION_INDEX_320:  btn_gain_320->click(); break;
        case CORRECTION_INDEX_640:  btn_gain_640->click(); break;
        default: break;
        }
    }
}
//--------------------------------------------------------------------------------
QGroupBox *ControlBox::add_mul_group(void)
{
    QRadioButton *btn_mul_1 = new QRadioButton("1");
    QRadioButton *btn_mul_5 = new QRadioButton("5");
    btn_mul_1->setChecked(true);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(btn_mul_1);
    hbox->addWidget(btn_mul_5);
    hbox->addStretch();

    QGroupBox *group = new QGroupBox(tr("умножение"), this);
    group->setLayout(hbox);
    return group;
}
//--------------------------------------------------------------------------------
QGroupBox *ControlBox::add_div_group(void)
{
    QRadioButton *btn_div_1   = new QRadioButton("1");
    QRadioButton *btn_div_2   = new QRadioButton("2");
    btn_div_1->setChecked(true);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(btn_div_1);
    hbox->addWidget(btn_div_2);
    hbox->addStretch();

    QGroupBox *group = new QGroupBox(tr("деление"), this);
    group->setLayout(hbox);
    return group;
}
//--------------------------------------------------------------------------------
void ControlBox::update(void)
{
    //emit info("update");

    if(stm32->isOpen() == false)
    {
        measure_stop();
        QMessageBox::critical(this, tr("Ошибка"), tr("Порт STM32F0 не открыт"));
        return;
    }

    bool ok = false;
    double dirty_value = 0;
    double clean_value = 0;

    dirty_value = get_dirty_data(500, &ok);
    if(!ok)
    {
        emit error("error read dirty data!");
    }
    Sleeper::msleep(1000);
    clean_value = get_data(500, &ok);
    if(!ok)
    {
        emit error("error read clean data!");
    }

    send_data_to_grapherBox(dirty_value, clean_value);

    if(cbUnlimTimer->isChecked()) return;

    cnt_max_time++;
    if(cnt_max_time >= sbMaxTime->value())
    {
        measure_stop();
    }
}
//--------------------------------------------------------------------------------
void ControlBox::measure_start(void)
{
    btnStart->setStyleSheet(BUTTON_ON);
    btnStop->setStyleSheet(BUTTON_OFF);
    cnt_max_time = 0;
    timer->start(sbTimer->value() * 1000);
}
//--------------------------------------------------------------------------------
void ControlBox::measure_stop(void)
{
    btnStart->setStyleSheet(BUTTON_OFF);
    btnStop->setStyleSheet(BUTTON_ON);
    timer->stop();
}
//--------------------------------------------------------------------------------
bool ControlBox::send_check_cmd(unsigned char cmd,
                                int timeout_msec,
                                bool silence)
{
    QByteArray ba;
    ba.append(cmd);
    data_stm32.clear();
    emit send_to_serial(ba);

    switch(cmd)
    {
    case GMD_GET_GAIN:
    case CMD_GET_DATA:
    case CMD_GET_DIRTY_DATA:
        wait_msec(timeout_msec, 6);
        break;
    default:
        wait_msec(timeout_msec, 2);
        break;
    }

    if(!silence)
        emit debug(QString("serial_data %1").arg(data_stm32.right(4).toHex().data()));

    if(data_stm32.isEmpty())
    {
        emit error("No data!");
        return false;
    }

    if(data_stm32.at(0) != cmd)
    {
        emit error(QString("return invalid command %1 != %2")
                   .arg(cmd)
                   .arg((int)data_stm32.at(0)));
        return false;
    }
    if(data_stm32.at(1) != CMD_OK)
    {
        emit error(QString("return invalid value %1 != %2")
                   .arg(CMD_OK)
                   .arg((int)data_stm32.at(1)));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_TEST(void)
{
    emit info("cmd_TEST");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_TEST, 100, true))
    {
        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_INIT(void)
{
    emit info("cmd_INIT");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_INIT, 5000, true))
    {
        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_GET_DATA(void)
{
    emit info("cmd_GET_DATA");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }

    bool ok = false;
    double value = get_data(500, &ok);
    if(ok)
    {
        draw_value(value);
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_GET_DIRTY_DATA(void)
{
    emit info("cmd_GET_DIRTY_DATA");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }

    bool ok = false;
    double value = get_dirty_data(500, &ok);
    if(ok)
    {
        draw_value(value);
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
int ControlBox::cmd_GET_GAIN(bool *ok)
{
    emit info("cmd_GET_GAIN");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        *ok = false;
        return 0;
    }

    bool temp_ok = false;
    int value = get_gain(500, &temp_ok);
    if(temp_ok)
    {
        QString gate;
        switch(value)
        {
        case CORRECTION_INDEX_010:  gate = "0.1"; break;
        case CORRECTION_INDEX_1:    gate = "1";   break;
        case CORRECTION_INDEX_5:    gate = "5";   break;
        case CORRECTION_INDEX_8:    gate = "8";   break;
        case CORRECTION_INDEX_16:   gate = "16";  break;
        case CORRECTION_INDEX_32:   gate = "32";  break;
        case CORRECTION_INDEX_40:   gate = "40";  break;
        case CORRECTION_INDEX_64:   gate = "64";  break;
        case CORRECTION_INDEX_80:   gate = "80";  break;
        case CORRECTION_INDEX_128:  gate = "128"; break;
        case CORRECTION_INDEX_160:  gate = "160"; break;
        case CORRECTION_INDEX_320:  gate = "320"; break;
        case CORRECTION_INDEX_640:  gate = "640"; break;
        default:   gate = QString("undefined gain: %1 ").arg(value); break;
        }

        QMessageBox::information(this, "GAIN", QString("gain %1").arg(gate));
    }
    else
    {
        emit error("Failed");
        *ok = false;
        return 0;
    }
    *ok = true;
    return value;
}
//--------------------------------------------------------------------------------
float ControlBox::get_data_value(void)
{
    union DATA_FLOAT data;

    data.bytes.a = data_stm32.at(2);
    data.bytes.b = data_stm32.at(3);
    data.bytes.c = data_stm32.at(4);
    data.bytes.d = data_stm32.at(5);

    return data.value;
}
//--------------------------------------------------------------------------------
double ControlBox::get_float_data(int timeout_msec, bool *ok)
{
    emit info("get_data");

    double value = 0.0f;
    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        *ok = false;
        return 0;
    }
    if(send_check_cmd(GMD_GET_GAIN, timeout_msec, true))
    {
        emit info("OK");
        if(data_stm32.count() == 6)
        {
            value = get_data_value();
        }
        else
        {
            emit error(QString("count %1 != 6").arg(data_stm32.count()));
            *ok = false;
            return 0;
        }
    }
    else
    {
        emit error("Failed");
        *ok = false;
        return 0;
    }
    *ok = true;
    return value;
}
//--------------------------------------------------------------------------------
double ControlBox::get_data(int timeout_msec, bool *ok)
{
    //emit info("get_data");

    double value = 0.0f;
    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        *ok = false;
        return 0;
    }
    if(send_check_cmd(CMD_GET_DATA, timeout_msec, true))
    {
        //emit info("OK");
        if(data_stm32.count() == 6)
        {
            value = get_data_value();
            draw_value(value);
        }
        else
        {
            emit error(QString("count %1 != 6").arg(data_stm32.count()));
            *ok = false;
            return 0;
        }
    }
    else
    {
        emit error("Failed");
        *ok = false;
        return 0;
    }
    *ok = true;
    return value;
}
//--------------------------------------------------------------------------------
int ControlBox::get_gain(int timeout_msec, bool *ok)
{
    //emit info("get_gain");

    double value = 0.0f;
    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        *ok = false;
        return 0;
    }
    if(send_check_cmd(GMD_GET_GAIN, timeout_msec, true))
    {
        //emit info("OK");
        if(data_stm32.count() == 6)
        {
            value = get_data_value();
            draw_value(value);
        }
        else
        {
            emit error(QString("count %1 != 6").arg(data_stm32.count()));
            *ok = false;
            return 0;
        }
    }
    else
    {
        emit error("Failed");
        *ok = false;
        return 0;
    }
    *ok = true;
    return value;
}
//--------------------------------------------------------------------------------
double ControlBox::get_dirty_data(int timeout_msec, bool *ok)
{
    //emit info("get_dirty_data");

    double value = 0.0f;
    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        *ok = false;
        return 0;
    }
    if(send_check_cmd(CMD_GET_DIRTY_DATA, timeout_msec, true))
    {
        //emit info("OK");
        if(data_stm32.count() == 6)
        {
            value = get_data_value();
            draw_value(value);
        }
        else
        {
            emit error(QString("count %1 != 6").arg(data_stm32.count()));
            *ok = false;
            return 0;
        }
    }
    else
    {
        emit error("Failed");
        *ok = false;
        return 0;
    }
    *ok = true;
    return value;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_010(void)
{
    emit info("cmd_SET_GAIN_010");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_010, 100, true))
    {
        btn_gain_010->click();
        gain = 0.5f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_1(void)
{
    emit info("cmd_SET_GAIN_1");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_1, 100, true))
    {
        btn_gain_1->click();
        gain = 1.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_5(void)
{
    emit info("cmd_SET_GAIN_5");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_5, 100, true))
    {
        btn_gain_5->click();
        gain = 5.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_8(void)
{
    emit info("cmd_SET_GAIN_8");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_8, 100, true))
    {
        btn_gain_8->click();
        gain = 8.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_16(void)
{
    emit info("cmd_SET_GAIN_16");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_16, 100, true))
    {
        btn_gain_16->click();
        gain = 16.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_32(void)
{
    emit info("cmd_SET_GAIN_32");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_32, 100, true))
    {
        btn_gain_32->click();
        gain = 32.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_40(void)
{
    emit info("cmd_SET_GAIN_40");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_40, 100, true))
    {
        btn_gain_40->click();
        gain = 40.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_64(void)
{
    emit info("cmd_SET_GAIN_64");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_64, 100, true))
    {
        btn_gain_64->click();
        gain = 64.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_80(void)
{
    emit info("cmd_SET_GAIN_80");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_80, 100, true))
    {
        btn_gain_80->click();
        gain = 80.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_128(void)
{
    emit info("cmd_SET_GAIN_128");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_128, 100, true))
    {
        btn_gain_128->click();
        gain = 128.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_160(void)
{
    emit info("cmd_SET_GAIN_160");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_160, 100, true))
    {
        btn_gain_160->click();
        gain = 160.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_320(void)
{
    emit info("cmd_SET_GAIN_320");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_320, 100, true))
    {
        btn_gain_320->click();
        gain = 320.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool ControlBox::cmd_SET_GAIN_640(void)
{
    emit info("cmd_SET_GAIN_640");

    if(!stm32->isOpen())
    {
        QMessageBox::critical(this, "ERROR", tr("Порт не открыт!"));
        return false;
    }
    if(send_check_cmd(CMD_SET_GAIN_640, 100, true))
    {
        btn_gain_640->click();
        gain = 640.0f;

        emit info("OK");
        return true;
    }
    else
    {
        emit error("Failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void ControlBox::reset(void)
{
    emit debug(tr("сброс"));

    min_value = 10000000.0f;
    max_value = -10000000.0f;
    delta_value = 0.0f;

    value_index = 0;
    old_value = 0;

    grapher->reset();
}
//--------------------------------------------------------------------------------
void ControlBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        //if(is_ready) break;
    }
}
//--------------------------------------------------------------------------------
double ControlBox::get_average_value(const QString &calibration_string,
                                     int max_timeout,
                                     int max_err_count,
                                     int max_count,
                                     bool *result_ok)
{
    data_calibrator.clear();
    calibrator->input(calibration_string);
    wait_msec(1000, 2);

    double average_value = 0.0f;
    int cnt = 0;
    int err_cnt = 0;
    while(cnt < max_count)
    {
        bool ok = false;
        double value = get_data(max_timeout, &ok);
        if(ok)
        {
            average_value += value;
            cnt++;
        }
        else
        {
            err_cnt++;
            if(err_cnt > max_err_count)
            {
                *result_ok = false;
                return 0;
            }
        }
        Sleeper::msleep(max_timeout);
    }

    *result_ok = true;
    average_value /= cnt;
    return average_value;
}
//--------------------------------------------------------------------------------
void ControlBox::calibrate(void)
{
    if(stm32->isOpen() == false)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Порт STM32F0 не открыт"));
        return;
    }
    if(calibrator->isOpen() == false)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Порт H4-6 не открыт"));
        return;
    }

    set_enabled(false);

    cmd_SET_GAIN_640();
    Sleeper::sleep(5);

    bool ok = false;
    double average_null = get_average_value("UD0\n\r", 500, 100, 100, &ok);
    double average_max  = get_average_value("UD1.003125\n\r", 500, 100, 100, &ok);

    data_calibrator.clear();
    calibrator->input(QByteArray("UD0\n\r"));
    wait_msec(1000, 2);

    emit info(QString("average_null %1").arg(average_null));
    emit info(QString("average_max %1").arg(average_max));
    //correct_value =

    set_enabled(true);
}
//--------------------------------------------------------------------------------
void ControlBox::set_enabled(bool state)
{
    btnStart->setEnabled(state);
    btnStop->setEnabled(state);
    btnCalibrate->setEnabled(state);

    btn_CMD_TEST->setEnabled(state);
    btn_CMD_INIT->setEnabled(state);
    btn_CMD_GET_DATA->setEnabled(state);

    QCoreApplication::processEvents();
}
//--------------------------------------------------------------------------------
void ControlBox::send_data_to_grapherBox(double dirty_value,
                                         double clean_value)
{
    value_index++;
    grapher->add_curve_data(0, value_index, dirty_value);
    grapher->add_curve_data(1, value_index, clean_value);
    draw_value(clean_value);

    if(clean_value > max_value) max_value = clean_value;
    if(clean_value < min_value) min_value = clean_value;
    delta_value = max_value - min_value;

    emit info(QString("Value %1 \t\t min_value %2 \t\t max_value %3 \t\t delta_value %4")
              .arg(clean_value, 0, 'g', 9)
              .arg(min_value,   0, 'g', 9)
              .arg(max_value,   0, 'g', 9)
              .arg(delta_value, 0, 'g', 9));
    old_value = clean_value;
}
//--------------------------------------------------------------------------------
void ControlBox::read_data_serialBox(const QByteArray &data)
{
    data_stm32.append(data);
    data_stm32_is_ready = true;
}
//--------------------------------------------------------------------------------
void ControlBox::read_data_calibratorBox(const QByteArray &data)
{
    data_calibrator.append(data);
    data_calibrator_is_ready = true;
}
//--------------------------------------------------------------------------------
void ControlBox::wait_msec(int timeout_msec, int count_break)
{
    QTime time;

    if(timeout_msec < 1)
        return;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
            break;
        if(data_stm32.count() == count_break)
            break;
    }
}
//--------------------------------------------------------------------------------
void ControlBox::draw_value(double value)
{
    display->display(QString("%1").arg(value, 0, 'f', 3));
}
//--------------------------------------------------------------------------------
void ControlBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
