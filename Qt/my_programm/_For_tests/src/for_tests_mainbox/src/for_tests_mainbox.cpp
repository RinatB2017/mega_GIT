/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "ui_for_tests_mainbox.h"
//--------------------------------------------------------------------------------
#include <algorithm>    // std::find_if
#include <vector>       // std::vector
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "for_tests_mainbox.hpp"
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
    save_widgets();

    if(timer)
    {
        timer->stop();
        timer->disconnect();
        timer->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    ui->horizontalSlider->setRange(0, 1000);

    connect(ui->sb_1,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &MainBox::check_in);
    connect(ui->sb_2,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &MainBox::check_in);
    connect(ui->sb_res, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &MainBox::check_in);
    connect(ui->btn_ok, &QPushButton::clicked,  this,   &MainBox::victory);

    check_in();

    ui->toolButton->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon)));
    connect(ui->toolButton, &QToolButton::clicked,  this,   &MainBox::delete_string);

    //---
    connect(ui->btn_click,  &QPushButton::clicked,  [this]() {
        emit info("click");
    });
    connect(ui->btn_set_dt,  &QPushButton::clicked,  [this]() {
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    });
    connect(ui->btn_clear_strings,  &QPushButton::clicked,  [this]() {
        ui->comboBox_2->clear();
    });
    connect(ui->btn_add_strings,  &QPushButton::clicked,  [this]() {
        ui->comboBox_2->addItem("a0");
        ui->comboBox_2->addItem("a1");
        ui->comboBox_2->addItem("a2");

        ui->listWidget->addItem("b0");
        ui->listWidget->addItem("b1");
        ui->listWidget->addItem("b2");
    });

    //---
    ui->hex_widget->setMaximum(std::numeric_limits<qlonglong>::max());
    ui->hex_widget->setMinimum(std::numeric_limits<qlonglong>::min());
    //---

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainBox::show_timer_count);

    //---
#if 0
    QElapsedTimer timer;
    for(int n=0; n<10; n++)
    {
        splash->showMessage(QString("test %1").arg(n));
        timer.start();
        while(timer.elapsed() < 1000)
        {
            QCoreApplication::processEvents();
        }
    }
