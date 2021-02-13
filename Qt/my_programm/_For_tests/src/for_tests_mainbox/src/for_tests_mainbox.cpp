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

#include "for_tests_mainbox.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "myfiledialog.hpp"
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
        disconnect(timer, &QTimer::timeout, this, &MainBox::show_timer_count);

        timer->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainBox::show_timer_count);

    connect_log_signals(ui->controls_widget, this);

    // add_lcd_clock();
    // add_digital_clock(false);
    add_digital_clock();

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif

#if 1
    QTimer::singleShot(0, [this]{
        MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
        if(mw)
        {
            mw->add_mdi_sorting();
            //TODO проверка корректного переноса frame в dockwidget
            mw->add_dock_widget("test_dock",
                                "test_dock",
                                Qt::BottomDockWidgetArea,
                                ui->test_frame);
        }
        load_widgets();
    });
#endif
}
//--------------------------------------------------------------------------------
bool MainBox::set_theme_windows(void)
{
    QString filename = ":/themes_css/Theme (Windows).css";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_norton_commander(void)
{
    QString filename = ":/themes_qss/Norton Commander.qss";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_styles(void)
{
    QString filename = ":/themes_qss/styles.qss";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",             &MainBox::test });
    commands.append({ id++, "load QSS",         &MainBox::load_qss });
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
    cb_test->setProperty(NO_SAVE, true);
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

    // testbar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

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
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#else
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
#endif

    /* Set Validator on QLineEdit */
    QLineEdit *lineEdit = new QLineEdit();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#else
    lineEdit->setValidator(ipValidator);
#endif
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
    Q_ASSERT(timer);
    timer->start(500);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::timer_stop(void)
{
    Q_ASSERT(timer);
    timer->stop();
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::show_timer_count(void)
{
    emit info(QString("cnt %1").arg(cnt++));
}
//--------------------------------------------------------------------------------
bool MainBox::load_qss(void)
{
    emit trace(Q_FUNC_INFO);

    MyFileDialog *dlg = new MyFileDialog("qss_file", "qss_file");
    dlg->setNameFilter("QSS files (*.qss)");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();

        QFile file(files.at(0));
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        file.close();
        qApp->setStyleSheet(styleSheet);
    }
    dlg->deleteLater();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 0
    QWidgetList widgets;
    widgets = qApp->allWidgets();
    emit info(QString("Found %1 widgets").arg(widgets.count()));

    QPushButton *w = new QPushButton();
    w->show();

    widgets = qApp->allWidgets();
    emit info(QString("Found %1 widgets").arg(widgets.count()));

    delete w;
#endif

#if 1
    emit info(QString("ver. [%1]").arg(qApp->applicationVersion()));
#endif

#if 0
    emit info("Copyright \\251 2020-2025");
#endif

#if 0
    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");
    //emit colorLog("yellow on blue", Qt::yellow, Qt::blue);
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
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainBox::load_setting";
#endif

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
#ifdef QT_DEBUG
    qDebug() << "MainBox::save_setting";
#endif

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
