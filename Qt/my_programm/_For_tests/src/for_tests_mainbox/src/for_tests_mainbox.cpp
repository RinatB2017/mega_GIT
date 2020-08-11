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

    ui->te_test->setProperty(NO_SAVE, true);

    //---
    connect(ui->btn_click,  &QPushButton::clicked,  [this]() {
        emit info("click");
    });
    connect(ui->btn_set_dt,  &QPushButton::clicked,  [this]() {
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    });
    //---

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainBox::show_timer_count);

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
    load_widgets();
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
    commands.append({ id++, "timer start",      &MainBox::timer_start });
    commands.append({ id++, "timer stop",       &MainBox::timer_stop });

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
#include "logdock_options.hpp"
#include "logdock_defines.hpp"

bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    beginGroup("logdock_options");

    bool flag_ReadOnly       = load_bool(FLAG_READONLY);
    bool flag_Color          = load_bool(FLAG_COLOR);
    bool flag_NoCRLF         = load_bool(FLAG_NOCRLF);
    bool flag_AddDateTime    = load_bool(FLAG_ADDDATETIME);
    bool flag_ErrorAsMessage = load_bool(FLAG_ERRORASMESSAGE);

    bool flag_LOG_EMERG     = load_bool(FLAG_LOG_EMERG);
    bool flag_LOG_ALERT     = load_bool(FLAG_LOG_ALERT);
    bool flag_LOG_CRIT      = load_bool(FLAG_LOG_CRIT);
    bool flag_LOG_ERR       = load_bool(FLAG_LOG_ERR);
    bool flag_LOG_WARNING   = load_bool(FLAG_LOG_WARNING);
    bool flag_LOG_NOTICE    = load_bool(FLAG_LOG_NOTICE);
    bool flag_LOG_INFO      = load_bool(FLAG_LOG_INFO);
    bool flag_LOG_DEBUG     = load_bool(FLAG_LOG_DEBUG);

    LogDock_options *optionsBox = new LogDock_options();
    Q_CHECK_PTR(optionsBox);

    optionsBox->setProperty(FLAG_READONLY,          flag_ReadOnly);
    optionsBox->setProperty(FLAG_COLOR,             flag_Color);
    optionsBox->setProperty(FLAG_NOCRLF,            flag_NoCRLF);
    optionsBox->setProperty(FLAG_ADDDATETIME,       flag_AddDateTime);
    optionsBox->setProperty(FLAG_ERRORASMESSAGE,    flag_ErrorAsMessage);

    optionsBox->setProperty(FLAG_LOG_EMERG,   flag_LOG_EMERG);
    optionsBox->setProperty(FLAG_LOG_ALERT,   flag_LOG_ALERT);
    optionsBox->setProperty(FLAG_LOG_CRIT,    flag_LOG_CRIT);
    optionsBox->setProperty(FLAG_LOG_ERR,     flag_LOG_ERR);
    optionsBox->setProperty(FLAG_LOG_WARNING, flag_LOG_WARNING);
    optionsBox->setProperty(FLAG_LOG_NOTICE,  flag_LOG_NOTICE);
    optionsBox->setProperty(FLAG_LOG_INFO,    flag_LOG_INFO);
    optionsBox->setProperty(FLAG_LOG_DEBUG,   flag_LOG_DEBUG);

    int res = optionsBox->exec();
    if(res == QDialog::Accepted)
    {
        save_bool(FLAG_READONLY,        optionsBox->property(FLAG_READONLY).toBool());
        save_bool(FLAG_COLOR,           optionsBox->property(FLAG_COLOR).toBool());
        save_bool(FLAG_NOCRLF,          optionsBox->property(FLAG_NOCRLF).toBool());
        save_bool(FLAG_ADDDATETIME,     optionsBox->property(FLAG_ADDDATETIME).toBool());
        save_bool(FLAG_ERRORASMESSAGE,  optionsBox->property(FLAG_ERRORASMESSAGE).toBool());

        save_bool(FLAG_LOG_EMERG,       optionsBox->property(FLAG_LOG_EMERG).toBool());
        save_bool(FLAG_LOG_ALERT,       optionsBox->property(FLAG_LOG_ALERT).toBool());
        save_bool(FLAG_LOG_CRIT,        optionsBox->property(FLAG_LOG_CRIT).toBool());
        save_bool(FLAG_LOG_ERR,         optionsBox->property(FLAG_LOG_ERR).toBool());
        save_bool(FLAG_LOG_WARNING,     optionsBox->property(FLAG_LOG_WARNING).toBool());
        save_bool(FLAG_LOG_NOTICE,      optionsBox->property(FLAG_LOG_NOTICE).toBool());
        save_bool(FLAG_LOG_INFO,        optionsBox->property(FLAG_LOG_INFO).toBool());
        save_bool(FLAG_LOG_DEBUG,       optionsBox->property(FLAG_LOG_DEBUG).toBool());
    }

    endGroup();
#endif

#if 0
    qreal deposit = 100.0;
    for(int n=0; n<30; n++)
    {
        deposit *= 1.02;
    }
    emit info(QString("deposit %1").arg(deposit));
#endif

#if 0
    qBadAlloc();
#endif

#if 0
    QTextCursor tmpCursor = ui->te_test->textCursor();
    tmpCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 4);
    tmpCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 4);
    ui->te_test->setTextCursor(tmpCursor);

    ui->te_test->textCursor().movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 5);
    ui->te_test->insertPlainText(QString("XXX: cnt = %1 ").arg(cnt++));
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
    return ui->cb_fag_exit->isChecked();
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
    qDebug() << "MainBox::load_setting";

    if(sb_test)
    {
        sb_test->setValue(load_int("sb_test"));
    }
    if(cb_block)
    {
        bool block_is_checked = load_int("cb_block");
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
