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
#include <QTreeWidgetItemIterator>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QToolButton>
#include <QTextStream>
#include <QTableView>
#include <QByteArray>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QKeyEvent>
#include <QSplitter>
#include <QSpinBox>
#include <QToolBar>
#include <QString>
#include <QDialog>
#include <QSound>
#include <QTimer>
#include <QDate>
#include <QTime>
#include <QList>
#include <QDir>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#ifdef CALIBRATION_WIRED
#include "ui_mainbox_wired.h"
#else
#include "ui_mainbox.h"
#endif
//--------------------------------------------------------------------------------
#include "H4_X_errors.hpp"
#include "H4_X_calibrator.hpp"
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
#include "v786_2_union.hpp"
//--------------------------------------------------------------------------------
#include "handcontrol.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "controllog.hpp"
#include "myreport.hpp"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    splash(splash),
    is_worked(false)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::createOptionsBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *optionsBar = new QToolBar(tr("optionsbar"));
    optionsBar->setObjectName("optionsBar");

    mw->addToolBar(Qt::TopToolBarArea, optionsBar);

    sb_time_calibration = new QSpinBox();
    sb_repeat = new QSpinBox();
    sb_delay = new QSpinBox();

    sb_time_calibration->setObjectName("sb_time_calibration");
    sb_repeat->setObjectName("sb_repeat");
    sb_delay->setObjectName("sb_delay");

    sb_time_calibration->setMinimum(MIN_TIME_CALIBRATION);
    sb_time_calibration->setMaximum(MAX_TIME_CALIBRATION);
    sb_time_calibration->setValue(TIME_CALIBRATION);

    sb_delay->setMinimum(100);
    sb_delay->setMaximum(10000);

    sb_repeat->setMinimum(1);
    sb_repeat->setMaximum(100);

    optionsBar->addWidget(new QLabel("Max time calibration"));
    optionsBar->addWidget(sb_time_calibration);
    optionsBar->addWidget(new QLabel("repeat"));
    optionsBar->addWidget(sb_repeat);
    optionsBar->addWidget(new QLabel("delay"));
    optionsBar->addWidget(sb_delay);
}
//--------------------------------------------------------------------------------
#ifdef CALIBRATOR_DEBUG
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "handcontrol",
                                       "handcontrol");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));

    toolBar->setMinimumSize(toolBar->sizeHint());
}
#endif
//--------------------------------------------------------------------------------
void MainBox::set_item_param(QTreeWidgetItem *item,
                             Qt::ItemFlags flags,
                             const QString &caption,
                             Qt::CheckState state)
{
    item->setFlags(item->flags() | flags);  // Qt::ItemIsUserCheckable
    item->setText(0, caption);
    item->setCheckState(0, state);          // Qt::Checked
}
//--------------------------------------------------------------------------------
void MainBox::init_tree_widgets(void)
{
    Qt::CheckState default_value = Qt::Unchecked;

    ui->treeWidget_limits->clear();
    ui->treeWidget_limits->setHeaderLabel(TREE_LIMITS_CAPTION);

    item_UDC = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::Type);
    item_UAC = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::Type);
#ifdef R_CALIBRATION_ENABLED
    item_R   = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::Type);
#endif

    item_limit_UDC_1000V = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::UserType);

    item_limit_UAC_200V  = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::UserType);
    item_limit_UAC_1000V = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::UserType);

    item_IDC = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::Type);
    item_IAC = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::Type);

    item_limit_IDC_20A    = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::UserType);
    item_limit_IAC_20A    = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::UserType);

    item_limit_UDC_200mV = new QTreeWidgetItem(item_UDC, QTreeWidgetItem::UserType);
    item_limit_UDC_2V    = new QTreeWidgetItem(item_UDC, QTreeWidgetItem::UserType);
    item_limit_UDC_20V   = new QTreeWidgetItem(item_UDC, QTreeWidgetItem::UserType);
    item_limit_UDC_200V  = new QTreeWidgetItem(item_UDC, QTreeWidgetItem::UserType);

    item_limit_UAC_200mV = new QTreeWidgetItem(item_UAC, QTreeWidgetItem::UserType);
    item_limit_UAC_2V    = new QTreeWidgetItem(item_UAC, QTreeWidgetItem::UserType);
    item_limit_UAC_20V   = new QTreeWidgetItem(item_UAC, QTreeWidgetItem::UserType);

    item_limit_IDC_200uA  = new QTreeWidgetItem(item_IDC, QTreeWidgetItem::UserType);
    item_limit_IDC_2mA    = new QTreeWidgetItem(item_IDC, QTreeWidgetItem::UserType);
    item_limit_IDC_20mA   = new QTreeWidgetItem(item_IDC, QTreeWidgetItem::UserType);
    item_limit_IDC_200mA  = new QTreeWidgetItem(item_IDC, QTreeWidgetItem::UserType);
    item_limit_IDC_2000mA = new QTreeWidgetItem(item_IDC, QTreeWidgetItem::UserType);

    item_limit_IAC_2mA    = new QTreeWidgetItem(item_IAC, QTreeWidgetItem::UserType);
    item_limit_IAC_20mA   = new QTreeWidgetItem(item_IAC, QTreeWidgetItem::UserType);
    item_limit_IAC_200mA  = new QTreeWidgetItem(item_IAC, QTreeWidgetItem::UserType);
    item_limit_IAC_2000mA = new QTreeWidgetItem(item_IAC, QTreeWidgetItem::UserType);

#ifdef R_CALIBRATION_ENABLED
    item_limit_R_200  = new QTreeWidgetItem(item_R, QTreeWidgetItem::UserType);
    item_limit_R_2K   = new QTreeWidgetItem(item_R, QTreeWidgetItem::UserType);
    item_limit_R_20K  = new QTreeWidgetItem(item_R, QTreeWidgetItem::UserType);
    item_limit_R_200K = new QTreeWidgetItem(item_R, QTreeWidgetItem::UserType);
    item_limit_R_2M   = new QTreeWidgetItem(item_R, QTreeWidgetItem::UserType);
#endif
    //---

    set_item_param(item_UDC, Qt::ItemIsUserCheckable, UDC, default_value);
    set_item_param(item_UAC, Qt::ItemIsUserCheckable, UAC, default_value);

    set_item_param(item_IDC, Qt::ItemIsUserCheckable, IDC, default_value);
    set_item_param(item_IAC, Qt::ItemIsUserCheckable, IAC, default_value);

#ifdef R_CALIBRATION_ENABLED
    set_item_param(item_R, Qt::ItemIsUserCheckable, RR, default_value);
#endif

    set_item_param(item_limit_UDC_200mV, Qt::ItemIsUserCheckable, LIMIT_UDC_200mV,  default_value);
    set_item_param(item_limit_UDC_2V,    Qt::ItemIsUserCheckable, LIMIT_UDC_2V,     default_value);
    set_item_param(item_limit_UDC_20V,   Qt::ItemIsUserCheckable, LIMIT_UDC_20V,    default_value);
    set_item_param(item_limit_UDC_200V,  Qt::ItemIsUserCheckable, LIMIT_UDC_200V,   default_value);

    set_item_param(item_limit_UAC_200mV, Qt::ItemIsUserCheckable, LIMIT_UAC_200mV,  default_value);
    set_item_param(item_limit_UAC_2V,    Qt::ItemIsUserCheckable, LIMIT_UAC_2V,     default_value);
    set_item_param(item_limit_UAC_20V,   Qt::ItemIsUserCheckable, LIMIT_UAC_20V,    default_value);

    set_item_param(item_limit_UAC_200V,  Qt::ItemIsUserCheckable, LIMIT_UAC_200V,   default_value);
    set_item_param(item_limit_UDC_1000V, Qt::ItemIsUserCheckable, LIMIT_UDC_1000V,  default_value);
    set_item_param(item_limit_UAC_1000V, Qt::ItemIsUserCheckable, LIMIT_UAC_1000V,  default_value);

    set_item_param(item_limit_IDC_200uA, Qt::ItemIsUserCheckable, LIMIT_IDC_200uA,  default_value);
    set_item_param(item_limit_IDC_2mA,   Qt::ItemIsUserCheckable, LIMIT_IDC_2mA,    default_value);
    set_item_param(item_limit_IDC_20mA,  Qt::ItemIsUserCheckable, LIMIT_IDC_20mA,   default_value);
    set_item_param(item_limit_IDC_200mA, Qt::ItemIsUserCheckable, LIMIT_IDC_200mA,  default_value);
    set_item_param(item_limit_IDC_2000mA,Qt::ItemIsUserCheckable, LIMIT_IDC_2000mA, default_value);
    set_item_param(item_limit_IDC_20A,   Qt::ItemIsUserCheckable, LIMIT_IDC_20A,    default_value);

    set_item_param(item_limit_IAC_2mA,   Qt::ItemIsUserCheckable, LIMIT_IAC_2mA,    default_value);
    set_item_param(item_limit_IAC_20mA,  Qt::ItemIsUserCheckable, LIMIT_IAC_20mA,   default_value);
    set_item_param(item_limit_IAC_200mA, Qt::ItemIsUserCheckable, LIMIT_IAC_200mA,  default_value);
    set_item_param(item_limit_IAC_2000mA,Qt::ItemIsUserCheckable, LIMIT_IAC_2000mA, default_value);
    set_item_param(item_limit_IAC_20A,   Qt::ItemIsUserCheckable, LIMIT_IAC_20A,    default_value);

#ifdef R_CALIBRATION_ENABLED
    set_item_param(item_limit_R_200,    Qt::ItemIsUserCheckable,  LIMIT_R_200,      default_value);
    set_item_param(item_limit_R_2K,     Qt::ItemIsUserCheckable,  LIMIT_R_2K,       default_value);
    set_item_param(item_limit_R_20K,    Qt::ItemIsUserCheckable,  LIMIT_R_20K,      default_value);
    set_item_param(item_limit_R_200K,   Qt::ItemIsUserCheckable,  LIMIT_R_200K,     default_value);
    set_item_param(item_limit_R_2M,     Qt::ItemIsUserCheckable,  LIMIT_R_2M,       default_value);
#endif

#ifdef USE_BOTH
    if(type_calibrator == H4_6)
    {
        //item_limit_IDC_20A->setDisabled(true);
        //item_limit_IAC_20A->setDisabled(true);

        item_limit_UAC_200V->setDisabled(true);
        item_limit_UAC_1000V->setDisabled(true);
    }
#else
    if(type_calibrator == H4_6)
    {
        item_limit_IDC_20A->setDisabled(true);
        //item_limit_IAC_20A->setDisabled(true);
        item_limit_UAC_200V->setDisabled(true);
        item_limit_UAC_1000V->setDisabled(true);

        item_limit_IDC_20A->setHidden(true);
        //item_limit_IAC_20A->setHidden(true);
        item_limit_UAC_200V->setHidden(true);
        item_limit_UAC_1000V->setHidden(true);
    }
#endif

#ifdef USE_ONLY_H4_7
    if(type_calibrator == H4_7)
    {
        item_UDC->setHidden(true);
        item_UAC->setHidden(true);
        item_IDC->setHidden(true);
        item_IAC->setHidden(true);

        item_limit_UDC_200mV->setHidden(true);
        item_limit_UDC_2V->setHidden(true);
        item_limit_UDC_20V->setHidden(true);
        item_limit_UDC_200V->setHidden(true);
        item_limit_UDC_1000V->setHidden(true);

        item_limit_UAC_200mV->setHidden(true);
        item_limit_UAC_2V->setHidden(true);
        item_limit_UAC_20V->setHidden(true);
        //item_limit_UAC_200V->setHidden(true);
        //item_limit_UAC_1000V->setHidden(true);

        item_limit_IDC_200uA->setHidden(true);
        item_limit_IDC_2mA->setHidden(true);
        item_limit_IDC_20mA->setHidden(true);
        item_limit_IDC_200mA->setHidden(true);
        item_limit_IDC_2000mA->setHidden(true);
        //item_limit_IDC_20A->setHidden(true);

        item_limit_IAC_2mA->setHidden(true);
        item_limit_IAC_20mA->setHidden(true);
        item_limit_IAC_200mA->setHidden(true);
        item_limit_IAC_2000mA->setHidden(true);
        //item_limit_IAC_20A->setHidden(true);
    }