#endif
    //---

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif

    //---
    QFileSystemModel *file_model = new QFileSystemModel(this);
    file_model->setNameFilters(QStringList() << "*");
    file_model->setNameFilterDisables(false);

    ui->tree->setModel(file_model);
    //---
    QStandardItemModel *text_model = new QStandardItemModel(0, 2, this);
    text_model->setHeaderData(0, Qt::Horizontal, tr("Test"));
    text_model->setHorizontalHeaderLabels(QStringList() << "Text" << "N");

    for(int n=0; n<10; n++)
    {
        text_model->insertRow(n);
        text_model->setData(text_model->index(n, 0, QModelIndex()), "text");
        text_model->setData(text_model->index(n, 1, QModelIndex()), n);
    }

    ui->tableView->setModel(text_model);
    //---

    load_widgets();
}
//--------------------------------------------------------------------------------
bool MainBox::set_theme_windows(void)
{
    QFile file(":/themes_css/Theme (Windows).css");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error("theme file not open");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_norton_commander(void)
{
    QFile file(":/themes_qss/Norton Commander.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error("theme file not open");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_styles(void)
{
    QFile file(":/themes_qss/styles.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error("theme file not open");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::delete_string(void)
{
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}
//--------------------------------------------------------------------------------
void MainBox::check_in(void)
{
    int a = ui->sb_1->value();
    int b = ui->sb_2->value();
    int c = ui->sb_res->value();

    bool res = ((a + b) == c);
    ui->btn_ok->setEnabled(res);
}
//--------------------------------------------------------------------------------
void MainBox::victory(void)
{
    messagebox_info("Info", "Victory!");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",             &MainBox::test });
    commands.append({ id++, "inc push button",  &MainBox::inc_push_button });
    commands.append({ id++, "dec push button",  &MainBox::dec_push_button });
    commands.append({ id++, "timer start",      &MainBox::timer_start });
    commands.append({ id++, "timer stop",       &MainBox::timer_stop });

    commands.append({ id++, "Theme (Windows).css",  &MainBox::set_theme_windows });
    commands.append({ id++, "Norton Commander.qss", &MainBox::set_norton_commander });
    commands.append({ id++, "styles.qss",           &MainBox::set_styles });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block");
    cb_block->setObjectName("cb_block");
    testbar->addWidget(cb_block);

    sb_test = new QSpinBox(this);
    sb_test->setObjectName("sb_test");
    testbar->addWidget(sb_test);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test,    &QToolButton::clicked,  this,   &MainBox::choice_test);

    connect(cb_block,   &QCheckBox::clicked,    cb_test,            &QComboBox::setDisabled);
    connect(cb_block,   &QCheckBox::clicked,    btn_choice_test,    &QComboBox::setDisabled);
    connect(cb_block,   &QCheckBox::clicked,    sb_test,            &QComboBox::setDisabled);

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    //    QList<MainBox::CMD>::iterator cmd_it = std::find_if(
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
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
#if 0
//TODO старый вариант, от 01.08.2020
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
#endif
//--------------------------------------------------------------------------------
void MainBox::inFunc(QPushButton *btn, saveSlot slot)
{
    connect(btn,    &QPushButton::clicked,  this,   slot);
}
//--------------------------------------------------------------------------------
void MainBox::s_inFunc(void)
{
    emit trace(Q_FUNC_INFO);
    QMessageBox::information(nullptr, "", "info");
}
//--------------------------------------------------------------------------------
int MainBox::get_cnt(void)
{
    emit trace(Q_FUNC_INFO);
    return QRandomGenerator::global()->generate() % 10;
}
//--------------------------------------------------------------------------------
void MainBox::test_validator(void)
{
    /* Create a string for a regular expression */
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    /*
     * Create a regular expression with a string
     * as a repeating element
     */
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    /*
     * Create a validation regular expression
     * using a regular expression
     */
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    /* Set Validator on QLineEdit */
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setValidator(ipValidator);
    lineEdit->show();
}
//--------------------------------------------------------------------------------
void MainBox::print_mp(QWidget *widget)
{
    emit error(QString("objectName %1").arg(widget->objectName()));

    const QMetaObject* metaObject = widget->metaObject();
    emit info(QString("propertyCount  = %1").arg(metaObject->propertyCount()));
    emit info(QString("propertyOffset = %1").arg(metaObject->propertyOffset()));
    emit error("---");
    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
    {
        emit error(QString("%1").arg(QString::fromLatin1(metaObject->property(i).name())));
    }
    emit error("---");
    emit info(QString("methodCount    = %1").arg(metaObject->methodCount()));
    emit info(QString("methodOffset   = %1").arg(metaObject->methodOffset()));
    emit error("---");
    for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
    {
        emit error(QString("%1").arg(QString::fromLatin1(metaObject->method(i).methodSignature())));
    }
    emit error("---");
}
//--------------------------------------------------------------------------------
bool MainBox::timer_start(void)
{
    Q_CHECK_PTR(timer);
    timer->start(500);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::timer_stop(void)
{
    Q_CHECK_PTR(timer);
    timer->stop();
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::show_timer_count(void)
{
    emit info(QString("cnt %1").arg(cnt++));
}
//--------------------------------------------------------------------------------
bool MainBox::inc_push_button(void)
{
    ui->push_widget->setFixedSize(ui->push_widget->width(),
                                  ui->push_widget->height()+10);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::dec_push_button(void)
{
    ui->push_widget->setFixedSize(ui->push_widget->width(),
                                  ui->push_widget->height()-10);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 0
    uint16_t a = 0x0106;
    uint16_t b = a >> 8;
    uint16_t c = a & 0xFF;
    emit info(QString("%1 %2")
              .arg(b, 2, 10, QChar('0'))
              .arg(c, 2, 10, QChar('0')));
#endif

#if 0
    typedef struct XXX
    {
        int id;
        QString text;
        QString param;
    } XXX_t;
    QList<XXX> xxx;

    xxx.append({ 1, "test1", "01020304" });
    xxx.append({ 1, "test2", "" });
    xxx.append({ 1, "test3", "01020304" });

    foreach(XXX val,xxx )
    {
        bool ok;
        int value = val.param.toInt(&ok, 16);
        if(ok)
        {
            emit info(QString("%1 %2")
                      .arg(val.text)
                      .arg(value));
        }
        else
        {
            emit info(QString("%1 (bad)%2")
                      .arg(val.text)
                      .arg(0));
        }
    }

    emit info("OK");
#endif

#if 0
    emit info(QString("ulong %1").arg(sizeof(ulong)));
    emit info(QString("qlonglong %1").arg(sizeof(qlonglong)));
    emit info(QString("uint32_t %1").arg(sizeof(uint32_t)));
    emit info(QString("uint64_t %1").arg(sizeof(uint64_t)));
#endif

#if 0
    ui->hex_widget->setValue(0xFEFEFEFE);
#endif

#if 0
    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");
#endif

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    bool is_exit = ui->cb_fag_exit->isChecked();
    if(is_exit == false)
    {
        messagebox_info("Info", "Низзя");
    }
    return is_exit;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
    qDebug() << "MainBox::load_setting";

    if(sb_test)
    {
        int value = 0;
        load_int("sb_test", &value);
        sb_test->setValue(value);
    }
    if(cb_block)
    {
        int value = 0;
        load_int("cb_block", &value);
        bool block_is_checked = value;
        cb_block->clicked(block_is_checked);
        cb_block->setChecked(block_is_checked);
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    qDebug() << "MainBox::save_setting";

    if(sb_test)
    {
        save_int("sb_test", sb_test->value());
    }
    if(cb_block)
    {
        save_int("cb_block", cb_block->isChecked());
    }
}
//--------------------------------------------------------------------------------