#endif

    //---
    ui->treeWidget_controls_U->clear();
    ui->treeWidget_controls_U->setHeaderLabel(TREE_CONTROLS_CAPTION_U);
    ui->treeWidget_controls_I->clear();
    ui->treeWidget_controls_I->setHeaderLabel(TREE_CONTROLS_CAPTION_I);
    ui->treeWidget_controls_Other->clear();
    ui->treeWidget_controls_Other->setHeaderLabel(TREE_CONTROLS_CAPTION_O);
    ui->treeWidget_controls_Hi_Low_U->clear();
    ui->treeWidget_controls_Hi_Low_U->setHeaderLabel(TREE_CONTROLS_CAPTION_HLU);

    QStringList controls_UDC_sl;
    controls_UDC_sl << CONTROL_UDC_10mV_TEXT
                    << CONTROL_UDC_minus_10mV_TEXT
                    << CONTROL_UDC_170mV_TEXT
                    << CONTROL_UDC_minus_170mV_TEXT
                    << CONTROL_UDC_0_5V_TEXT
                    << CONTROL_UDC_minus_0_5V_TEXT
                    << CONTROL_UDC_1V_TEXT
                    << CONTROL_UDC_1_5V_TEXT
                    << CONTROL_UDC_1_7V_TEXT
                    << CONTROL_UDC_minus_1_7V_TEXT
                    << CONTROL_UDC_17_0V_TEXT
                    << CONTROL_UDC_170V_TEXT;
    if(type_calibrator == H4_7)
    {
        controls_UDC_sl << CONTROL_UDC_1000V_TEXT
                        << CONTROL_UDC_minus_1000V_TEXT;
    }

    QStringList controls_UAC_sl;
    controls_UAC_sl << CONTROL_UAC_17mV_20kHz_TEXT
                    << CONTROL_UAC_190mV_1kHz_TEXT
                    << CONTROL_UAC_190mV_50kHz_TEXT
                    << CONTROL_UAC_190mV_100kHz_TEXT
                    << CONTROL_UAC_0_5V_1kHz_TEXT
                    << CONTROL_UAC_1V_20Hz_TEXT
                    << CONTROL_UAC_1V_1kHz_TEXT
                    << CONTROL_UAC_1V_20kHz_TEXT
                    << CONTROL_UAC_1V_50kHz_TEXT
                    << CONTROL_UAC_1V_100kHz_TEXT
                    << CONTROL_UAC_1_7V_1kHz_TEXT
                    << CONTROL_UAC_10V_1kHz_TEXT
                    << CONTROL_UAC_10V_20kHz_TEXT
                    << CONTROL_UAC_10V_50kHz_TEXT
                    << CONTROL_UAC_10V_100kHz_TEXT
                    << CONTROL_UAC_100V_1kHz_TEXT
                    << CONTROL_UAC_100V_20kHz_TEXT
                    << CONTROL_UAC_100V_100kHz_TEXT;
    if(type_calibrator == H4_7)
    {
        controls_UAC_sl << CONTROL_UAC_170V_1kHz_TEXT
                        << CONTROL_UAC_400V_1kHz_TEXT;
    }

    QStringList controls_IDC_sl;
    controls_IDC_sl << CONTROL_IDC_0_5mkA_TEXT
                    << CONTROL_IDC_100mkA_TEXT
                    << CONTROL_IDC_minus_100mkA_TEXT
                    << CONTROL_IDC_1mA_TEXT
                    << CONTROL_IDC_minus_1mA_TEXT
                    << CONTROL_IDC_10mA_TEXT
                    << CONTROL_IDC_minus_10mA_TEXT
                    << CONTROL_IDC_100mA_TEXT
                    << CONTROL_IDC_1000mA_TEXT
                    << CONTROL_IDC_minus_1000mA_TEXT;
    if(type_calibrator == H4_7)
    {
        controls_IDC_sl << CONTROL_IDC_19A_TEXT
                        << CONTROL_IDC_minus_19A_TEXT;
    }

    QStringList controls_IAC_sl;
    controls_IAC_sl << CONTROL_IAC_1mA_1_kHz_TEXT
                    << CONTROL_IAC_10mA_1_kHz_TEXT
                    << CONTROL_IAC_100mA_1_kHz_TEXT
                    << CONTROL_IAC_1000mA_1_kHz_TEXT
                    << CONTROL_IAC_10A_1_kHz_TEXT;

    QStringList controls_OTHER_sl;
    controls_OTHER_sl << CONTROL_R_0_Om_TEXT
                      << CONTROL_R_10_Om_TEXT
                      << CONTROL_R_100_Om_TEXT
                      << CONTROL_R_190_Om_TEXT
                      << CONTROL_R_1_kOm_TEXT
                      << CONTROL_R_1_9_kOm_TEXT
                      << CONTROL_R_10_kOm_TEXT
                      << CONTROL_R_100_kOm_TEXT
                      << CONTROL_R_1_MOm_TEXT
                      << CONTROL_C_0_pF_TEXT
                      << CONTROL_C_100_pF_TEXT
                      << CONTROL_C_1000_pF_TEXT
                      << CONTROL_C_10000_pF_TEXT
                      << CONTROL_C_0_1_mkF_TEXT
                      << CONTROL_C_1_mkF_TEXT
                      << CONTROL_C_10_mkF_TEXT
                      << CONTROL_C_1000_mkF_TEXT
                      << CONTROL_L_0_mH_TEXT
                      << CONTROL_L_1_mH_TEXT
                      << CONTROL_L_10_mH_TEXT
                      << CONTROL_L_100_mH_TEXT;

    QStringList controls_HI_sl;
    controls_HI_sl  << CONTROL_HI_UDC_1V_TEXT
                    << CONTROL_HI_UDC_10V_TEXT
                    << CONTROL_HI_UAC_1V_1kHz_TEXT
                    << CONTROL_HI_UAC_10V_1kHz_TEXT
                    << CONTROL_HI_R_1kOm_TEXT
                    << CONTROL_HI_R_100kOm_TEXT
                    << CONTROL_HI_IDC_10A_TEXT
                    << CONTROL_HI_FREQ_1MHz_TEXT
                    << CONTROL_HI_C_1mkF_TEXT
                    << CONTROL_HI_L_10mH_TEXT;

    QStringList controls_LO_sl;
    controls_LO_sl  << CONTROL_LO_UDC_1V_TEXT
                    << CONTROL_LO_UDC_10V_TEXT
                    << CONTROL_LO_UAC_1V_1kHz_TEXT
                    << CONTROL_LO_UAC_10V_1kHz_TEXT
                    << CONTROL_LO_R_1kOm_TEXT
                    << CONTROL_LO_R_100kOm_TEXT
                    << CONTROL_LO_IDC_10A_TEXT
                    << CONTROL_LO_FREQ_1MHz_TEXT
                    << CONTROL_LO_C_1mkF_TEXT
                    << CONTROL_LO_L_10mH_TEXT;

    QTreeWidgetItem *item_UDC = new QTreeWidgetItem(ui->treeWidget_controls_U, QTreeWidgetItem::Type);
    set_item_param(item_UDC, Qt::ItemIsUserCheckable, tr("постоянное"), Qt::Checked);
    for(int n=0; n<controls_UDC_sl.count(); n++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(item_UDC, QTreeWidgetItem::UserType);
        set_item_param(item, Qt::ItemIsUserCheckable, controls_UDC_sl.at(n), Qt::Checked);
    }

    QTreeWidgetItem *item_UAC = new QTreeWidgetItem(ui->treeWidget_controls_U, QTreeWidgetItem::Type);
    set_item_param(item_UAC, Qt::ItemIsUserCheckable, tr("переменное"), Qt::Checked);
    for(int n=0; n<controls_UAC_sl.count(); n++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(item_UAC, QTreeWidgetItem::UserType);
        set_item_param(item, Qt::ItemIsUserCheckable, controls_UAC_sl.at(n), Qt::Checked);
    }

    QTreeWidgetItem *item_IDC = new QTreeWidgetItem(ui->treeWidget_controls_I, QTreeWidgetItem::Type);
    set_item_param(item_IDC, Qt::ItemIsUserCheckable, tr("постоянный"), Qt::Checked);
    for(int n=0; n<controls_IDC_sl.count(); n++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(item_IDC, QTreeWidgetItem::UserType);
        set_item_param(item, Qt::ItemIsUserCheckable, controls_IDC_sl.at(n), Qt::Checked);
    }

    QTreeWidgetItem *item_IAC = new QTreeWidgetItem(ui->treeWidget_controls_I, QTreeWidgetItem::Type);
    set_item_param(item_IAC, Qt::ItemIsUserCheckable, tr("переменный"), Qt::Checked);
    for(int n=0; n<controls_IAC_sl.count(); n++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(item_IAC, QTreeWidgetItem::UserType);
        set_item_param(item, Qt::ItemIsUserCheckable, controls_IAC_sl.at(n), Qt::Checked);
    }

    QTreeWidgetItem *item_OTHER = new QTreeWidgetItem(ui->treeWidget_controls_Other, QTreeWidgetItem::Type);
    set_item_param(item_OTHER, Qt::ItemIsUserCheckable, tr("Прочее"), Qt::Checked);
    for(int n=0; n<controls_OTHER_sl.count(); n++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(item_OTHER, QTreeWidgetItem::UserType);
        set_item_param(item, Qt::ItemIsUserCheckable, controls_OTHER_sl.at(n), Qt::Checked);
    }

    QTreeWidgetItem *item_HI = new QTreeWidgetItem(ui->treeWidget_controls_Hi_Low_U, QTreeWidgetItem::Type);
    set_item_param(item_HI, Qt::ItemIsUserCheckable, tr("повышенное"), Qt::Checked);
    for(int n=0; n<controls_HI_sl.count(); n++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(item_HI, QTreeWidgetItem::UserType);
        set_item_param(item, Qt::ItemIsUserCheckable, controls_HI_sl.at(n), Qt::Checked);
    }

    QTreeWidgetItem *item_LO = new QTreeWidgetItem(ui->treeWidget_controls_Hi_Low_U, QTreeWidgetItem::Type);
    set_item_param(item_LO, Qt::ItemIsUserCheckable, tr("пониженное"), Qt::Checked);
    for(int n=0; n<controls_LO_sl.count(); n++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(item_LO, QTreeWidgetItem::UserType);
        set_item_param(item, Qt::ItemIsUserCheckable, controls_LO_sl.at(n), Qt::Checked);
    }
    //---

    connect(ui->treeWidget_limits, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));
    ui->treeWidget_limits->installEventFilter(this);

    connect(ui->treeWidget_controls_U, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));
    connect(ui->treeWidget_controls_I, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));
    connect(ui->treeWidget_controls_Other, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));
    connect(ui->treeWidget_controls_Hi_Low_U, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));

    ui->treeWidget_controls_U->installEventFilter(this);
    ui->treeWidget_controls_I->installEventFilter(this);
    ui->treeWidget_controls_Other->installEventFilter(this);
    ui->treeWidget_controls_Hi_Low_U->installEventFilter(this);
}
//--------------------------------------------------------------------------------
void MainBox::init_serialport_widgets(void)
{
    btn_find_devices = new QPushButton(tr("Найти устройства"));
    btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    connect(btn_find_devices, SIGNAL(clicked()), this, SLOT(search_devices()));

    ui->serial_layout->addWidget(btn_find_devices);
}
//--------------------------------------------------------------------------------
QString MainBox::get_port(const QString &busy_port,
                          bool *ok)
{
    QDialog dialog;

    QComboBox *cb_ports = new QComboBox();
    cb_ports->clear();
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        if(port.portName() != busy_port)
        {
            cb_ports->addItem(port.portName());
        }
    }

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel(tr("Выберите порт:")));
    hbox->addWidget(cb_ports);
    hbox->addWidget(btn_box);

    dialog.setLayout(hbox);

    dialog.setFixedSize(dialog.sizeHint());

    int btn = dialog.exec();
    if(btn == QDialog::Accepted)
    {
        *ok = true;
        return cb_ports->currentText();
    }
    *ok = false;
    return "";
}
//--------------------------------------------------------------------------------
bool MainBox::find_one_device(void)
{
    QSerialPort::FlowControl flow;
    bool ok = false;

#if 1
    flow = QSerialPort::NoFlowControl;
#else
    if(type_calibrator == H4_6)
    {
        flow = QSerialPort::NoFlowControl;
    }
    if(type_calibrator == H4_7)
    {
        flow = QSerialPort::HardwareControl;
    }
#endif

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    if(ports.count() < 1)
    {
        emit error(tr("Нужен МИНИМУМ 1 порт (калибратор)"));
        return false;
    }

    QString port_calibrator;
    if(ports.count() > 1)
    {
        port_calibrator = get_port("", &ok);
        if(!ok)
        {
            return false;
        }
    }

    foreach (const QSerialPortInfo &port, ports)
    {
        if(port.portName() == port_calibrator)
        {
            ok = calibrator->open_device(port_calibrator, flow);
            if(ok)
            {
                return true;
            }
        }
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MainBox::find_two_device(void)
{
    QSerialPort::FlowControl flow;
    QString port_name;
    bool ok = false;

#if 1
    flow = QSerialPort::NoFlowControl;
#else
    if(type_calibrator == H4_6)
    {
        flow = QSerialPort::NoFlowControl;
    }
    if(type_calibrator == H4_7)
    {
        flow = QSerialPort::HardwareControl;
    }
#endif

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    if(ports.count() < 2)
    {
        emit error(tr("Нужно МИНИМУМ 2 порта (калибратор и USB приемо-передатчик)"));
        return false;
    }

#ifdef CALIBRATION_WIRED
    ok = multimeter->find_wired_device(&port_name);
    if(!ok)
    {
        return false;
    }
#else
    ok = multimeter->find_device(&port_name);
    if(!ok)
    {
        return false;
    }
#endif

    QString port_calibrator = get_port(port_name, &ok);
    if(!ok)
    {
        return false;
    }

    ok = calibrator->open_device(port_calibrator, flow);
    if(!ok)
    {
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::search_devices(void)
{
    bool ok = false;

    ok = choice_calibration();
    if(!ok) return false;

    emit info(tr("Поиск устройств"));

#ifdef CALIBRATION_WIRED
    ok = find_two_device();
#else
    if(ui->cb_TypeMultimeter->currentIndex() == 2)
    {
        ok = find_two_device();
    }
    else
    {
        ok = find_one_device();
    }
#endif

    init_tree_widgets();
    emit info(tr("Поиск устройства закончен"));
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::choice_calibration(void)
{
#ifdef USE_ONLY_H4_6
    type_calibrator = H4_6;
    ui->lcd_type_calibrator->display("H4-6");
    return true;
#endif
#ifdef USE_ONLY_H4_7
    type_calibrator = H4_7;
    ui->lcd_type_calibrator->display("H4-7");
    return true;
#endif

    QDialog dialog;

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QRadioButton *calibrator_h4_6 = new QRadioButton();
    QRadioButton *calibrator_h4_7 = new QRadioButton();

    calibrator_h4_6->setText(tr("Н4-6"));
    calibrator_h4_7->setText(tr("Н4-7"));

    QVBoxLayout *calibrator_box = new QVBoxLayout();
    calibrator_box->addWidget(calibrator_h4_6);
    calibrator_box->addWidget(calibrator_h4_7);
    calibrator_h4_6->setChecked(true);

    QGroupBox *group_box = new QGroupBox();
    group_box->setLayout(calibrator_box);

    QLabel *caption = new QLabel(tr("Выберите калибратор"));
    caption->setAlignment(Qt::AlignHCenter);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(caption);
    vbox->addWidget(group_box);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog.setLayout(vbox);
    dialog.setFixedSize(dialog.sizeHint());

    int btn = dialog.exec();
    if(btn == QDialog::Accepted)
    {
        if(calibrator_h4_6->isChecked())
        {
            type_calibrator = H4_6;
            ui->lcd_type_calibrator->display("H4-6");
            emit info(tr("Выбран калибратор H4-6"));
            return true;
        }

        if(calibrator_h4_7->isChecked())
        {
            type_calibrator = H4_7;
            ui->lcd_type_calibrator->display("H4-7");
            emit info(tr("Выбран калибратор H4-7"));
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::update_text_type_calibrator(void)
{
    switch(type_calibrator)
    {
    case H4_6:  ui->lcd_type_calibrator->display("H4-6");   break;
    case H4_7:  ui->lcd_type_calibrator->display("H4-7");   break;
    default:    break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createOptionsBar();

    if(QDir("report").exists() == false)
    {
        QDir dir;
        if(dir.mkdir("report"))
        {
            emit info("dir 'report' created");
        }
    }

    control_log = new ControlLog("system.dat");
    control_log->set_date(QDate::currentDate().toString());
    control_log->set_time(QTime::currentTime().toString());
    control_log->set_result(tr("Программа калибровки запущена"));
    control_log->save();

    if(splash) splash->showMessage(tr("create log widget"));

    type_calibrator = H4_6;
#ifdef USE_ONLY_H4_6
    type_calibrator = H4_6;
#endif
#ifdef USE_ONLY_H4_7
    type_calibrator = H4_7;
#endif

    old_step.step_a = -1;
    old_step.step_b = -1;
    old_step.step_c = -1;

    new_step.step_a = -1;
    new_step.step_b = -1;
    new_step.step_c = -1;

    QTimer::singleShot(100, this, SLOT(update_text_type_calibrator()));

#ifdef CALIBRATOR_DEBUG
    if(splash) splash->showMessage(tr("create test bar"));
    createTestBar();
#endif

#ifdef FAKE
    if(splash) splash->showMessage(tr("create fake bar"));
    createFakeBar();
#endif

    current_state.cable_in = CABLE_IN;
    current_state.need_amplifier_U = false;
    current_state.need_amplifier_I_DC = false;
    current_state.need_amplifier_I_AC = false;

    if(splash) splash->showMessage(tr("create serialport widget"));
    init_serialport_widgets();
    if(splash) splash->showMessage(tr("create tree widget"));
    init_tree_widgets();

    if(splash) splash->showMessage(tr("init calibrator widget"));

    calibrator = new Calibrator(this);
    connect(calibrator, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(calibrator, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(calibrator, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(calibrator, SIGNAL(trace(QString)),   this, SIGNAL(trace(QString)));

    if(splash) splash->showMessage(tr("init multimeter widget"));

    multimeter = new Multimeter_V786_2(this);
    connect(multimeter, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(multimeter, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(multimeter, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(multimeter, SIGNAL(trace(QString)),   this, SIGNAL(trace(QString)));

    if(splash) splash->showMessage(tr("connect"));

    ui->cb_Step_Report->clear();
    ui->cb_Step_Report->addItem(tr("до технол. приработки"));
    ui->cb_Step_Report->addItem(tr("после технол. приработки"));

#ifndef CALIBRATION_WIRED
    ui->cb_TypeMultimeter->clear();
    ui->cb_TypeMultimeter->addItem(tr("В7-86"));
    ui->cb_TypeMultimeter->addItem(tr("В7-86/1"));
    ui->cb_TypeMultimeter->addItem(tr("В7-86/2"));
    connect(ui->cb_TypeMultimeter, SIGNAL(currentIndexChanged(int)), this, SLOT(type_selected(int)));
    ui->cb_TypeMultimeter->setCurrentIndex(2);

    ui->sb_channel->setMinimum(1);
    ui->sb_channel->setMaximum(15);
#endif

    connect(ui->btn_Report, SIGNAL(clicked()), this, SLOT(report()));
    connect(ui->btn_clear, SIGNAL(clicked()), this, SLOT(clear_report_database()));

    connect(ui->btn_Start, SIGNAL(clicked()), this, SLOT(start_calibration()));
    connect(ui->btn_Test_Connect_V786, SIGNAL(clicked()), this, SLOT(test_connect_v786()));
    connect(ui->btn_Test_Connect_Calibr, SIGNAL(clicked()), this, SLOT(test_connect_calibrator()));

    connect(ui->btn_limits_All_On,  SIGNAL(clicked()), this, SLOT(set_limits_all_on()));
    connect(ui->btn_limits_All_Off, SIGNAL(clicked()), this, SLOT(set_limits_all_off()));

    connect(ui->btn_controls_All_On,  SIGNAL(clicked()), this, SLOT(set_controls_all_on()));
    connect(ui->btn_controls_All_Off, SIGNAL(clicked()), this, SLOT(set_controls_all_off()));

    connect(ui->btn_CheckMultimeter, SIGNAL(clicked(bool)), this, SLOT(check_multemeter(bool)));

#ifdef CALIBRATION_WIRED
    connect(ui->btn_wired_off, SIGNAL(clicked()), this, SLOT(wired_off()));
#endif

    setMinimumWidth(1024);

#ifdef TREE_SPLITTER
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->setChildrenCollapsible(false);

    ui->control_frame->setParent(splitter);
    ui->limits_frame->setParent(splitter);
    splitter->addWidget(ui->limits_frame);
    splitter->addWidget(ui->control_frame);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    ui->tune_frame->setFixedWidth(ui->tune_frame->sizeHint().width());
    ui->layout_calibration->addWidget(splitter);
#endif

#ifndef CALIBRATOR_DEBUG
    ui->cb_ignore_error->setHidden(true);
#endif

#ifdef FAKE
    connect(calibrator, SIGNAL(send(QVariant)), multimeter, SLOT(receive(QVariant)));
#endif
}
//--------------------------------------------------------------------------------
#ifdef CALIBRATION_WIRED
void MainBox::wired_off(void)
{
    if(multimeter->port_is_open() == false)
    {
        message_critical(this, tr("Ошибка"), tr("Порт мультиметра В7-86 не открыт!"));
        return;
    }

    int button = QMessageBox::question(this,
                                       tr("Отключение проводного интерфейса"),
                                       tr("Вы уверены, что хотите отключить проводной интерфейс?"),
                                       QMessageBox::Yes | QMessageBox::No);
    if(button != QMessageBox::Yes) return;

    emit info(V764_2_CMD_0x1D_TEXT);
#ifdef CALIBRATION_WIRED
    multimeter->set_serno(ui->sb_SerNo->value());
    multimeter->set_year(ui->sb_Year->value());
    multimeter->set_address(FIXED_ADDRESS);
    multimeter->set_channel(FIXED_CHANNEL);
    multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
    multimeter->set_all(ui->sb_SerNo->value(),
                        ui->sb_Year->value(),
                        ui->sb_address->value(),
                        ui->sb_channel->value());
#endif

    multimeter->set_serno(ui->sb_SerNo->value());
    multimeter->set_year(ui->sb_Year->value());
    multimeter->set_address(FIXED_ADDRESS);
    multimeter->set_channel(FIXED_CHANNEL);
    multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
    multimeter->print_cmd_text(V764_2_CMD_0x1D);
    bool ok = multimeter->wired_off();
    if(!ok)
    {
        multimeter->print_last_error();
    }
}
#endif
//--------------------------------------------------------------------------------
void MainBox::type_selected(int index)
{
    if(index == 2)
    {
        ui->cb_Step_Report->setEnabled(true);
        ui->btn_clear->setEnabled(true);
        ui->btn_Report->setEnabled(true);
    }
    else
    {
        ui->cb_Step_Report->setEnabled(false);
        ui->btn_clear->setEnabled(false);
        ui->btn_Report->setEnabled(false);
    }
}
//--------------------------------------------------------------------------------
QString MainBox::return_freq_string_H4_6(float freq, bool append_key_Hz)
{
    QString temp;
    temp.clear();

    emit debug(QString("append_key_Hz %1 %2")
               .arg(freq)
               .arg(append_key_Hz));
    if(freq <= 0.1f)
    {
        if(append_key_Hz)
            temp.append(H4_6_KEY_Hz); //Hz
        freq *= 1000;
    }
    else
    {
        if(append_key_Hz)
            temp.append(H4_6_KEY_kHz); //kHz
    }

    temp.append(H4_6_KEY_ENTER_F);
    QString x = QString("%1").arg(freq);
    for(int n=0; n<x.length(); n++)
    {
        if(x.at(n) == '0') temp.append(H4_6_KEY_0);
        if(x.at(n) == '1') temp.append(H4_6_KEY_1);
        if(x.at(n) == '2') temp.append(H4_6_KEY_2);
        if(x.at(n) == '3') temp.append(H4_6_KEY_3);
        if(x.at(n) == '4') temp.append(H4_6_KEY_4);
        if(x.at(n) == '5') temp.append(H4_6_KEY_5);
        if(x.at(n) == '6') temp.append(H4_6_KEY_6);
        if(x.at(n) == '7') temp.append(H4_6_KEY_7);
        if(x.at(n) == '8') temp.append(H4_6_KEY_8);
        if(x.at(n) == '9') temp.append(H4_6_KEY_9);
        if(x.at(n) == '.') temp.append(H4_6_KEY_RIGHT);
    }
    temp.append(H4_6_KEY_ENTER_F);
    return temp;
}
//--------------------------------------------------------------------------------
QString MainBox::return_freq_string_H4_7(float freq)
{
    QString temp;
    temp.clear();

    emit debug(QString("return_freq_string_H4_7: freq %1").arg(freq));
    if(freq <= 0.1f)
    {
        temp.append(H4_7_KEY_Hz); //Hz
        freq *= 1000;
    }
    else
    {
        temp.append(H4_7_KEY_kHz); //kHz
    }

    temp.append(H4_7_KEY_ENTER_F);
    QString x = QString("%1").arg(freq);
    for(int n=0; n<x.length(); n++)
    {
        if(x.at(n) == '0') temp.append(H4_7_KEY_0);
        if(x.at(n) == '1') temp.append(H4_7_KEY_1);
        if(x.at(n) == '2') temp.append(H4_7_KEY_2);
        if(x.at(n) == '3') temp.append(H4_7_KEY_3);
        if(x.at(n) == '4') temp.append(H4_7_KEY_4);
        if(x.at(n) == '5') temp.append(H4_7_KEY_5);
        if(x.at(n) == '6') temp.append(H4_7_KEY_6);
        if(x.at(n) == '7') temp.append(H4_7_KEY_7);
        if(x.at(n) == '8') temp.append(H4_7_KEY_8);
        if(x.at(n) == '9') temp.append(H4_7_KEY_9);
        if(x.at(n) == '.') temp.append(H4_7_KEY_DOT);
    }
    temp.append(H4_7_KEY_ENTER_F);
    return temp;
}
//--------------------------------------------------------------------------------
void MainBox::spike_nail_1000V(void)
{
    QString temp;

    temp.clear();
    temp.append(H4_7_KEY_V);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    calibrator->step_to_step(temp,
                             sb_delay->value(),
                             sb_time_calibration->value(),
                             true);

    if(ui->cb_100->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_1);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);
        //temp.append(H4_7_KEY_OFF);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_200->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_2);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_300->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_3);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_400->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_4);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_500->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_5);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_600->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_6);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_700->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_7);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_800->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_8);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_900->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_9);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_1000->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_1);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 false);
    }
}
//--------------------------------------------------------------------------------
void MainBox::spike_nail_minus_1000V(void)
{
    QString temp;

    temp.clear();
    temp.append(H4_7_KEY_V);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);

    calibrator->step_to_step(temp,
                             sb_delay->value(),
                             sb_time_calibration->value(),
                             true);

    if(ui->cb_100->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_1);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_OFF);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_200->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_2);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_300->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_3);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_400->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_4);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_500->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_5);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_600->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_6);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_700->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_7);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_800->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_8);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_900->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_9);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 true);
    }

    if(ui->cb_1000->isChecked())
    {
        temp.clear();
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_1);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_PLUS_MINUS);
        temp.append(H4_7_KEY_ENTER_VI);

        calibrator->step_to_step(temp,
                                 sb_delay->value(),
                                 sb_time_calibration->value(),
                                 false);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_measuring_3500(bool state)
{
    flag_is_measuring = state;
    if(!state)
    {
        return;
    }

    multimeter->print_cmd_text(V764_2_CMD_0x02);
    bool ok = multimeter->remote_control_on();
    if(!ok)
    {
        QToolButton *btn = (QToolButton *)sender();
        if(btn) btn->setChecked(false);
        return;
    }

    //---
    int err_count = 0;
    int max_measuring = 3500;
    double value = 0;
    for(int n=0; n<max_measuring; n++)
    {
        if(!flag_is_measuring) break;
        emit info(QString("%1 of %2 errors %3")
                  .arg(n)
                  .arg(max_measuring)
                  .arg(err_count));
        multimeter->print_cmd_text(V764_2_CMD_0x07);
        multimeter->set_retention(0);
        multimeter->set_deviation(0);
        multimeter->set_limit(0);
        multimeter->set_auto_limit(true);
        bool ok = multimeter->measuring_UAC(&value);
        if(!ok)
        {
            multimeter->print_last_error();
            err_count++;
        }
    }
    //---
    multimeter->print_cmd_text(V764_2_CMD_0x03);
    multimeter->remote_control_off();
}
//--------------------------------------------------------------------------------
void MainBox::test_calibration_3500(bool state)
{
    flag_is_measuring = state;
    if(!state)
    {
        return;
    }
    multimeter->print_cmd_text(V764_2_CMD_0x02);
    bool ok = multimeter->remote_control_on();
    if(!ok)
    {
        QToolButton *btn = (QToolButton *)sender();
        if(btn) btn->setChecked(false);
        return;
    }

    //---
    int err_count = 0;
    int max_measuring = 3500;
    for(int n=0; n<max_measuring; n++)
    {
        if(!flag_is_measuring) break;
        emit info(QString("%1 of %2 errors %3")
                  .arg(n)
                  .arg(max_measuring)
                  .arg(err_count));
        multimeter->set_data_len(2);
        multimeter->set_data_0(2);
        multimeter->set_data_1(2);
        multimeter->print_cmd_text(V764_2_CMD_0x14);
        bool ok = multimeter->calibration_UAC();
        if(!ok)
        {
            multimeter->print_last_error();
            err_count++;
        }
    }
    //---
    multimeter->print_cmd_text(V764_2_CMD_0x03);
    multimeter->remote_control_off();
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    HandControl *hc = new HandControl(calibrator,
                                      multimeter,
                                      this);
    hc->setWindowTitle(tr("Ручное управление"));
    hc->setWindowIcon(QIcon(ICON_PROGRAMM));
    hc->set_serno(ui->sb_SerNo->value());
    hc->set_year(ui->sb_Year->value());

#ifdef CALIBRATION_WIRED
    hc->set_address(FIXED_ADDRESS);
    hc->set_channel(FIXED_CHANNEL);
#else
    hc->set_address(ui->sb_address->value());
    hc->set_channel(ui->sb_channel->value());
#endif

    hc->setFixedSize(hc->sizeHint());
    hc->exec();
}
//--------------------------------------------------------------------------------
void MainBox::test_connect_v786(void)
{
    emit info(tr(""));
    if(multimeter->port_is_open() == false)
    {
        message_critical(this, tr("Ошибка"), tr("Порт мультиметра В7-86 не открыт!"));
        return;
    }

    emit info(tr("Проверка радиоканала"));
#ifdef CALIBRATION_WIRED
    multimeter->set_serno(ui->sb_SerNo->value());
    multimeter->set_year(ui->sb_Year->value());
    multimeter->set_address(FIXED_ADDRESS);
    multimeter->set_channel(FIXED_CHANNEL);
    multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
    multimeter->set_serno(ui->sb_SerNo->value());
    multimeter->set_year(ui->sb_Year->value());
    multimeter->set_address(ui->sb_address->value());
    multimeter->set_channel(ui->sb_channel->value());
    multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

    bool ok = false;
    multimeter->print_cmd_text(V764_2_CMD_0x02);
    ok = multimeter->service_on(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(!ok)
    {
        multimeter->print_last_error();
    }

    multimeter->print_cmd_text(V764_2_CMD_0x01);
    ok = multimeter->find_cmd(V764_2_CMD_0x01);
    if(!ok)
    {
        multimeter->print_last_error();
    }
}
//--------------------------------------------------------------------------------
void MainBox::item_clicked(QTreeWidgetItem *item, int column)
{
    Qt::CheckState state = item->checkState(column) ? Qt::Checked : Qt::Unchecked;
    if(item->parent())
    {
        int cnt_is_checked = 0;
        for(int n=0; n<item->parent()->childCount(); n++)
        {
            if(item->parent()->child(n)->checkState(0) == Qt::Checked)
                cnt_is_checked++;
        }
        if(cnt_is_checked == 0)
        {
            item->parent()->setCheckState(0, Qt::Unchecked);
            return;
        }
        if(cnt_is_checked == item->parent()->childCount())
        {
            item->parent()->setCheckState(0, Qt::Checked);
        }
        else
        {
            item->parent()->setCheckState(0, Qt::PartiallyChecked);
        }
    }
    else
    {
        for(int n=0; n<item->childCount(); n++)
            item->child(n)->setCheckState(0, state);
    }
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Space)
        {
            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_steps(void)
{
    emit info(tr("Подготовка шагов калибровки"));

    answers.clear();
    QTreeWidgetItemIterator it(ui->treeWidget_limits);
    while (*it)
    {
        if((*it)->type() == QTreeWidgetItem::UserType)
        {
            if((*it)->checkState(0) == Qt::Checked)
            {
                if((*it)->text(0) == LIMIT_UDC_200mV) prepare_step_0_0();
                if((*it)->text(0) == LIMIT_UDC_2V)    prepare_step_0_1();
                if((*it)->text(0) == LIMIT_UDC_20V)   prepare_step_0_2();
                if((*it)->text(0) == LIMIT_UDC_200V)  prepare_step_0_3();
                if((*it)->text(0) == LIMIT_UDC_1000V) prepare_step_0_4();

                if((*it)->text(0) == LIMIT_UAC_200mV) prepare_step_1_0();
                if((*it)->text(0) == LIMIT_UAC_2V)    prepare_step_1_1();
                if((*it)->text(0) == LIMIT_UAC_20V)   prepare_step_1_2();
                if((*it)->text(0) == LIMIT_UAC_200V)  prepare_step_1_3();
                if((*it)->text(0) == LIMIT_UAC_1000V) prepare_step_1_4();

                if((*it)->text(0) == LIMIT_IDC_200uA) prepare_step_2_0();
                if((*it)->text(0) == LIMIT_IDC_2mA)   prepare_step_2_1();
                if((*it)->text(0) == LIMIT_IDC_20mA)  prepare_step_2_2();
                if((*it)->text(0) == LIMIT_IDC_200mA) prepare_step_2_3();
                if((*it)->text(0) == LIMIT_IDC_2000mA)prepare_step_2_4();
                if((*it)->text(0) == LIMIT_IDC_20A)   prepare_step_2_5();

                if((*it)->text(0) == LIMIT_IAC_2mA)   prepare_step_3_0();
                if((*it)->text(0) == LIMIT_IAC_20mA)  prepare_step_3_1();
                if((*it)->text(0) == LIMIT_IAC_200mA) prepare_step_3_2();
                if((*it)->text(0) == LIMIT_IAC_2000mA)prepare_step_3_3();
                if((*it)->text(0) == LIMIT_IAC_20A)   prepare_step_3_4();

                if((*it)->text(0) == LIMIT_R_200)     prepare_step_4_0();
                if((*it)->text(0) == LIMIT_R_2K)      prepare_step_4_1();
                if((*it)->text(0) == LIMIT_R_20K)     prepare_step_4_2();
                if((*it)->text(0) == LIMIT_R_200K)    prepare_step_4_3();
                if((*it)->text(0) == LIMIT_R_2M)      prepare_step_4_4();
            }
        }
        ++it;
    }
    emit info(QString(tr("Подготовлено %1 шагов калибровки")).arg(answers.count()));
}
//--------------------------------------------------------------------------------
void MainBox::test_connect_calibrator(void)
{
    if(calibrator->port_is_open() == false)
    {
        message_critical(this, tr("Ошибка"), tr("Порт калибратора не открыт!"));
        return;
    }
    emit info(tr("Проверка связи с калибратором"));
    if(check_calibrator())
        emit info(tr("OK"));
}
//--------------------------------------------------------------------------------
bool MainBox::check_calibrator(void)
{
    return calibrator->check_calibrator(sb_repeat->value());
}
//--------------------------------------------------------------------------------
void MainBox::check_cable(int index, bool send_UD0)
{
    if((current_state.cable_in == answers.at(index).cable_in) &&
            (current_state.need_amplifier_U == answers.at(index).need_amplifier_U) &&
            (current_state.need_amplifier_I_DC == answers.at(index).need_amplifier_I_DC) &&
            (current_state.need_amplifier_I_AC == answers.at(index).need_amplifier_I_AC))
        return;

    QString text;

    text.clear();
    text.append(tr("Не забудьте правильно скоммутировать кабели мультиметра.\n"));
    switch(answers.at(index).cable_in)
    {
    case CABLE_IN:
        text.append(tr("Кабель должен быть подключен к гнездам IN и COM\n"));
        break;

    case CABLE_A:
        text.append(tr("Кабель должен быть подключен к гнездам A и COM\n"));
        break;

    case CABLE_mA:
        text.append(tr("Кабель должен быть подключен к гнездам mA и COM\n"));
        break;

    default:
        text.append(tr("Кабель должен быть подключен к гнездам IN и COM\n"));
        break;
    }
    if(answers.at(index).need_amplifier_U)
        text.append(tr("ВНИМАНИЕ! Требуется подключение усилителя напряжения\n"));
    if(answers.at(index).need_amplifier_I_DC)
        text.append(tr("ВНИМАНИЕ! Требуется подключение усилителя тока (DC)\n"
                       "Усилитель калибратора Н4-7 подключается в обратной полярности (подробнее описано в документации)\n"));
    if(answers.at(index).need_amplifier_I_AC)
        text.append(tr("ВНИМАНИЕ! Требуется подключение усилителя тока (AC)\n"));

    if(send_UD0)
    {
        calibrator->set_timeout(sb_time_calibration->value());
        calibrator->reset_U();
    }

    current_state.cable_in = answers.at(index).cable_in;
    current_state.need_amplifier_U = answers.at(index).need_amplifier_U;
    current_state.need_amplifier_I_DC = answers.at(index).need_amplifier_I_DC;
    current_state.need_amplifier_I_AC = answers.at(index).need_amplifier_I_AC;

    text.append(tr("Будет произведено переключение режима калибровки.\n"));
    text.append(tr("Нажмите ОК"));

    calibrator->set_timeout(sb_time_calibration->value());
    calibrator->reset_U();
    msgInfo(text);
}
//--------------------------------------------------------------------------------
void MainBox::set_calibration_point_color(const QString &name,
                                          const QBrush foreground,
                                          const QBrush background)
{
    QTreeWidgetItemIterator limits(ui->treeWidget_limits);
    while (*limits)
    {
        if((*limits)->text(0) == name)
        {
            (*limits)->setForeground(0, foreground);
            (*limits)->setBackground(0, background);
        }
        *limits++;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clr_calibration_point_color(const QString &name)
{
    QTreeWidgetItemIterator limits(ui->treeWidget_limits);
    while (*limits)
    {
        if((*limits)->text(0) == name)
        {
            (*limits)->setBackground(0, QBrush(Qt::NoBrush));
            (*limits)->setForeground(0, QBrush(Qt::NoBrush));
        }
        *limits++;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clr_calibration_point_all(void)
{
    QTreeWidgetItemIterator limits(ui->treeWidget_limits);
    while (*limits)
    {
        (*limits)->setBackground(0, QBrush(Qt::NoBrush));
        (*limits)->setForeground(0, QBrush(Qt::NoBrush));
        *limits++;
    }

}
//--------------------------------------------------------------------------------
void MainBox::start_calibration(void)
{
    clr_calibration_point_all();

    if(is_worked == false)
    {
        is_worked = true;
        ui->btn_Start->setText(tr("Стоп"));
        ui->btn_Start->setIcon(QIcon(":/programm/media-playback-stop.png"));
        controls_enabled(false);
        ui->btn_Start->setEnabled(true);
        cnt_error = 0;
        calibration();
        controls_enabled(true);
        ui->progressBar->setValue(0);
        ui->btn_Start->setText(tr("Старт"));
        ui->btn_Start->setIcon(QIcon(":/programm/media-playback-start.png"));
        is_worked = false;
    }
    else
    {
        is_worked = false;
    }
}
//--------------------------------------------------------------------------------
bool MainBox::calibration(void)
{
    if(is_worked == false) return false;
    prepare_steps();
    if(answers.count() == 0)
    {
        message_critical(this, tr("Ошибка"), tr("Не выбрано ни одного шага калибровки!"));
        return false;
    }

    if(is_worked == false) return false;
    if(calibrator->port_is_open() == false)
    {
        message_critical(this, tr("Ошибка"), tr("Порт калибратора не открыт!"));
        return false;
    }
    if(is_worked == false) return false;

#ifndef CALIBRATION_WIRED
    if(ui->cb_TypeMultimeter->currentIndex() == 2)
    {
        if(multimeter->port_is_open() == false)
        {
            message_critical(this, tr("Ошибка"), tr("Порт мультиметра В7-86 не открыт!"));
            return false;
        }
    }
#endif

    emit info(tr("Старт калибровки"));
#ifndef CALIBRATION_WIRED
    control_log->set_modification(ui->cb_TypeMultimeter->currentText());
#endif
    control_log->set_timeout_calibrator(sb_time_calibration->value());
    control_log->set_timeout_multimeter(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    control_log->set_serial_number(ui->sb_SerNo->value());
    control_log->set_serial_year(ui->sb_Year->value());
#ifndef CALIBRATION_WIRED
    control_log->set_address(ui->sb_address->value());
    control_log->set_channel(ui->sb_channel->value());
#endif
    control_log->set_date(QDate::currentDate().toString());
    control_log->set_time(QTime::currentTime().toString());
    control_log->set_result(tr("Старт калибровки"));
    control_log->save();

#ifdef CALIBRATION_WIRED
    if(true)
#else
    if(ui->cb_TypeMultimeter->currentIndex() == 2)
#endif
    {
        if(is_worked == false) return false;

#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->print_cmd_text(V764_2_CMD_0x04);
        bool ok = multimeter->service_on(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
        if(!ok)
        {
            multimeter->print_last_error();
            return false;
        }

        multimeter->print_cmd_text(V764_2_CMD_0x01);
        ok = multimeter->find_cmd(V764_2_CMD_0x01);
        if(!ok)
        {
            return multimeter->get_last_error();
        }

        if(is_worked == false) return false;
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif
        multimeter->print_cmd_text(V764_2_CMD_0x02);
        ok = multimeter->remote_control_on();
        if(!ok)
        {
            return multimeter->get_last_error();
        }
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(answers.count());
    QTime full_time;
    QTime log_time;
    full_time.start();
    time_log.clear();
    for(int n=0; n<answers.count(); n++)
    {
        if(is_worked == false)
            break;

        ui->progressBar->setValue(n + 1);
        emit info(QString(tr("Выполняется шаг %1.%2.%3"))
                  .arg(answers.at(n).step_a)
                  .arg(answers.at(n).step_b)
                  .arg(answers.at(n).step_c));
        emit debug(QString("%1").arg(answers.at(n).calibrator_question));

#ifndef CALIBRATION_WIRED
        control_log->set_modification(ui->cb_TypeMultimeter->currentText());
#endif
        control_log->set_timeout_calibrator(sb_time_calibration->value());
        control_log->set_timeout_multimeter(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
        control_log->set_serial_number(ui->sb_SerNo->value());
        control_log->set_serial_year(ui->sb_Year->value());
#ifndef CALIBRATION_WIRED
        control_log->set_address(ui->sb_address->value());
        control_log->set_channel(ui->sb_channel->value());
#endif
        control_log->set_date(QDate::currentDate().toString());
        control_log->set_time(QTime::currentTime().toString());
        control_log->set_step(QString("%1.%2.%3")
                              .arg(answers.at(n).step_a)
                              .arg(answers.at(n).step_b)
                              .arg(answers.at(n).step_c));
        control_log->set_result(tr("Выполняется шаг"));
        control_log->set_time_step_ms(-1);
        control_log->save();

        log_time.start();
        int err = work(n, sb_repeat->value());
        if(err != E_NO_ERROR)
        {
            multimeter->print_last_error();
            set_calibration_point_color(answers.at(n).limit_name,
                                        QBrush(Qt::white),
                                        QBrush(Qt::red));
        }
        else
        {
            set_calibration_point_color(answers.at(n).limit_name,
                                        QBrush(Qt::black),
                                        QBrush(Qt::green));
        }

        if(err != E_NO_ERROR)
        {
            if(err == E_ERROR_BREAK)
            {
#ifndef CALIBRATION_WIRED
                control_log->set_modification(ui->cb_TypeMultimeter->currentText());
#endif
                control_log->set_timeout_calibrator(sb_time_calibration->value());
                control_log->set_timeout_multimeter(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
                control_log->set_serial_number(ui->sb_SerNo->value());
                control_log->set_serial_year(ui->sb_Year->value());
#ifndef CALIBRATION_WIRED
                control_log->set_address(ui->sb_address->value());
                control_log->set_channel(ui->sb_channel->value());
#endif
                control_log->set_date(QDate::currentDate().toString());
                control_log->set_time(QTime::currentTime().toString());
                control_log->set_step(QString("%1.%2.%3")
                                      .arg(answers.at(n).step_a)
                                      .arg(answers.at(n).step_b)
                                      .arg(answers.at(n).step_c));
                control_log->set_result(tr("Калибровка отменена!"));
                control_log->save();

                message_critical(this, tr("Ошибка"), tr("Калибровка отменена!"));
                calibrator->set_timeout(sb_time_calibration->value());
                calibrator->reset_U();
                return false;
            }
            if(err != E_ERROR_BREAK)
            {
                if(ui->cb_ignore_error->isChecked() == false)
                {
#ifndef CALIBRATION_WIRED
                    control_log->set_modification(ui->cb_TypeMultimeter->currentText());
#endif
                    control_log->set_timeout_calibrator(sb_time_calibration->value());
                    control_log->set_timeout_multimeter(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
                    control_log->set_serial_number(ui->sb_SerNo->value());
                    control_log->set_serial_year(ui->sb_Year->value());
#ifndef CALIBRATION_WIRED
                    control_log->set_address(ui->sb_address->value());
                    control_log->set_channel(ui->sb_channel->value());
#endif
                    control_log->set_date(QDate::currentDate().toString());
                    control_log->set_time(QTime::currentTime().toString());
                    control_log->set_step(QString("%1.%2.%3")
                                          .arg(answers.at(n).step_a)
                                          .arg(answers.at(n).step_b)
                                          .arg(answers.at(n).step_c));
                    control_log->set_result(tr("Калибровка завершена с ошибкой!"));
                    control_log->save();

                    message_critical(this, tr("Ошибка"), tr("Калибровка завершена с ошибкой!"));
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    return false;
                }
            }
#ifndef CALIBRATION_WIRED
            control_log->set_modification(ui->cb_TypeMultimeter->currentText());
#endif
            control_log->set_timeout_calibrator(sb_time_calibration->value());
            control_log->set_timeout_multimeter(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
            control_log->set_serial_number(ui->sb_SerNo->value());
            control_log->set_serial_year(ui->sb_Year->value());
#ifndef CALIBRATION_WIRED
            control_log->set_address(ui->sb_address->value());
            control_log->set_channel(ui->sb_channel->value());
#endif
            control_log->set_date(QDate::currentDate().toString());
            control_log->set_time(QTime::currentTime().toString());
            control_log->set_step(QString("%1.%2.%3")
                                  .arg(answers.at(n).step_a)
                                  .arg(answers.at(n).step_b)
                                  .arg(answers.at(n).step_c));
            control_log->set_result(QString(tr("Ошибка %1")).arg(err));
            control_log->save();
        }
        else
        {
            struct log_data temp_data;
            temp_data.step = QString("%1.%2.%3")
                    .arg(answers.at(n).step_a)
                    .arg(answers.at(n).step_b)
                    .arg(answers.at(n).step_c);
            temp_data.time = log_time.elapsed();
            time_log.append(temp_data);

#ifndef CALIBRATION_WIRED
            control_log->set_modification(ui->cb_TypeMultimeter->currentText());
#endif
            control_log->set_timeout_calibrator(sb_time_calibration->value());
            control_log->set_timeout_multimeter(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
            control_log->set_serial_number(ui->sb_SerNo->value());
            control_log->set_serial_year(ui->sb_Year->value());
#ifndef CALIBRATION_WIRED
            control_log->set_address(ui->sb_address->value());
            control_log->set_channel(ui->sb_channel->value());
#endif
            control_log->set_date(QDate::currentDate().toString());
            control_log->set_time(QTime::currentTime().toString());
            control_log->set_result("Калибровка шага выполнена успешно");
            control_log->set_step(temp_data.step);
            control_log->set_time_step_ms(temp_data.time);
            control_log->save();

            if(ui->cb_AutoSave->isChecked() && answers.at(n).last_point)
            {
#ifdef CALIBRATION_WIRED
                multimeter->set_serno(ui->sb_SerNo->value());
                multimeter->set_year(ui->sb_Year->value());
                multimeter->set_address(FIXED_ADDRESS);
                multimeter->set_channel(FIXED_CHANNEL);
                multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
                multimeter->set_serno(ui->sb_SerNo->value());
                multimeter->set_year(ui->sb_Year->value());
                multimeter->set_address(ui->sb_address->value());
                multimeter->set_channel(ui->sb_channel->value());
                multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif
                multimeter->print_cmd_text(V764_2_CMD_0x1B);
                bool ok = multimeter->write_to_flash();
                if(!ok)
                {
                    multimeter->print_last_error();
                }
            }
        }
    }
    int full_time_msec = full_time.elapsed();

    calibrator->set_timeout(sb_time_calibration->value());
    calibrator->reset_U();

#ifdef CALIBRATION_WIRED
    if(true)
#else
    if(ui->cb_TypeMultimeter->currentIndex() == 2)
#endif
    {
        if(is_worked == false) return false;
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif
        multimeter->print_cmd_text(V764_2_CMD_0x1B);
        bool ok = multimeter->write_to_flash();
        if(!ok)
        {
            multimeter->print_last_error();
        }

        if(is_worked == false) return false;
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif
        multimeter->print_cmd_text(V764_2_CMD_0x03);
        ok = multimeter->remote_control_off();
        if(!ok)
        {
            multimeter->print_last_error();
        }

        emit info(QString(tr("Прошло времени: %1 сек."))
                  .arg(full_time_msec / 1000.0f));
        emit info(QString(tr("Затрачено времени на шаг (среднее): %1 сек."))
                  .arg(full_time_msec / answers.count() / 1000.0f));
        emit info(QString(tr("Ошибок: %1 "))
                  .arg(cnt_error));

#ifndef CALIBRATION_WIRED
        control_log->set_modification(ui->cb_TypeMultimeter->currentText());
#endif
        control_log->set_timeout_calibrator(sb_time_calibration->value());
        control_log->set_timeout_multimeter(MAX_TIMEOUT_MULTIMETER_CALIBRATION);
        control_log->set_serial_number(ui->sb_SerNo->value());
        control_log->set_serial_year(ui->sb_Year->value());
#ifndef CALIBRATION_WIRED
        control_log->set_address(ui->sb_address->value());
        control_log->set_channel(ui->sb_channel->value());
#endif
        control_log->set_date(QDate::currentDate().toString());
        control_log->set_time(QTime::currentTime().toString());
        control_log->set_step("");
        control_log->set_result(QString(tr("Калибровка выполнена успешно. Шагов: %1. Прошло времени: %2 сек. Затрачено времени на шаг (среднее): %3 сек. Ошибок: %4")
                                        .arg(answers.count())
                                        .arg(full_time_msec / 1000.0f)
                                        .arg(full_time_msec / answers.count() / 1000.0f)
                                        .arg(cnt_error)));
        control_log->set_time_step_ms(0);
        control_log->save();

        QDateTime current = QDateTime::currentDateTime();
        QString temp = QString("%1")
                .arg(current.toString("yyyyMMddhhmmss"));

        QFile file(QString("log/time_log_%1.txt").arg(temp));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            for(int n=0; n<time_log.count(); n++)
            {
                out << "step:" << time_log.at(n).step << " time_ms " << time_log.at(n).time << "\n";
            }
        }
    }
    msgInfo(tr("Калибровка завершена"));
    return true;
}
//--------------------------------------------------------------------------------
int MainBox::work(int index,
                  int max_repeat)
{
    QByteArray temp;
    if(index > answers.count())
    {
        emit error(QString(tr("work: index %1 not correct")).arg(index));
        return E_ERROR_INDEX_NOT_CORRECT;
    }

    for(int i=0; i<max_repeat; i++)
    {
        if(check_calibrator())
        {
            if(answers.at(index).calibrator_question_before.isEmpty() == false)
            {
                calibrator->step_to_step(answers.at(index).calibrator_question_before,
                                         sb_delay->value(),
                                         sb_time_calibration->value(),
                                         false);
            }

            if((answers.at(index).step_a == 3) && (answers.at(index).step_b == 4))
                check_cable(index, false);
            else
                check_cable(index, true);

            temp.clear();
            temp.append(QString("%1").arg(answers.at(index).calibrator_question));

            if(answers.at(index).calibrator_step_by_step)
            {
                if((answers.at(index).step_a == 0) && (answers.at(index).step_b == 4))
                {
                    if(type_calibrator == H4_6)
                    {
                        calibrator->step_to_step(answers.at(index).calibrator_question,
                                                 sb_delay->value(),
                                                 sb_time_calibration->value(),
                                                 false);
                    }
                    if(type_calibrator == H4_7)
                    {
                        spike_nail_1000V(); // мега костыль - особенность калибратора Н4-7
                    }
                }
                else
                {
                    calibrator->step_to_step(answers.at(index).calibrator_question,
                                             sb_delay->value(),
                                             sb_time_calibration->value(),
                                             false);
                }
            }
            else
            {
#ifdef NEW_CALIBRATION_TIME
                old_step = new_step;
                new_step.step_a = answers.at(index).step_a;
                new_step.step_b = answers.at(index).step_b;
                new_step.step_c = answers.at(index).step_c;
                int calibrator_timeout = 0;
                if((old_step.step_a == new_step.step_a) && (old_step.step_b == new_step.step_b))
                    calibrator_timeout = sb_time_calibration->value() / 2;
                else
                    calibrator_timeout = sb_time_calibration->value();
                calibrator->set_timeout(calibrator_timeout);
                calibrator->send(temp);
#else
                calibrator->send(temp);
#endif
            }

            if(check_calibrator())
            {
                if(type_calibrator == H4_6)
                {
                    int a = answers.at(index).step_a;
                    int b = answers.at(index).step_b;
                    int c = answers.at(index).step_c;
                    int x = a*100 + b*10 + c;
                    if((x == 340) && (x == 345))
                    {
                        msgInfo(tr("Отожмите перегрузку"));
                    }
                }
#ifdef CALIBRATION_WIRED
                if(true)
#else
                if(ui->cb_TypeMultimeter->currentIndex() == 2)
#endif
                {
                    bool ok = multimeter->find_cmd(answers.at(index).cmd);
                    if(!ok)
                    {
                        multimeter->print_last_error();
                    }
                    else
                    {
                        emit info("OK");
                        return E_NO_ERROR;
                    }
                }
            }
            else
            {
                int button = message_question(this,
                                              tr("Ручная калибровка"),
                                              QString(tr("Выполняется шаг %1.%2.%3."))
                                              .arg(answers.at(index).step_a)
                                              .arg(answers.at(index).step_b)
                                              .arg(answers.at(index).step_c));
                if(button == QMessageBox::Yes)
                    return E_NO_ERROR;
                else
                    return E_ERROR_BREAK;
            }
        }
    }
    return E_ERROR_MAX_REPEAT;
}
//--------------------------------------------------------------------------------
float MainBox::return_freq(unsigned int step)
{
    if(step < 3)
        return (float)20 / (float)1000;
    if(step == 3)
        return (float)50 / (float)1000;
    if((step > 3) && (step < 14))
        return (float)((step-3)*100) / (float)1000;
    if((step > 13) && (step < 31))
        return (float)(1500 + ((step-14)*500)) / (float)1000;
    if((step > 30) && (step < 50))
        return (float)(10000 + ((step-31)*5000)) / (float)1000;

    emit error(QString(tr("Недопустимый шаг %1")).arg(step));
    return -1;
}
//--------------------------------------------------------------------------------
#ifdef NEW_POINTS
float MainBox::return_freq_new_points(unsigned int step)
{
    switch(step)
    {
    case 0: return (float)20 / (float)1000;
    case 1: return (float)20 / (float)1000;
    case 2: return (float)20 / (float)1000;
    case 3: return (float)50 / (float)1000;
    case 4: return (float)100 / (float)1000;
    case 5: return (float)1000 / (float)1000;
    case 6: return (float)20000 / (float)1000;
    case 7: return (float)50000 / (float)1000;
    case 8: return (float)100000 / (float)1000;
    }

    emit error(QString(tr("(return_freq_new)Недопустимый шаг %1")).arg(step));
    return -1;
}
#endif
//--------------------------------------------------------------------------------
QString MainBox::prepare_string(const QString &prefix,
                                float value,
                                float limit,
                                int step)
{
    QString temp;
    float freq = 0.0f;

    if(step == 0)
    {
        value = limit / 10.0f;
        freq = 20.0f;
    }
    if(step == 1)
    {
        value = limit;
        freq = 20.0f;
    }
    if(step == 2)
    {
        freq = 20.0f;
    }
    if(step == 3)
    {
        freq = 50.0f;
    }
    if(step >= 4 && step <=13)
    {
        freq = 100.0f + (step - 4) * 100.f;
    }
    if(step >= 14 && step <=31)
    {
        freq = 1500.0f + (step - 14) * 500.f;
    }
    if(step >= 32 && step <=49)
    {
        freq = 15000.0f + (step - 32) * 5000.f;
    }

    temp.append(QString("%1%2F%3\r\n")
                .arg(prefix)
                .arg(value)
                .arg(freq / 1000.0f));
    return temp;
}
//--------------------------------------------------------------------------------
#ifdef NEW_POINTS
QString MainBox::prepare_string_new_points(const QString &prefix,
                                           float value,
                                           float limit,
                                           int step)
{
    QString temp;
    float freq = 0.0f;

    switch(step)
    {
    case 0: //20 Hz
        value = limit / 10.0f;
        freq = 20.0f;
        break;

    case 1: //20 Hz
        value = limit;
        freq = 20.0f;
        break;

    case 2: //20 Hz
        freq = 20.0f;
        break;

    case 3: //50 Hz
        freq = 50.0f;
        break;

    case 4: //100 Hz
        freq = 100.0f;
        break;

    case 5: //1 kHz
        freq = 1000.0f;
        break;

    case 6: //10 kHz
        freq = 10000.0f;
        break;

    case 7: //20 kHz
        freq = 20000.0f;
        break;

    case 8: //50 kHz
        freq = 50000.0f;
        break;

    case 9: //100 kHz
        freq = 100000.0f;
        break;

    default:
        emit error(QString("unknown step %1").arg(step));
        return "";
    }

    temp.append(QString("%1%2F%3\r\n")
                .arg(prefix)
                .arg(value)
                .arg(freq / 1000.0f));
    return temp;
}
#endif
//--------------------------------------------------------------------------------
QString MainBox::prepare_string_adv(const QString &prefix,
                                    float value,
                                    float limit,
                                    int step)
{
    Q_UNUSED(limit)

    QString temp;
    float freq = 0.0f;

    if(step == 0)
    {
        value = value / 10.0f;
        freq = 20.0f;
    }
    if(step == 1)
    {
        value = value;
        freq = 20.0f;
    }
    if(step == 2)
    {
        freq = 20.0f;
    }
    if(step == 3)
    {
        freq = 50.0f;
    }
    if(step >= 4 && step <=13)
    {
        freq = 100.0f + (step - 4) * 100.f;
    }
    if(step >= 14 && step <=31)
    {
        freq = 1500.0f + (step - 14) * 500.f;
    }
    if(step >= 32 && step <=49)
    {
        freq = 15000.0f + (step - 32) * 5000.f;
    }

    temp.append(QString("%1%2F%3\r\n")
                .arg(prefix)
                .arg(value)
                .arg(freq / 1000.0f));
    return temp;
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_0_0(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_UDC_200mV;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="UD0.19999\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 0;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="UD0.19999\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 0;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_0_1(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_UDC_2V;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="UD1.9999\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 1;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="UD1.9999\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 1;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_0_2(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_UDC_20V;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="UD19.999\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 2;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="UD19.999\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 2;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_0_3(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_UDC_200V;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="UD199.99\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 3;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="UD199.99\r\n";
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 3;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_0_4(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;
    QString temp;

    a_data.limit_name = LIMIT_UDC_1000V;
    switch(type_calibrator)
    {
    case H4_6:
        temp.clear();
        temp.append(H4_6_KEY_V);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(H4_6_KEY_1);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(H4_6_KEY_OFF);

        a_data.calibrator_question=temp;
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = true;
        a_data.need_amplifier_U = true;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 4;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        temp.clear();
        temp.append(H4_7_KEY_V);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_1);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_OFF);

        a_data.calibrator_question=temp;
        a_data.cmd = V764_2_CMD_0x13;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = true;
        a_data.need_amplifier_U = true;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 0;
        a_data.step_b = 4;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_1_0(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

#ifndef NEW_POINTS
    int max_step = 49;
#else
    int max_step = 9;
#endif

    a_data.limit_name = LIMIT_UAC_200mV;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("UA", 0.199f, 0.20f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("UA", 0.199f, 0.20f, n);
#endif
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 0;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("UA", 0.199f, 0.20f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("UA", 0.199f, 0.20f, n);
#endif
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 0;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_1_1(void)
{
    questions_data a_data;

    QString before_string;
    before_string.clear();

#ifndef NEW_POINTS
    int max_step = 49;
#else
    int max_step = 9;
#endif

    a_data.limit_name = LIMIT_UAC_2V;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("UA", 1.99f, 2.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("UA", 1.99f, 2.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 1;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("UA", 1.99f, 2.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("UA", 1.99f, 2.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 1;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_1_2(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

#ifndef NEW_POINTS
    int max_step = 49;
#else
    int max_step = 9;
#endif

    a_data.limit_name = LIMIT_UAC_20V;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("UA", 19.9f, 20.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("UA", 19.9f, 20.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 2;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("UA", 19.9f, 20.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("UA", 19.9f, 20.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 2;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_1_3(void)
{
    QString before_string;
    before_string.clear();

    if(type_calibrator == H4_6)
    {
        emit error(tr("Шаг 1.3 нельзя выполнить на H4-6"));
        return;
    }

    questions_data a_data;
    QString temp;
    float freq;

#ifndef NEW_POINTS
    int max_step = 49;
#else
    int max_step = 9;
#endif

    a_data.limit_name = LIMIT_UAC_20V;
    if(type_calibrator == H4_7)
    {
        for(int n=0; n<=max_step; n++)
        {
            switch(n)
            {
            case 0:
                temp.clear();
                temp.append(H4_7_KEY_V);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_ENTER_VI);
                temp.append(H4_7_KEY_2);
                temp.append(H4_7_KEY_0);
                temp.append(H4_7_KEY_ENTER_VI);
#ifndef NEW_POINTS
                freq = return_freq(n);
#else
                freq = return_freq_new_points(n);
#endif
                if(freq > 0)
                    temp.append(QString("%1").arg(return_freq_string_H4_7(freq)));
                temp.append(H4_7_KEY_OFF);
                a_data.calibrator_question=temp;
                break;

            case 1:
                temp.clear();
                temp.append(H4_7_KEY_V);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_ENTER_VI);
                temp.append(H4_7_KEY_2);
                temp.append(H4_7_KEY_0);
                temp.append(H4_7_KEY_0);
                temp.append(H4_7_KEY_ENTER_VI);
#ifndef NEW_POINTS
                freq = return_freq(n);
#else
                freq = return_freq_new_points(n);
#endif
                if(freq > 0)
                    temp.append(QString("%1").arg(return_freq_string_H4_7(freq)));
                temp.append(H4_7_KEY_OFF);
                a_data.calibrator_question=temp;
                break;

            default:
                temp.clear();
                temp.append(H4_7_KEY_V);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_ENTER_VI);
                temp.append(H4_7_KEY_1);
                temp.append(H4_7_KEY_9);
                temp.append(H4_7_KEY_9);
                temp.append(H4_7_KEY_ENTER_VI);
#ifndef NEW_POINTS
                freq = return_freq(n);
#else
                freq = return_freq_new_points(n);
#endif
                if(freq > 0)
                    temp.append(QString("%1").arg(return_freq_string_H4_7(freq)));
                temp.append(H4_7_KEY_OFF);
                a_data.calibrator_question=temp;
                break;
            }
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = true;
            a_data.need_amplifier_U = true;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 3;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_1_4(void)
{
    QString before_string;
    before_string.clear();

    if(type_calibrator == H4_6)
    {
        emit error(tr("Шаг 1.4 нельзя выполнить на H4-6"));
        return;
    }

    questions_data a_data;
    QString temp;

#ifndef NEW_POINTS
    int max_step = 34;
#else
    int max_step = 7;
#endif

    a_data.limit_name = LIMIT_UAC_200V;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<max_step; n++)
        {
            if(n == 0)
            {
                temp.clear();
                temp.append(H4_6_KEY_V);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_ENTER_VI);
                temp.append(H4_6_KEY_7);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_ENTER_VI);
#ifndef NEW_POINTS
                temp.append(QString("%1").arg(return_freq_string_H4_6(return_freq(n), true)));
#else
                temp.append(QString("%1").arg(return_freq_string_H4_6(return_freq_new_points(n), true)));
#endif
                temp.append(H4_6_KEY_OFF);
                a_data.calibrator_question=temp;
            }
            else
            {
                temp.clear();
                temp.append(H4_6_KEY_V);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_RIGHT);
                temp.append(H4_6_KEY_ENTER_VI);
                temp.append(H4_6_KEY_7);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_ENTER_VI);
#ifndef NEW_POINTS
                temp.append(QString("%1").arg(return_freq_string_H4_6(return_freq(n), true)));
#else
                temp.append(QString("%1").arg(return_freq_string_H4_6(return_freq_new_points(n), true)));
#endif
                temp.append(H4_6_KEY_OFF);
                a_data.calibrator_question=temp;
            }
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = true;
            a_data.need_amplifier_U = true;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 4;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<max_step; n++)
        {
            if(n == 0)
            {
                temp.clear();
                temp.append(H4_7_KEY_V);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_ENTER_VI);
                temp.append(H4_7_KEY_7);
                temp.append(H4_7_KEY_0);
                temp.append(H4_7_KEY_ENTER_VI);
#ifndef NEW_POINTS
                temp.append(QString("%1").arg(return_freq_string_H4_7(return_freq(n))));
#else
                temp.append(QString("%1").arg(return_freq_string_H4_7(return_freq_new_points(n))));
#endif
                temp.append(H4_7_KEY_OFF);
                a_data.calibrator_question=temp;
            }
            else
            {
                temp.clear();
                temp.append(H4_7_KEY_V);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_RIGHT);
                temp.append(H4_7_KEY_ENTER_VI);
                temp.append(H4_7_KEY_7);
                temp.append(H4_7_KEY_0);
                temp.append(H4_7_KEY_0);
                temp.append(H4_7_KEY_ENTER_VI);
#ifndef NEW_POINTS
                temp.append(QString("%1").arg(return_freq_string_H4_7(return_freq(n))));
#else
                temp.append(QString("%1").arg(return_freq_string_H4_7(return_freq_new_points(n))));
#endif
                temp.append(H4_7_KEY_OFF);
                a_data.calibrator_question=temp;
            }
            a_data.cmd = V764_2_CMD_0x14;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = true;
            a_data.need_amplifier_U = true;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 4;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_2_0(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_IDC_200uA;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="ID0.19999\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 0;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="ID0.19999\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 0;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_2_1(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_IDC_2mA;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="ID1.9999\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 1;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="ID1.9999\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 1;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_2_2(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_IDC_20mA;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="ID19.999\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 2;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="ID19.999\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 2;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_2_3(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_IDC_200mA;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="ID199.99\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 3;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="ID199.99\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 3;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_2_4(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_IDC_2000mA;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="ID1999.9\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 4;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="ID1999.9\r\n";
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_mA;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 4;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_2_5(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;
    QString temp;

    a_data.limit_name = LIMIT_UDC_20V;
    switch(type_calibrator)
    {
    case H4_6:
        temp.clear();
        temp.append(H4_6_KEY_I);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(H4_6_KEY_1);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(H4_6_KEY_OFF);

        a_data.calibrator_question=temp;
        a_data.calibrator_step_by_step = true;
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_A;
        a_data.calibrator_type = H4_6;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = true;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 5;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        temp.clear();
        temp.append("UD10\r\n");

        a_data.calibrator_question=temp;
        a_data.calibrator_step_by_step = false;
        a_data.cmd = V764_2_CMD_0x15;
        a_data.cable_in = CABLE_A;
        a_data.calibrator_type = H4_7;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = true;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 2;
        a_data.step_b = 5;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_3_0(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

#ifndef NEW_POINTS
    int max_step = 31;
#else
    int max_step = 6;
#endif

    a_data.limit_name = LIMIT_IAC_2mA;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 1.99f, 2.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 1.99f, 2.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 0;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 1.99f, 2.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 1.99f, 2.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 0;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_3_1(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

#ifndef NEW_POINTS
    int max_step = 31;
#else
    int max_step = 6;
#endif

    a_data.limit_name = LIMIT_IAC_20mA;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 19.90f, 20.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 19.90f, 20.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 1;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 19.90f, 20.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 19.90f, 20.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 1;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_3_2(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

#ifndef NEW_POINTS
    int max_step = 31;
#else
    int max_step = 6;
#endif

    a_data.limit_name = LIMIT_IAC_200mA;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 199.0f, 200.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 199.0f, 200.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 2;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 199.0f, 200.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 199.0f, 200.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 2;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_3_3(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

#ifndef NEW_POINTS
    int max_step = 31;
#else
    int max_step = 6;
#endif

    a_data.limit_name = LIMIT_IAC_2000mA;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 1990.0f, 2000.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 1990.0f, 2000.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 3;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=max_step; n++)
        {
#ifndef NEW_POINTS
            a_data.calibrator_question=prepare_string("IA", 1990.0f, 2000.0f, n);
#else
            a_data.calibrator_question=prepare_string_new_points("IA", 1990.0f, 2000.0f, n);
#endif
            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_mA;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 3;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_3_4(void)
{
    QString before_string;
    before_string.clear();

    QString temp;
    questions_data a_data;

#ifndef NEW_POINTS
    int max_step = 14;
#else
    int max_step = 5;
#endif

    a_data.limit_name = LIMIT_UAC_1000V;
    switch(type_calibrator)
    {
    case H4_6:
        for(int n=0; n<max_step; n++)
        {
            if(n == 0)
            {
                temp.clear();
                temp.append(H4_6_KEY_ENTER_VI);
                temp.append(H4_6_KEY_1);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_ENTER_VI);
                temp.append(QString("%1").arg(return_freq_string_H4_6(return_freq(n), false)));
                a_data.calibrator_question=temp;
            }
            else
            {
                temp.clear();
                temp.append(H4_6_KEY_ENTER_VI);
                temp.append(H4_6_KEY_1);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_0);
                temp.append(H4_6_KEY_ENTER_VI);
                temp.append(QString("%1").arg(return_freq_string_H4_6(return_freq(n), false)));
                a_data.calibrator_question=temp;
            }

            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_A;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = true;
            switch(n)
            {
            case 0:
                before_string.clear();
                before_string.append(H4_6_KEY_I);
                before_string.append(H4_6_KEY_RIGHT);
                before_string.append(H4_6_KEY_RIGHT);
                before_string.append(H4_6_KEY_RIGHT);
                before_string.append(H4_6_KEY_RIGHT);
                before_string.append(H4_6_KEY_ENTER_VI);
                before_string.append(H4_6_KEY_0);
                before_string.append(H4_6_KEY_ENTER_VI);
                before_string.append(H4_6_KEY_Hz);
                before_string.append(H4_6_KEY_ENTER_F);
                before_string.append(H4_6_KEY_2);
                before_string.append(H4_6_KEY_0);
                before_string.append(H4_6_KEY_ENTER_F);
                before_string.append(H4_6_KEY_OFF);
                a_data.calibrator_question_before = before_string;
                break;

            case 5:
                before_string.clear();
                before_string.append(H4_6_KEY_ENTER_F);
                before_string.append(H4_6_KEY_1);
                before_string.append(H4_6_KEY_ENTER_F);
                before_string.append(H4_6_KEY_kHz);
                before_string.append(H4_6_KEY_OFF);
                a_data.calibrator_question_before = before_string;
                break;

            default:
                a_data.calibrator_question_before.clear();
                break;
            }
            a_data.step_a = 3;
            a_data.step_b = 4;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<max_step; n++)
        {
            switch(n)
            {
            case 0:
            case 1:
                a_data.calibrator_question=prepare_string_adv("UA", 10.0f, 20.0f, n);
                break;

            default:
                a_data.calibrator_question=prepare_string("UA", 10.0f, 20.0f, n);
                break;
            }

            a_data.cmd = V764_2_CMD_0x16;
            a_data.cable_in = CABLE_A;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = true;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 3;
            a_data.step_b = 4;
            a_data.step_c = n;
            a_data.last_point = (n == max_step);

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_4_0(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_R_200;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="R1\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 0;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="R1\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 0;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_4_1(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_R_2K;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="R2\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 1;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="R2\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 1;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_4_2(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_R_20K;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="R3\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 2;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="R3\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 2;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_4_3(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_R_200K;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="R4\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 3;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="R4\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 3;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_4_4(void)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    a_data.limit_name = LIMIT_R_2M;
    switch(type_calibrator)
    {
    case H4_6:
        a_data.calibrator_question="R5\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_6;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 4;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    case H4_7:
        a_data.calibrator_question="R5\r\n";
        a_data.cmd = V764_2_CMD_0x17;
        a_data.cable_in = CABLE_IN;
        a_data.calibrator_type = H4_7;
        a_data.calibrator_step_by_step = false;
        a_data.need_amplifier_U = false;
        a_data.need_amplifier_I_DC = false;
        a_data.need_amplifier_I_AC = false;
        a_data.calibrator_question_before = before_string;
        a_data.step_a = 4;
        a_data.step_b = 4;
        a_data.step_c = 0;
        a_data.last_point = true;

        answers.append(a_data);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::set_limits_all_on(void)
{
    QTreeWidgetItemIterator it(ui->treeWidget_limits);
    while (*it)
    {
        if((*it)->isDisabled() == false)
            (*it)->setCheckState(0, Qt::Checked);
        ++it;
    }
    ui->treeWidget_limits->expandAll();
}
//--------------------------------------------------------------------------------
void MainBox::set_limits_all_off(void)
{
    QTreeWidgetItemIterator it(ui->treeWidget_limits);
    while (*it)
    {
        if((*it)->isDisabled() == false)
            (*it)->setCheckState(0, Qt::Unchecked);
        ++it;
    }
    ui->treeWidget_limits->collapseAll();
}
//--------------------------------------------------------------------------------
void MainBox::set_controls_all_on(void)
{
    QTreeWidgetItemIterator it_U(ui->treeWidget_controls_U);
    QTreeWidgetItemIterator it_I(ui->treeWidget_controls_I);
    QTreeWidgetItemIterator it_O(ui->treeWidget_controls_Other);
    QTreeWidgetItemIterator it_HLU(ui->treeWidget_controls_Hi_Low_U);
    while (*it_U)
    {
        if((*it_U)->isDisabled() == false)
            (*it_U)->setCheckState(0, Qt::Checked);
        ++it_U;
    }
    while (*it_I)
    {
        if((*it_I)->isDisabled() == false)
            (*it_I)->setCheckState(0, Qt::Checked);
        ++it_I;
    }
    while (*it_O)
    {
        if((*it_O)->isDisabled() == false)
            (*it_O)->setCheckState(0, Qt::Checked);
        ++it_O;
    }
    while (*it_HLU)
    {
        if((*it_HLU)->isDisabled() == false)
            (*it_HLU)->setCheckState(0, Qt::Checked);
        ++it_HLU;
    }
    ui->treeWidget_controls_U->expandAll();
    ui->treeWidget_controls_I->expandAll();
    ui->treeWidget_controls_Other->expandAll();
    ui->treeWidget_controls_Hi_Low_U->expandAll();
}
//--------------------------------------------------------------------------------
void MainBox::set_controls_all_off(void)
{
    QTreeWidgetItemIterator it_U(ui->treeWidget_controls_U);
    QTreeWidgetItemIterator it_I(ui->treeWidget_controls_I);
    QTreeWidgetItemIterator it_O(ui->treeWidget_controls_Other);
    QTreeWidgetItemIterator it_HLU(ui->treeWidget_controls_Hi_Low_U);
    while (*it_U)
    {
        if((*it_U)->isDisabled() == false)
            (*it_U)->setCheckState(0, Qt::Unchecked);
        ++it_U;
    }
    while (*it_I)
    {
        if((*it_I)->isDisabled() == false)
            (*it_I)->setCheckState(0, Qt::Unchecked);
        ++it_I;
    }
    while (*it_O)
    {
        if((*it_O)->isDisabled() == false)
            (*it_O)->setCheckState(0, Qt::Unchecked);
        ++it_O;
    }
    while (*it_HLU)
    {
        if((*it_HLU)->isDisabled() == false)
            (*it_HLU)->setCheckState(0, Qt::Unchecked);
        ++it_HLU;
    }
    ui->treeWidget_controls_U->collapseAll();
    ui->treeWidget_controls_I->collapseAll();
    ui->treeWidget_controls_Other->collapseAll();
    ui->treeWidget_controls_Hi_Low_U->collapseAll();
}
//--------------------------------------------------------------------------------
void MainBox::controls_enabled(bool state)
{
    ui->btn_Start->setEnabled(state);
    ui->treeWidget_limits->setEnabled(state);
    ui->btn_limits_All_On->setEnabled(state);
    ui->btn_limits_All_Off->setEnabled(state);
    ui->btn_controls_All_On->setEnabled(state);
    ui->btn_controls_All_Off->setEnabled(state);
#ifndef CALIBRATION_WIRED
    ui->sb_address->setEnabled(state);
    ui->sb_channel->setEnabled(state);
#endif
    ui->btn_Test_Connect_V786->setEnabled(state);
    sb_time_calibration->setEnabled(state);

#ifndef CALIBRATION_WIRED
    ui->cb_TypeMultimeter->setEnabled(state);
#endif
    ui->sb_SerNo->setEnabled(state);
    ui->sb_Year->setEnabled(state);
    sb_repeat->setEnabled(state);
    sb_delay->setEnabled(state);
    ui->cb_Step_Report->setEnabled(state);
    ui->btn_clear->setEnabled(state);
    ui->btn_CheckMultimeter->setEnabled(state);
    ui->btn_Report->setEnabled(state);

    ui->treeWidget_controls_U->setEnabled(state);
    ui->treeWidget_controls_I->setEnabled(state);
    ui->treeWidget_controls_Other->setEnabled(state);
    ui->treeWidget_controls_Hi_Low_U->setEnabled(state);

    btn_find_devices->setEnabled(state);

    if(!state)
    {
        ui->treeWidget_limits->clearSelection();
        ui->treeWidget_controls_U->clearSelection();
        ui->treeWidget_controls_I->clearSelection();
        ui->treeWidget_controls_Other->clearSelection();
        ui->treeWidget_controls_Hi_Low_U->clearSelection();
    }
}
//--------------------------------------------------------------------------------
int MainBox::check_DC_mV(float value_mV,
                         float max_delta_mV,
                         unsigned char limit,
                         float *result_delta_mV,
                         bool is_auto_check)
{
    bool ok = false;
    int err = E_NO_ERROR;

    emit info(QString(tr("Производится измерение постоянного напряжения %1mV с допустимой погрешностью %2mV"))
              .arg(value_mV)
              .arg(max_delta_mV));

    //???
    double value = 0;
    ok = multimeter->measuring_UDC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    calibrator->set_timeout(sb_time_calibration->value());
    ok = calibrator->set_U_V(value_mV / 1000.0f);
    if(!ok)
    {
        calibrator->print_last_error();
        return err;
    }

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x06);
        bool ok = multimeter->measuring_UDC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_mV = value - value_mV;

        unsigned char m_limit = multimeter->get_limit(V764_2_CMD_0x06, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x06 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_mV) > max_delta_mV)
        {
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_mV)
                       .arg(max_delta_mV));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 мВ)")).arg(value_mV));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_DC_V(float value_V,
                        float max_delta_mV,
                        unsigned char limit,
                        float *result_delta_mV,
                        bool is_auto_check)
{
    bool ok = false;
    QByteArray ba;

    emit info(QString(tr("Производится измерение постоянного напряжения %1V с допустимой погрешностью %2mV"))
              .arg(value_V)
              .arg(max_delta_mV));

    //???
    double value = 0;
    ok = multimeter->measuring_UDC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    ba.clear();
    calibrator->set_timeout(sb_time_calibration->value());
    ok = calibrator->set_U_V(value_V);
    if(!ok)
    {
        calibrator->print_last_error();
        return E_UNKNOWN_ERROR;
    }

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x06);
        double value = 0;
        bool ok = multimeter->measuring_UDC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_mV = (value - value_V) * 1000.0f;

        int m_limit = multimeter->get_limit(V764_2_CMD_0x06, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x06 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_mV) > max_delta_mV)
        {
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_mV)
                       .arg(max_delta_mV));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 В)")).arg(value_V));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_AC_mV(float value_mV,
                         float max_delta_mV,
                         float freq_kHz,
                         unsigned char limit,
                         float *result_delta_mV,
                         bool is_auto_check)
{
    bool ok = false;
    emit info(QString(tr("Производится измерение переменного напряжения %1mV частотой %2 kHz с допустимой погрешностью %3mV"))
              .arg(value_mV)
              .arg(freq_kHz)
              .arg(max_delta_mV));

    //???
    double value = 0;
    ok = multimeter->measuring_UAC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    calibrator->set_timeout(sb_time_calibration->value());
    ok = calibrator->set_U_V(value_mV / 1000.0f, freq_kHz);
    if(!ok)
    {
        calibrator->print_last_error();
        return E_UNKNOWN_ERROR;
    }
    if(check_calibrator() == false)
        return E_UNKNOWN_ERROR;

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x07);
        double value = 0;
        bool ok = multimeter->measuring_UAC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_mV = value - value_mV;

        int m_limit = multimeter->get_limit(V764_2_CMD_0x07, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x07 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_mV) > max_delta_mV)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_mV)
                       .arg(max_delta_mV));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 мВ)")).arg(value_mV));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_AC_V(float value_V,
                        float max_delta_mV,
                        float freq_kHz,
                        unsigned char limit,
                        float *result_delta_mV,
                        bool is_auto_check)
{
    bool ok = false;
    emit info(QString(tr("Производится измерение переменного напряжения %1V частотой %2 kHz с допустимой погрешностью %3mV"))
              .arg(value_V)
              .arg(freq_kHz)
              .arg(max_delta_mV));

    //???
    double value = 0;
    ok = multimeter->measuring_UAC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    calibrator->set_timeout(sb_time_calibration->value());
    ok = calibrator->set_U_V(value_V, freq_kHz);
    if(!ok)
    {
        calibrator->print_last_error();
        return E_UNKNOWN_ERROR;
    }

    if(check_calibrator() == false)
        return E_UNKNOWN_ERROR;

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x07);
        double value = 0;
        bool ok = multimeter->measuring_UAC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_mV = (value - value_V) * 1000.0f;

        int m_limit = multimeter->get_limit(V764_2_CMD_0x07, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x07 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_mV) > max_delta_mV)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_mV)
                       .arg(max_delta_mV));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 В %2 кГц)"))
                                      .arg(value_V)
                                      .arg(freq_kHz));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_DC_mkA(float value_mkA,
                          float max_delta_mkA,
                          unsigned char limit,
                          float *result_delta_mkA,
                          bool is_auto_check)
{
    bool ok = false;
    int err = E_NO_ERROR;

    emit info(QString(tr("Производится измерение постоянного тока %1mkA с допустимой погрешностью %2mkA"))
              .arg(value_mkA)
              .arg(max_delta_mkA));

    //???
    double value = 0;
    ok = multimeter->measuring_IDC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    calibrator->set_timeout(sb_time_calibration->value());
    ok = calibrator->set_I_mA(value_mkA / 1000.0f);
    if(!ok)
    {
        calibrator->print_last_error();
        return E_UNKNOWN_ERROR;
    }

    if(check_calibrator() == false)
        return E_UNKNOWN_ERROR;

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x08);
        double value = 0;
        bool ok = multimeter->measuring_IDC(&value);
        if(!ok)
        {
            multimeter->print_last_error();
            return err;
        }
        *result_delta_mkA = value - value_mkA;

        int m_limit = multimeter->get_limit(V764_2_CMD_0x08, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x08 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        float max_result_delta_mkA = value_mkA + max_delta_mkA;
        float min_result_delta_mkA = value_mkA - max_delta_mkA;
        if(value > max_result_delta_mkA)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %1 max_delta = %2"))
                       .arg(value - value_mkA)
                       .arg(max_delta_mkA));
            return E_INVALID_DELTA;
        }
        if(value < min_result_delta_mkA)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %1 max_delta = %2"))
                       .arg(value_mkA - value)
                       .arg(max_delta_mkA));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 мкА)")).arg(value_mkA));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_DC_mA(float value_mA,
                         float max_delta_mA,
                         unsigned char limit,
                         float *result_delta_mA,
                         bool is_auto_check)
{
    bool ok = false;
    emit info(QString(tr("Производится измерение постоянного тока %1mA с допустимой погрешностью %2mA"))
              .arg(value_mA)
              .arg(max_delta_mA));

    //???
    double value = 0;
    ok = multimeter->measuring_IDC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    calibrator->set_timeout(sb_time_calibration->value());
    ok = calibrator->set_I_mA(value_mA);
    if(!ok)
    {
        calibrator->print_last_error();
        return E_UNKNOWN_ERROR;
    }

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x08);
        double value = 0;
        ok = multimeter->measuring_IDC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_mA = value - value_mA;

        int m_limit = multimeter->get_limit(V764_2_CMD_0x08, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x08 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_mA) > max_delta_mA)
        {
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_mA)
                       .arg(max_delta_mA));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 мА)")).arg(value_mA));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_AC_mA(float value_mA,
                         float max_delta_mA,
                         float freq_kHz,
                         unsigned char limit,
                         float *result_delta_mA,
                         bool is_auto_check)
{
    bool ok = false;
    emit info(QString(tr("Производится измерение переменного тока %1mA частотой %2 kHz с допустимой погрешностью %3mA"))
              .arg(value_mA)
              .arg(freq_kHz)
              .arg(max_delta_mA));

    //???
    double value = 0;
    ok = multimeter->measuring_IAC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    calibrator->set_timeout(sb_time_calibration->value());
    ok = calibrator->set_I_mA(value_mA, freq_kHz);
    if(!ok)
    {
        calibrator->print_last_error();
        return E_UNKNOWN_ERROR;
    }
    if(check_calibrator() == false)
        return E_UNKNOWN_ERROR;

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x09);
        double value = 0;
        bool ok = multimeter->measuring_IAC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_mA = value - value_mA;

        int m_limit = multimeter->get_limit(V764_2_CMD_0x09, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x09 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_mA) > max_delta_mA)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_mA)
                       .arg(max_delta_mA));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 мА %2 кГц)"))
                                      .arg(value_mA)
                                      .arg(freq_kHz));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_AC_A(float value_A,
                        float max_delta_mA,
                        float freq_kHz,
                        unsigned char limit,
                        float *result_delta_mA,
                        bool is_auto_check)
{
    bool ok = false;
    QString temp;

    emit info(QString(tr("Производится измерение переменного тока %1A частотой %2 kHz с допустимой погрешностью %3A"))
              .arg(value_A)
              .arg(freq_kHz)
              .arg(max_delta_mA / 1000.0f));

    //???
    double value = 0;
    ok = multimeter->measuring_IAC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    temp.clear();
    if(type_calibrator == H4_6)
    {
        temp.append(H4_6_KEY_I);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(H4_6_KEY_1);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(QString("%1").arg(return_freq_string_H4_6(freq_kHz, true)));
        temp.append(H4_6_KEY_OFF);
    }
    if(type_calibrator == H4_7)
    {
        temp.append(H4_7_KEY_I);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_RIGHT);
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(H4_7_KEY_1);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_0);
        temp.append(H4_7_KEY_ENTER_VI);
        temp.append(QString("%1").arg(return_freq_string_H4_7(freq_kHz)));
        temp.append(H4_7_KEY_OFF);
    }

    ok = calibrator->step_to_step(temp,
                                  sb_delay->value(),
                                  sb_time_calibration->value(),
                                  false);

    if(ok == false)
        return E_UNKNOWN_ERROR;

    if(check_calibrator() == false)
        return E_UNKNOWN_ERROR;

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x09);
        double value = 0;
        bool ok = multimeter->measuring_IAC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_mA = (value - value_A) * 1000.0f;

        unsigned char m_limit = multimeter->get_limit(V764_2_CMD_0x09, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x09 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_mA) > max_delta_mA)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_mA)
                       .arg(max_delta_mA));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 А %2 кГц)"))
                                      .arg(value_A)
                                      .arg(freq_kHz));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_DC_A(float value_A,
                        float max_delta_A,
                        unsigned char limit,
                        float *result_delta_A,
                        bool is_auto_check)
{
    bool ok = false;
    QString temp;

    emit info(QString(tr("Производится измерение постоянного тока %1A с допустимой погрешностью %2A"))
              .arg(value_A)
              .arg(max_delta_A));

    //???
    double value = 0;
    ok = multimeter->measuring_IDC(&value);
    if(!ok)
    {
        return multimeter->get_last_error();
    }
    //---

    if(type_calibrator == H4_6)
    {
        temp.clear();
        temp.append(H4_6_KEY_I);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_RIGHT);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(H4_6_KEY_1);
        temp.append(H4_6_KEY_9);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_0);
        temp.append(H4_6_KEY_ENTER_VI);
        temp.append(H4_6_KEY_OFF);
        ok = calibrator->step_to_step(temp,
                                      sb_delay->value(),
                                      sb_time_calibration->value(),
                                      false);

        if(!ok)
        {
            calibrator->print_last_error();
            return E_UNKNOWN_ERROR;
        }
    }
    if(type_calibrator == H4_7)
    {
        calibrator->set_timeout(sb_time_calibration->value());
        bool ok = calibrator->set_U_V(value_A);
        if(!ok)
        {
            calibrator->print_last_error();
            return E_UNKNOWN_ERROR;
        }
    }

    if(check_calibrator() == false)
        return E_UNKNOWN_ERROR;

    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x08);
        double value = 0;
        bool ok = multimeter->measuring_IDC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        *result_delta_A = value - value_A;

        unsigned char m_limit = multimeter->get_limit(V764_2_CMD_0x08, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x08 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta_A) > max_delta_A)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta_A)
                       .arg(max_delta_A));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      QString(tr("Проверьте показание мультиметра (должно быть %1 А)")).arg(value_A));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_other_measuring(const QString &prefix,
                                   unsigned int cmd,
                                   double measuring_value,
                                   const QString &measure,
                                   float max_delta,
                                   unsigned char limit,
                                   float *result_delta,
                                   bool is_auto_check)
{
    int button = message_question(this,
                                  tr("Проверка"),
                                  QString(tr("Подключите %1 в %2 %3 к мультиметру и нажмите OK"))
                                  .arg(prefix)
                                  .arg(measuring_value)
                                  .arg(measure));
    if(button != QMessageBox::Yes)
        return E_BREAK_MEASURING;

    if(is_auto_check)
    {
        bool ok;
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        double value = 0;
        switch(cmd)
        {
        case V764_2_CMD_0x0A:
            multimeter->set_deviation(0);
            multimeter->set_retention(0);
            multimeter->set_auto_limit(false);
            multimeter->set_limit(limit);
            multimeter->print_cmd_text(V764_2_CMD_0x0A);
            ok = multimeter->measuring_R(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        case V764_2_CMD_0x0C:
            multimeter->print_cmd_text(V764_2_CMD_0x0C);
            ok = multimeter->measuring_L(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        case V764_2_CMD_0x0B:
            multimeter->print_cmd_text(V764_2_CMD_0x0B);
            ok = multimeter->measuring_C(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        case V764_2_CMD_0x0E:
            multimeter->print_cmd_text(V764_2_CMD_0x0E);
            ok = multimeter->measuring_F(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        case V764_2_CMD_0x0F:
            multimeter->print_cmd_text(V764_2_CMD_0x0F);
            ok = multimeter->measuring_P(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        case V764_2_CMD_0x10:
            multimeter->print_cmd_text(V764_2_CMD_0x10);
            ok = multimeter->measuring_Q(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        case V764_2_CMD_0x11:
            multimeter->print_cmd_text(V764_2_CMD_0x11);
            ok = multimeter->measuring_S(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        case V764_2_CMD_0x12:
            multimeter->print_cmd_text(V764_2_CMD_0x12);
            ok = multimeter->measuring_PF(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            break;

        default:
            break;
        }
        *result_delta = value - measuring_value;

        unsigned char m_limit = multimeter->get_limit(cmd, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("cmd Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        if(qAbs(*result_delta) > max_delta)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(*result_delta)
                       .arg(max_delta));
            return E_INVALID_DELTA;
        }
    }
    else
    {
        int button = message_question(this,
                                      tr("Ручная проверка"),
                                      tr("Проверьте показание мультиметра"));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

    emit info(tr("OK"));
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_10mV(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_question(this,
                                      tr("Кабельное хозяйство"),
                                      tr("Подключите кабель мультиметра к гнезду IN и нажмите Yes"));
        if(button != QMessageBox::Yes)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mV(10.0f,  0.04f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_10mV_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_minus_10mV(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mV(-10.0f, 0.04f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_minus_10mV_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_199mV(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mV(199.0f,  0.34f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_170mV_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_minus_199mV(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mV(-199.0f, 0.34f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_minus_170mV_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_0_5V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(0.5f,  0.3f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_0_5V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_minus_0_5V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(-0.5f, 0.3f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_minus_0_5V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_1V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(1.0f, 0.5f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_1V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_1_5V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(1.5f, 0.7f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_1_5V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_1_99V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(1.99f, 0.9f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_1_7V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_minus_1_99V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(-1.99f, 0.9f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_minus_1_7V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_19_9V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(19.9f, 9.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_17_0V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_199V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(199.0f, 90.0f, L_200V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_170V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_19_9mV_20kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mV(19.90f, 0.96f, 20.0f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_17mV_20kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_190mV_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mV(190.0f, 1.2f, 1.0f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_190mV_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_190mV_50kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mV(190.0f, 4.78f, 50.0f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_190mV_50kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_190mV_100kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mV(190.0f, 4.78f, 100.0f, L_200mV, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_190mV_100kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_0_5V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(0.5f, 2.8f, 1.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_0_5V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_1V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.0f, 4.0f, 1.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_1V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_1V_20Hz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.0f, 4.0f, 0.02f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_1V_20Hz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_1V_20kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.0f, 8.8f, 20.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_1V_20kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_1V_50kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.0f, 28.0f, 50.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_1V_50kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_1V_100kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.0f, 28.0f, 100.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_1V_100kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_1_99V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.9900f, 5.6f, 1.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_1_7V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_10V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(10.0f, 32.0f, 1.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_10V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_10V_20kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(10.0f, 64.0f, 20.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_10V_20kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_10V_50kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(10.0f, 280.0f, 50.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_10V_50kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_10V_100kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(10.0f, 280.0f, 100.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_10V_100kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_100V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(100.0f, 400.0f, 1.0f, L_200V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_100V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg((result_delta / 1000.0f), 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_100V_20kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(100.0f, 1600.0f, 20.0f, L_200V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_100V_20kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg((result_delta / 1000.0f), 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_100V_100kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(100.0f, 2160.0f, 100.0f, L_200V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_100V_100kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg((result_delta / 1000.0f), 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_0_5mkA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mkA(0.5f, 0.8f, L_200mkA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_0_5mkA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкА")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_100mkA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mkA(100.0f, 1.6f, L_200mkA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_100mkA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкА")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_minus_100mkA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mkA(-100.0f, 1.6f, L_200mkA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_minus_100mkA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкА")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_1mA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mA(1.0f, 0.0048f, L_2mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_1mA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_minus_1mA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mA(-1.0f, 0.0048f, L_2mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_minus_1mA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_10mA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mA(10.0f, 0.016f, L_20mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_10mA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_minus_10mA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mA(-10.0f, 0.016f, L_20mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_minus_10mA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_100mA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mA(100.0f, 0.16f, L_200mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_100mA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_1000mA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mA(1000.0f, 1.6f, L_2000mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_1000mA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_minus_1000mA(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_mA(-1000.0f, 1.6f, L_2000mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_minus_1000mA_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_1000V(MyReport *report)
{
    float result_delta = 0.0f;
    int err = E_NO_ERROR;

    if(type_calibrator != H4_7)
    {
        emit error(tr("Данная проверка может быть выполнена ТОЛЬКО на Н4-7"));
        return E_UNKNOWN_ERROR;
    }
    if(cable_in != TEST_CABLE_IN_UDC)
    {
        cable_in = TEST_CABLE_IN_UDC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Требуется усилитель постоянного напряжения"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

    emit info(tr("Производится измерение постоянного напряжения 1000V с допустимой погрешностью 2.8V"));

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    spike_nail_1000V();

    int limit = L_1000V;
    float value_V = 1000.0f;
    float max_delta_V = 2.8f;
    double value = 0;
    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x06);
        bool ok = multimeter->measuring_UDC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        int m_limit = multimeter->get_limit(V764_2_CMD_0x06, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x06 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        float delta_V = qAbs(value_V - value);
        if(delta_V > max_delta_V)
        {
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(delta_V)
                       .arg(max_delta_V));
            return E_INVALID_DELTA;
        }
        result_delta = (value_V - value);
    }
    else
    {
        int button = message_information(this,
                                         tr("Ручная проверка"),
                                         tr("Проверьте показание мультиметра (должно быть 1000 В)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_1000V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UDC_minus_1000V(MyReport *report)
{
    float result_delta = 0.0f;
    int err = E_NO_ERROR;

    if(type_calibrator != H4_7)
    {
        emit error(tr("Данная проверка может быть выполнена ТОЛЬКО на Н4-7"));
        return E_UNKNOWN_ERROR;
    }
    if(cable_in != TEST_CABLE_IN_UDC)
    {
        cable_in = TEST_CABLE_IN_UDC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Требуется усилитель постоянного напряжения"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

    emit info(tr("Производится измерение постоянного напряжения -1000V с допустимой погрешностью 2.8V"));

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    spike_nail_minus_1000V();

    int limit = L_1000V;
    float value_V = -1000.0f;
    float max_delta_V = 2.8f;
    double value = 0;
    if(is_auto_check)
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x06);
        bool ok = multimeter->measuring_UDC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        int m_limit = multimeter->get_limit(V764_2_CMD_0x06, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x06 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        float delta_V = qAbs(value_V - value);
        if(delta_V > max_delta_V)
        {
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(delta_V)
                       .arg(max_delta_V));
            return E_INVALID_DELTA;
        }
        result_delta = (value_V - value);
    }
    else
    {
        int button = message_information(this,
                                         tr("Ручная проверка"),
                                         tr("Проверьте показание мультиметра (должно быть -1000 В)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UDC",
                            CONTROL_UDC_minus_1000V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_19A(MyReport *report)
{
    float result_delta = 0.0f;

    if(type_calibrator == H4_6)
    {
        emit error(tr("Эту проверку нельзя выполнить на Н4-6"));
        return E_UNKNOWN_ERROR;
    }

    if(cable_in != TEST_CABLE_A_IDC)
    {
        cable_in = TEST_CABLE_A_IDC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду A и нажмите OK\n"
                                            "Требуется усилитель постоянного тока\n"
                                            "Усилитель калибратора Н4-7 подключается в обратной полярности (подробнее описано в документации)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_A(19.0f, 0.08f, L_20A, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR)
    {
        cnt_error++;
    }
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_19A_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 А")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IDC_minus_19A(MyReport *report)
{
    float result_delta = 0.0f;

    if(type_calibrator == H4_6)
    {
        emit error(tr("Эту проверку нельзя выполнить на Н4-6"));
        return E_UNKNOWN_ERROR;
    }

    if(cable_in != TEST_CABLE_A_IDC)
    {
        cable_in = TEST_CABLE_A_IDC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду A и нажмите OK\n"
                                            "Требуется усилитель постоянного тока\n"
                                            "Усилитель калибратора Н4-7 подключается в обратной полярности (подробнее описано в документации)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_A(-19.0f, 0.08f, L_20A, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR)
    {
        cnt_error++;
    }
    else
    {
        report->update_data("data_report_IDC",
                            CONTROL_IDC_minus_19A_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 А")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_199V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;
    int err = E_NO_ERROR;

    if(type_calibrator == H4_6)
    {
        emit error(tr("Эту проверку нельзя выполнить на Н4-6"));
        return E_UNKNOWN_ERROR;
    }
    if(cable_in != TEST_CABLE_IN_UAC)
    {
        cable_in = TEST_CABLE_IN_UAC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Внимание! Требуется усилитель напряжения переменного тока"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

    QString temp;
    temp.clear();
    temp.append(H4_7_KEY_V);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_ENTER_VI);
    temp.append(H4_7_KEY_1);
    temp.append(H4_7_KEY_9);
    temp.append(H4_7_KEY_9);
    temp.append(H4_7_KEY_ENTER_VI);
    temp.append(H4_7_KEY_kHz);
    temp.append(H4_7_KEY_ENTER_F);
    temp.append(H4_7_KEY_1);
    temp.append(H4_7_KEY_ENTER_F);
    temp.append(H4_7_KEY_OFF);
    bool ok = calibrator->step_to_step(temp,
                                       sb_delay->value(),
                                       sb_time_calibration->value(),
                                       false);
    if(ok == false)
        return E_UNKNOWN_ERROR;

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    if(is_auto_check)
    {
        unsigned char limit = L_200V;
        float value_V = 199.0f;
        float max_delta_V = 0.72f;
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x07);
        double value = 0;
        bool ok = multimeter->measuring_UAC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        int m_limit = multimeter->get_limit(V764_2_CMD_0x07, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x07 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        float delta_V = qAbs(value_V - value);
        if(delta_V > max_delta_V)
        {
            emit debug(QString(tr("val = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(delta_V)
                       .arg(max_delta_V));
            return E_INVALID_DELTA;
        }
        result_delta = (value_V - value);
        emit info("Ok");
    }
    else
    {
        int button = message_information(this,
                                         tr("Ручная проверка"),
                                         tr("Проверьте показание мультиметра (должно быть 199В 1кГц)"));
        if(button != QMessageBox::Ok)
        {
            calibrator->set_timeout(sb_time_calibration->value());
            calibrator->reset_U();
            return E_BREAK_MEASURING;
        }
    }

    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_170V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_UAC_400V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;
    int err = E_NO_ERROR;

    if(type_calibrator == H4_6)
    {
        emit error(tr("Эту проверку нельзя выполнить на Н4-6"));
        return E_UNKNOWN_ERROR;
    }
    if(cable_in != TEST_CABLE_IN_UAC)
    {
        cable_in = TEST_CABLE_IN_UAC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Требуется усилитель переменного напряжения"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

    QString temp;
    temp.clear();
    temp.append(H4_7_KEY_V);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_RIGHT);
    temp.append(H4_7_KEY_ENTER_VI);
    temp.append(H4_7_KEY_4);
    temp.append(H4_7_KEY_0);
    temp.append(H4_7_KEY_0);
    temp.append(H4_7_KEY_ENTER_VI);
    temp.append(H4_7_KEY_kHz);
    temp.append(H4_7_KEY_ENTER_F);
    temp.append(H4_7_KEY_1);
    temp.append(H4_7_KEY_ENTER_F);
    temp.append(H4_7_KEY_OFF);
    bool ok = calibrator->step_to_step(temp,
                                       sb_delay->value(),
                                       sb_time_calibration->value(),
                                       false);
    if(ok == false)
        return E_UNKNOWN_ERROR;

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    if(is_auto_check)
    {
        unsigned char limit = L_1000V;
        float value_V = 400.0f;
        float max_delta_V = 2.4f;
        double value = 0;
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif
        multimeter->set_deviation(0);
        multimeter->set_retention(0);
        multimeter->set_auto_limit(false);
        multimeter->set_limit(limit);
        multimeter->print_cmd_text(V764_2_CMD_0x07);
        bool ok = multimeter->measuring_UAC(&value);
        if(!ok)
        {
            return multimeter->get_last_error();
        }
        int m_limit = multimeter->get_limit(V764_2_CMD_0x07, &ok);
        if(m_limit != limit)
        {
            emit error(QString(tr("V764_2_CMD_0x07 Мультиметр вернул предел %1, а должно быть %2"))
                       .arg(m_limit)
                       .arg(limit));
            return E_INVALID_LIMIT;
        }

        float delta_V = qAbs(value_V - value);
        if(delta_V > max_delta_V)
        {
            emit debug(QString(tr("value = %1")).arg(value));
            emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                       .arg(delta_V)
                       .arg(max_delta_V));
            return E_INVALID_DELTA;
        }
        result_delta = (value_V - value);
        emit info("Ok");
    }
    else
    {
        int button = message_information(this,
                                         tr("Ручная проверка"),
                                         tr("Проверьте показание мультиметра (должно быть 400В 1кГц)"));
        if(button != QMessageBox::Ok)
        {
            calibrator->set_timeout(sb_time_calibration->value());
            calibrator->reset_U();
            return E_BREAK_MEASURING;
        }
    }

    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_UAC",
                            CONTROL_UAC_400V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IAC_1mA_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mA(1.0f, 0.0048f, 1.0f, L_2mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IAC",
                            CONTROL_IAC_1mA_1_kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IAC_10mA_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mA(10.0f, 0.044f, 1.0f, L_20mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IAC",
                            CONTROL_IAC_10mA_1_kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IAC_100mA_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mA(100.0f, 0.32f, 1.0f, L_200mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IAC",
                            CONTROL_IAC_100mA_1_kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IAC_1000mA_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_mA)
    {
        cable_in = TEST_CABLE_mA;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду mA и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_mA(1000.0f, 3.2f, 1.0f, L_2000mA, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IAC",
                            CONTROL_IAC_1000mA_1_kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мА")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_IAC_10A_1kHz(MyReport *report)
{
    float result_delta = 0.0f;
    float value_A = 10.0f;
    int err = E_NO_ERROR;

    if(cable_in != TEST_CABLE_A)
    {
        calibrator->set_timeout(sb_time_calibration->value());
        calibrator->reset_I(); // сделано по просьбе регуляторов

        cable_in = TEST_CABLE_A;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду A и нажмите OK\n"
                                            "Требуется усилитель переменного тока\n"
                                            "Обратите внимание на индикатор ПЕРЕГРУЗКА"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif

    if(type_calibrator == H4_6)
    {
        err = check_AC_A(10.0f, 240.0f, 1.0f, L_20A, &result_delta, is_auto_check);
    }
    if(type_calibrator == H4_7)
    {
        calibrator->set_timeout(sb_time_calibration->value());
        bool ok = calibrator->set_U_V(value_A, 1.0f);
        if(!ok)
        {
            calibrator->print_last_error();
            return E_UNKNOWN_ERROR;
        }

        if(is_auto_check)
        {
            int limit = L_20A;
            float max_delta_A = 0.24f;
            bool ok = false;
#ifdef CALIBRATION_WIRED
            multimeter->set_serno(ui->sb_SerNo->value());
            multimeter->set_year(ui->sb_Year->value());
            multimeter->set_address(FIXED_ADDRESS);
            multimeter->set_channel(FIXED_CHANNEL);
            multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
            multimeter->set_serno(ui->sb_SerNo->value());
            multimeter->set_year(ui->sb_Year->value());
            multimeter->set_address(ui->sb_address->value());
            multimeter->set_channel(ui->sb_channel->value());
            multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

            multimeter->set_deviation(0);
            multimeter->set_retention(0);
            multimeter->set_auto_limit(false);
            multimeter->set_limit(limit);
            multimeter->print_cmd_text(V764_2_CMD_0x09);
            double value = 0;
            ok = multimeter->measuring_IAC(&value);
            if(!ok)
            {
                return multimeter->get_last_error();
            }
            int m_limit = multimeter->get_limit(V764_2_CMD_0x09, &ok);
            if(m_limit != limit)
            {
                emit error(QString(tr("V764_2_CMD_0x09 Мультиметр вернул предел %1, а должно быть %2"))
                           .arg(m_limit)
                           .arg(limit));
                return E_INVALID_LIMIT;
            }

            float delta_A = qAbs(value_A - value);
            if(delta_A > max_delta_A)
            {
                emit debug(QString(tr("value = %1")).arg(value));
                emit error(QString(tr("Не в допуске! delta = %2 max_delta = %3"))
                           .arg(delta_A)
                           .arg(max_delta_A));
                return E_INVALID_DELTA;
            }
            result_delta = (value_A - value);
        }
        else
        {
            int button = message_information(this,
                                             tr("Ручная проверка"),
                                             tr("Проверьте показание мультиметра (должно быть 10А 1 кГц)"));
            if(button != QMessageBox::Ok)
                return E_BREAK_MEASURING;
        }
    }

    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_IAC",
                            CONTROL_IAC_10A_1_kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 А")).arg(result_delta / 1000.0f, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_0oM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    0.0f,
                                    "Ом",
                                    0.08f,
                                    L_200o,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_0_Om_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 Ом")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_10oM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    10.0f,
                                    "Ом",
                                    0.11f,
                                    L_200o,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_10_Om_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 Ом")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_100oM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    100.0f,
                                    "Ом",
                                    0.36f,
                                    L_200o,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_100_Om_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 Ом")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_190oM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    190.0f,
                                    "Ом",
                                    0.61f,
                                    L_200o,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_190_Om_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 Ом")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_1koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    1.0f,
                                    "кОм",
                                    0.0017f,
                                    L_2kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_1_kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_1_9koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    1.9f,
                                    "кОм",
                                    0.0031f,
                                    L_2kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_1_9_kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_10koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    10.0f,
                                    "кОм",
                                    0.012f,
                                    L_20kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_10_kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_100koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    100.0f,
                                    "кОм",
                                    0.120f,
                                    L_200kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_100_kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_R_1MoM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    1.0f,
                                    "МОм",
                                    0.0032f,
                                    L_2Mom,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_R",
                            CONTROL_R_1_MOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 МОм")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_0pF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    0.0f,
                                    "пФ",
                                    2.4f,
                                    L_C_0pF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_0_pF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 пФ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_100pF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    100.0f,
                                    "пФ",
                                    4.0f,
                                    L_C_100pF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_100_pF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 пФ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_1000pF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    1000.0f,
                                    "пФ",
                                    40.0f,
                                    L_C_1000pF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_1000_pF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 пФ")).arg(result_delta, 0, 'f', 1));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_10000pF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    10000.0f,
                                    "пФ",
                                    400.0f,
                                    L_C_10mkF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_10000_pF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 пФ")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_0_1mkF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    0.1f,
                                    "мкФ",
                                    0.004f,
                                    L_C_0_1mkF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_0_1_mkF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкФ")).arg(result_delta, 0, 'f', 5));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_1mkF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    1.0f,
                                    "мкФ",
                                    0.04f,
                                    L_C_1mkF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_1_mkF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкФ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_10mkF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    10.0f,
                                    "мкФ",
                                    0.4f,
                                    L_C_10mkF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_10_mkF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкФ")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_C_1000mkF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    1000.0f,
                                    "мкФ",
                                    0.4f,
                                    L_C_1000mkF,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_C",
                            CONTROL_C_1000_mkF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкФ")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_L_0mH(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру индуктивности"),
                                    V764_2_CMD_0x0C,
                                    0.0f,
                                    "мГн",
                                    0.016f,
                                    L_L_0mH,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_L",
                            CONTROL_L_0_mH_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГн")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_L_1mH(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру индуктивности"),
                                    V764_2_CMD_0x0C,
                                    1.0f,
                                    "мГн",
                                    0.04f,
                                    L_L_1mH,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_L",
                            CONTROL_L_1_mH_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГн")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_L_10mH(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру индуктивности"),
                                    V764_2_CMD_0x0C,
                                    10.0f,
                                    "мГн",
                                    0.4f,
                                    L_L_10mH,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_L",
                            CONTROL_L_10_mH_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГн")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_L_100mH(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру индуктивности"),
                                    V764_2_CMD_0x0C,
                                    100.0f,
                                    "мГн",
                                    4.0,
                                    L_L_100mH,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_L",
                            CONTROL_L_100_mH_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГн")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_UDC_1V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(1.0f, 0.5f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_UDC_1V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_UDC_10V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(10.0f, 6.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_UDC_10V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_UAC_1V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.0f, 4.0f, 1.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_UAC_1V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_UAC_10V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(10.0f, 32.0f, 1.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_UAC_10V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_R_1koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    1.0f,
                                    "кОм",
                                    0.0017f,
                                    L_2kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_R_1kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_R_100koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    100.0f,
                                    "кОм",
                                    0.120f,
                                    L_200kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_R_100kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_IDC_10A(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_A_IDC)
    {
        cable_in = TEST_CABLE_A_IDC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду A и нажмите OK\n"
                                            "Требуется подключение усилителя постоянного тока\n"
                                            "Усилитель калибратора Н4-7 подключается в обратной полярности (подробнее описано в документации)\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_A(10.0f, 0.06f, L_20A, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_IDC_10A_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 А")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_FREQ_1MHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("частоту"),
                                    V764_2_CMD_0x0E,
                                    1.0f,
                                    "мГц",
                                    0.04f,
                                    -1,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_FREQ_1MHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГц")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_C_1mkF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    1.0f,
                                    "мкФ",
                                    0.04f,
                                    -1,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_C_1mkF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкФ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_hi_L_10mH(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть повышено (4V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру индуктивности"),
                                    V764_2_CMD_0x0C,
                                    10.0f,
                                    "мГн",
                                    0.4f,
                                    -1,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_hi_power",
                            CONTROL_HI_L_10mH_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГн")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_UDC_1V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(1.0f, 0.5f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_UDC_1V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_UDC_10V(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_V(10.0f, 6.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_UDC_10V_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_UAC_1V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(1.0f, 4.0f, 1.0f, L_2V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_UAC_1V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_UAC_10V_1kHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_AC_V(10.0f, 32.0f, 1.0f, L_20V, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_UAC_10V_1kHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 В")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_R_1koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    1.0f,
                                    "кОм",
                                    0.0017f,
                                    L_2kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_R_1kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_R_100koM(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру сопротивления"),
                                    V764_2_CMD_0x0A,
                                    100.0f,
                                    "кОм",
                                    0.120f,
                                    L_200kOm,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_R_100kOm_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 кОм")).arg(result_delta, 0, 'f', 2));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_IDC_10A(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_A_IDC)
    {
        cable_in = TEST_CABLE_A_IDC;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Требуется усилитель постоянно тока\n"
                                            "Усилитель калибратора Н4-7 подключается в обратной полярности (подробнее описано в документации)\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_DC_A(10.0f, 0.06f, L_20A, &result_delta, is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_IDC_10A_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 А")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_FREQ_1MHz(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("частоту"),
                                    V764_2_CMD_0x0E,
                                    1.0f,
                                    "мГц",
                                    0.04f,
                                    -1,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_HI_FREQ_1MHz_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГц")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_C_1mkF(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру емкости"),
                                    V764_2_CMD_0x0B,
                                    1.0f,
                                    "мкФ",
                                    0.04f,
                                    -1,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_C_1mkF_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мкФ")).arg(result_delta, 0, 'f', 4));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::control_lo_L_10mH(MyReport *report)
{
    float result_delta = 0.0f;

    if(cable_in != TEST_CABLE_IN)
    {
        cable_in = TEST_CABLE_IN;
        int button = message_information(this,
                                         tr("Кабельное хозяйство"),
                                         tr("Подключите кабель мультиметра к гнезду IN и нажмите OK\n"
                                            "Питание мультиметра должно быть понижено (2V)"));
        if(button != QMessageBox::Ok)
            return E_BREAK_MEASURING;
    }

#ifdef CALIBRATION_WIRED
    bool is_auto_check = true;
#else
    bool is_auto_check = (ui->cb_TypeMultimeter->currentIndex() == 2);
#endif
    int err = check_other_measuring(tr("меру индуктивности"),
                                    V764_2_CMD_0x0C,
                                    10.0f,
                                    "мГн",
                                    0.4f,
                                    -1,
                                    &result_delta,
                                    is_auto_check);
    cnt_test++;
    if(err != E_NO_ERROR) cnt_error++;
    else
    {
        report->update_data("data_report_lo_power",
                            CONTROL_LO_L_10mH_TEXT,
                            ui->cb_Step_Report->currentIndex(),
                            QString(tr("%1 мГн")).arg(result_delta, 0, 'f', 3));
    }
    return err;
}
//--------------------------------------------------------------------------------
int MainBox::check_treeWidget_controls_U(MyReport *report)
{
    int err = E_NO_ERROR;

    QTreeWidgetItemIterator it_U(ui->treeWidget_controls_U);
    while (*it_U)
    {
        if(ui->btn_CheckMultimeter->isChecked() == false)
            break;
        if((*it_U)->type() == QTreeWidgetItem::UserType)
        {
            if((*it_U)->checkState(0) == Qt::Checked)
            {
                //UDC
                if((*it_U)->text(0) == CONTROL_UDC_10mV_TEXT)
                {
                    err = control_UDC_10mV(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_minus_10mV_TEXT)
                {
                    err = control_UDC_minus_10mV(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_170mV_TEXT)
                {
                    err = control_UDC_199mV(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_minus_170mV_TEXT)
                {
                    err = control_UDC_minus_199mV(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_0_5V_TEXT)
                {
                    err = control_UDC_0_5V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_minus_0_5V_TEXT)
                {
                    err = control_UDC_minus_0_5V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_1V_TEXT)
                {
                    err = control_UDC_1V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_1_5V_TEXT)
                {
                    err = control_UDC_1_5V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_1_7V_TEXT)
                {
                    err = control_UDC_1_99V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_minus_1_7V_TEXT)
                {
                    err = control_UDC_minus_1_99V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_17_0V_TEXT)
                {
                    err = control_UDC_19_9V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_170V_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UDC_199V(report);
                    calibrator->reset_U();
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }

                //UAC
                if((*it_U)->text(0) == CONTROL_UAC_17mV_20kHz_TEXT)
                {
                    err = control_UAC_19_9mV_20kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_190mV_1kHz_TEXT)
                {
                    err = control_UAC_190mV_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_190mV_50kHz_TEXT)
                {
                    err = control_UAC_190mV_50kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_190mV_100kHz_TEXT)
                {
                    err = control_UAC_190mV_100kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_0_5V_1kHz_TEXT)
                {
                    err = control_UAC_0_5V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_1V_20Hz_TEXT)
                {
                    err = control_UAC_1V_20Hz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_1V_1kHz_TEXT)
                {
                    err = control_UAC_1V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_1V_20kHz_TEXT)
                {
                    err = control_UAC_1V_20kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_1V_50kHz_TEXT)
                {
                    err = control_UAC_1V_50kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_1V_100kHz_TEXT)
                {
                    err = control_UAC_1V_100kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_1_7V_1kHz_TEXT)
                {
                    err = control_UAC_1_99V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_10V_1kHz_TEXT)
                {
                    err = control_UAC_10V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_10V_20kHz_TEXT)
                {
                    err = control_UAC_10V_20kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_10V_50kHz_TEXT)
                {
                    err = control_UAC_10V_50kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_10V_100kHz_TEXT)
                {
                    err = control_UAC_10V_100kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_100V_1kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UAC_100V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_100V_20kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UAC_100V_20kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_100V_100kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UAC_100V_100kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }

                //Требуется усилитель напряжения
                if((*it_U)->text(0) == CONTROL_UDC_1000V_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UDC_1000V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UDC_minus_1000V_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UDC_minus_1000V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }

                if((*it_U)->text(0) == CONTROL_UAC_170V_1kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UAC_199V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_U)->text(0) == CONTROL_UAC_400V_1kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_UAC_400V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_U)->setCheckState(0, Qt::Unchecked);
                }
            }
        }
        *it_U++;
    }

    return err;
}
//--------------------------------------------------------------------------------
int MainBox::check_treeWidget_controls_I(MyReport *report)
{
    int err = E_NO_ERROR;

    QTreeWidgetItemIterator it_I(ui->treeWidget_controls_I);
    while (*it_I)
    {
        if(ui->btn_CheckMultimeter->isChecked() == false)
            break;
        if((*it_I)->type() == QTreeWidgetItem::UserType)
        {
            if((*it_I)->checkState(0) == Qt::Checked)
            {
                //mA
                if((*it_I)->text(0) == CONTROL_IDC_0_5mkA_TEXT)
                {
                    err = control_IDC_0_5mkA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_100mkA_TEXT)
                {
                    err = control_IDC_100mkA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_minus_100mkA_TEXT)
                {
                    err = control_IDC_minus_100mkA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_1mA_TEXT)
                {
                    err = control_IDC_1mA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_minus_1mA_TEXT)
                {
                    err = control_IDC_minus_1mA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_10mA_TEXT)
                {
                    err = control_IDC_10mA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_minus_10mA_TEXT)
                {
                    err = control_IDC_minus_10mA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_100mA_TEXT)
                {
                    err = control_IDC_100mA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_1000mA_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IDC_1000mA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_minus_1000mA_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IDC_minus_1000mA(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }

                //A
                if((*it_I)->text(0) == CONTROL_IAC_1mA_1_kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IAC_1mA_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IAC_10mA_1_kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IAC_10mA_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IAC_100mA_1_kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IAC_100mA_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IAC_1000mA_1_kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IAC_1000mA_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }

                //Требуется усилитель тока
                if((*it_I)->text(0) == CONTROL_IDC_19A_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IDC_19A(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IDC_minus_19A_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IDC_minus_19A(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_I)->text(0) == CONTROL_IAC_10A_1_kHz_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_IAC_10A_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_I)->setCheckState(0, Qt::Unchecked);
                }
            }
        }
        *it_I++;
    }

    return err;
}
//--------------------------------------------------------------------------------
int MainBox::check_treeWidget_controls_Other(MyReport *report)
{
    int err = E_NO_ERROR;

    QTreeWidgetItemIterator it_O(ui->treeWidget_controls_Other);
    while (*it_O)
    {
        if(ui->btn_CheckMultimeter->isChecked() == false)
            break;
        if((*it_O)->type() == QTreeWidgetItem::UserType)
        {
            if((*it_O)->checkState(0) == Qt::Checked)
            {
                //R
                if((*it_O)->text(0) == CONTROL_R_0_Om_TEXT)
                {
                    err = control_R_0oM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_10_Om_TEXT)
                {
                    err = control_R_10oM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_100_Om_TEXT)
                {
                    err = control_R_100oM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_190_Om_TEXT)
                {
                    err = control_R_190oM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_1_kOm_TEXT)
                {
                    err = control_R_1koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_1_9_kOm_TEXT)
                {
                    err = control_R_1_9koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_10_kOm_TEXT)
                {
                    err = control_R_10koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_100_kOm_TEXT)
                {
                    err = control_R_100koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_R_1_MOm_TEXT)
                {
                    err = control_R_1MoM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }

                if((*it_O)->text(0) == CONTROL_C_0_pF_TEXT)
                {
                    err = control_C_0pF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_C_100_pF_TEXT)
                {
                    err = control_C_100pF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_C_1000_pF_TEXT)
                {
                    err = control_C_1000pF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_C_10000_pF_TEXT)
                {
                    err = control_C_10000pF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_C_0_1_mkF_TEXT)
                {
                    err = control_C_0_1mkF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_C_1_mkF_TEXT)
                {
                    err = control_C_1mkF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_C_10_mkF_TEXT)
                {
                    err = control_C_10mkF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_C_1000_mkF_TEXT)
                {
                    err = control_C_1000mkF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }

                if((*it_O)->text(0) == CONTROL_L_0_mH_TEXT)
                {
                    err = control_L_0mH(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_L_1_mH_TEXT)
                {
                    err = control_L_1mH(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_L_10_mH_TEXT)
                {
                    err = control_L_10mH(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_O)->text(0) == CONTROL_L_100_mH_TEXT)
                {
                    err = control_L_100mH(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR) (*it_O)->setCheckState(0, Qt::Unchecked);
                }
            }
        }
        *it_O++;
    }

    return err;
}
//--------------------------------------------------------------------------------
int MainBox::check_treeWidget_controls_Hi_Low_U(MyReport *report)
{
    int err = E_NO_ERROR;

    QTreeWidgetItemIterator it_HLU(ui->treeWidget_controls_Hi_Low_U);
    while (*it_HLU)
    {
        if(ui->btn_CheckMultimeter->isChecked() == false)
            break;
        if((*it_HLU)->type() == QTreeWidgetItem::UserType)
        {
            if((*it_HLU)->checkState(0) == Qt::Checked)
            {
                //повышенное напряжение
                if((*it_HLU)->text(0) == CONTROL_HI_UDC_1V_TEXT)
                {
                    err = control_hi_UDC_1V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_HI_UDC_10V_TEXT)
                {
                    err = control_hi_UDC_10V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_HI_UAC_1V_1kHz_TEXT)
                {
                    err = control_hi_UAC_1V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_HI_UAC_10V_1kHz_TEXT)
                {
                    err = control_hi_UAC_10V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }

                if((*it_HLU)->text(0) == CONTROL_HI_R_1kOm_TEXT)
                {
                    err = control_hi_R_1koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_HI_R_100kOm_TEXT)
                {
                    err = control_hi_R_100koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }

                if((*it_HLU)->text(0) == CONTROL_HI_IDC_10A_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_hi_IDC_10A(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_HI_FREQ_1MHz_TEXT)
                {
                    err = control_hi_FREQ_1MHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_HI_C_1mkF_TEXT)
                {
                    err = control_hi_C_1mkF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_HI_L_10mH_TEXT)
                {
                    err = control_hi_L_10mH(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }

                //пониженное напряжение
                if((*it_HLU)->text(0) == CONTROL_LO_UDC_1V_TEXT)
                {
                    err = control_lo_UDC_1V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_LO_UDC_10V_TEXT)
                {
                    err = control_lo_UDC_10V(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_LO_UAC_1V_1kHz_TEXT)
                {
                    err = control_lo_UAC_1V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_LO_UAC_10V_1kHz_TEXT)
                {
                    err = control_lo_UAC_10V_1kHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }

                if((*it_HLU)->text(0) == CONTROL_LO_R_1kOm_TEXT)
                {
                    err = control_lo_R_1koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_LO_R_100kOm_TEXT)
                {
                    err = control_lo_R_100koM(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }

                if((*it_HLU)->text(0) == CONTROL_LO_IDC_10A_TEXT)
                {
                    calibrator->set_timeout(sb_time_calibration->value());
                    calibrator->reset_U();
                    err = control_lo_IDC_10A(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    calibrator->reset_U();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_LO_FREQ_1MHz_TEXT)
                {
                    err = control_lo_FREQ_1MHz(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_LO_C_1mkF_TEXT)
                {
                    err = control_lo_C_1mkF(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
                if((*it_HLU)->text(0) == CONTROL_LO_L_10mH_TEXT)
                {
                    err = control_lo_L_10mH(report);
                    if(err != E_NO_ERROR) multimeter->print_last_error();
                    if(err == E_BREAK_MEASURING) break;
                    if(err == E_NO_ERROR)
                        (*it_HLU)->setCheckState(0, Qt::Unchecked);
                }
            }
        }
        *it_HLU++;
    }

    return err;
}
//--------------------------------------------------------------------------------
void MainBox::check_multemeter(bool state)
{
    if(state == false)
        return;

#if 0
    int button = message_information(this,
                                     tr("Проверка"),
                                     tr("Подключите кабель мультиметра к гнезду IN и нажмите OK"),
                                     QMessageBox::Cancel | QMessageBox::Ok);
    if(button != QMessageBox::Ok)
        return;
#endif

    QString database_name = QString(REPORT_NAME)
            .arg(ui->sb_SerNo->value())
            .arg(ui->sb_Year->value());

    emit debug(database_name);
    emit debug(QString("index %1").arg(ui->cb_Step_Report->currentIndex()));

    int err = E_NO_ERROR;

#ifdef CALIBRATION_WIRED
    if(true)
#else
    if(ui->cb_TypeMultimeter->currentIndex() == 2)
#endif
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->print_cmd_text(V764_2_CMD_0x02);
        bool ok = multimeter->remote_control_on();
        if(!ok)
        {
            multimeter->print_last_error();
            ui->btn_CheckMultimeter->setChecked(false);
            return;
        }
    }

    cnt_error = 0;
    cnt_test = 0;

    MyReport *report = new MyReport(this);
    bool ok = report->open_database("QSQLITE", database_name);
    if(ok == false)
    {
        ui->btn_CheckMultimeter->setChecked(false);
        return;
    }
    if((report->get_count_tables() != 11) && (report->get_count_tables() != 12))
    {
        report->clean_database_report();
    }

    cable_in = TEST_CABLE_OFF;

#if 1
    controls_enabled(false);
    ui->btn_CheckMultimeter->setEnabled(true);
#else
    ui->treeWidget_controls_U->setEnabled(false);
    ui->treeWidget_controls_I->setEnabled(false);
    ui->treeWidget_controls_Other->setEnabled(false);
    ui->treeWidget_controls_Hi_Low_U->setEnabled(false);
#endif

#if 0
    check_treeWidget_controls_U(report);
    check_treeWidget_controls_I(report);
    check_treeWidget_controls_Other(report);
    check_treeWidget_controls_Hi_Low_U(report);
#else
    if(err == E_NO_ERROR)
    {
        err = check_treeWidget_controls_U(report);
    }
    if(err == E_NO_ERROR)
    {
        err = check_treeWidget_controls_I(report);
    }
    if(err == E_NO_ERROR)
    {
        err = check_treeWidget_controls_Other(report);
    }
    if(err == E_NO_ERROR)
    {
        err = check_treeWidget_controls_Hi_Low_U(report);
    }
#endif

#if 1
    controls_enabled(true);
#ifndef CALIBRATION_WIRED
    type_selected(ui->cb_TypeMultimeter->currentIndex());
#endif
#else
    ui->treeWidget_controls_U->setEnabled(true);
    ui->treeWidget_controls_I->setEnabled(true);
    ui->treeWidget_controls_Other->setEnabled(true);
    ui->treeWidget_controls_Hi_Low_U->setEnabled(true);
#endif

    emit info(QString(tr("Прошло тестов: %1. Ошибок: %2"))
              .arg(cnt_test)
              .arg(cnt_error));

    calibrator->set_timeout(sb_time_calibration->value());
    calibrator->reset_U();

#ifdef CALIBRATION_WIRED
    if(true)
#else
    if(ui->cb_TypeMultimeter->currentIndex() == 2)
#endif
    {
#ifdef CALIBRATION_WIRED
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(FIXED_ADDRESS);
        multimeter->set_channel(FIXED_CHANNEL);
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#else
        multimeter->set_serno(ui->sb_SerNo->value());
        multimeter->set_year(ui->sb_Year->value());
        multimeter->set_address(ui->sb_address->value());
        multimeter->set_channel(ui->sb_channel->value());
        multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
#endif

        multimeter->print_cmd_text(V764_2_CMD_0x03);
        ok = multimeter->remote_control_off();
        if(!ok)
        {
            multimeter->print_last_error();
        }
    }

    report->close_database(database_name);
    report->deleteLater();

    msgInfo(tr("Проверки закончены!"));
    ui->btn_CheckMultimeter->setChecked(false);
}
//--------------------------------------------------------------------------------
void MainBox::report(void)
{
    int serno = ui->sb_SerNo->value();
    int year  = ui->sb_Year->value();

    MyReport *report = new MyReport(this);
    QString database_name = QString(REPORT_NAME)
            .arg(serno)
            .arg(year);
    bool ok = report->open_database("QSQLITE", database_name);
    if(!ok) return;

    report->view(serno, year);
    report->close_database(database_name);
    report->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::clear_report_database(void)
{
    int button = message_question(this,
                                  tr("Очистка отчета"),
                                  tr("Вы уверены, что хотите очистить отчет?"));
    if(button != QMessageBox::Yes)
        return;

    QString database_name = QString(REPORT_NAME)
            .arg(ui->sb_SerNo->value())
            .arg(ui->sb_Year->value());

    MyReport *report = new MyReport(this);
    report->open_database("QSQLITE", database_name);
    report->clean_database_report();
    report->deleteLater();

    msgInfo(tr("Очистка отчета завершена!"));
}
//--------------------------------------------------------------------------------
void MainBox::msgInfo(const QString &message)
{
#ifndef FAKE
    QSound::play(":/music/info.wav");
#endif
    QMessageBox::information(this, tr("Информация"), message);
}
//--------------------------------------------------------------------------------
int MainBox::message_question(QWidget *parent,
                              const QString &title,
                              const QString &text)
{
#ifndef FAKE
    QSound::play(":/music/question.wav");
#endif
    return QMessageBox::question(parent,
                                 title,
                                 text,
                                 QMessageBox::No | QMessageBox::Yes);
}
//--------------------------------------------------------------------------------
int MainBox::message_information(QWidget *parent,
                                 const QString &title,
                                 const QString &text)
{
#ifndef FAKE
    QSound::play(":/music/info.wav");
#endif
    return QMessageBox::information(parent,
                                    title,
                                    text,
                                    QMessageBox::Ok);
}
//--------------------------------------------------------------------------------
int MainBox::message_critical(QWidget *parent,
                              const QString &title,
                              const QString &text)
{
#ifndef FAKE
    QSound::play(":/music/error.wav");
#endif
    return QMessageBox::critical(parent,
                                 title,
                                 text,
                                 QMessageBox::Ok);
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void MainBox::createFakeBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *fakeBar = new QToolBar(tr("fakebar"));
    fakeBar->setObjectName("fakeBar");

    mw->addToolBar(Qt::TopToolBarArea, fakeBar);

    QToolButton *btn_fake = add_button(fakeBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
