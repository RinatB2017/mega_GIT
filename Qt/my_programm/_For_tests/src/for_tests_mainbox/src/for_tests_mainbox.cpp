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

#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "for_tests_mainbox.hpp"
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

#if 0
    // create shortcut
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this);

    // connect its 'activated' signal to close application
    QObject::connect(shortcut,    &QShortcut::activated,    qApp,   &QApplication::quit);
    //---
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
    connect(ui->de_begin,   &QDateEdit::editingFinished,  [this]() {
        emit info("correct min");
        ui->de_test->setMinimumDate(ui->de_begin->date());
    });
    connect(ui->de_end,   &QDateEdit::editingFinished,  [this]() {
        emit info("correct max");
        ui->de_test->setMaximumDate(ui->de_end->date());
    });

    QTimer::singleShot(100, [this]{
        if(ui->de_test->minimumDate() != ui->de_begin->date())
        {
            ui->de_test->setMinimumDate(ui->de_begin->date());
        }
        if(ui->de_test->maximumDate() != ui->de_end->date())
        {
            ui->de_test->setMaximumDate(ui->de_end->date());
        }
        emit info("CORRECT");
    });
    //---

    //---
    ui->hex_widget->setMaximum(std::numeric_limits<qlonglong>::max());
    ui->hex_widget->setMinimum(std::numeric_limits<qlonglong>::min());
    //---

    //---
    mdi_widget_0 = new QLineEdit(this);
    mdi_widget_0->setObjectName("widget_0");
    mdi_widget_0->setAttribute(Qt::WA_DeleteOnClose);

    mdi_widget_1 = new QLineEdit(this);
    mdi_widget_1->setObjectName("widget_1");
    mdi_widget_1->setAttribute(Qt::WA_DeleteOnClose);

    mdi_widget_2 = new QLineEdit(this);
    mdi_widget_2->setObjectName("widget_2");
    mdi_widget_2->setAttribute(Qt::WA_DeleteOnClose);

    w0 = ui->mdiArea->addSubWindow(mdi_widget_0);
    w1 = ui->mdiArea->addSubWindow(mdi_widget_1);
    w2 = ui->mdiArea->addSubWindow(mdi_widget_2);

    w0->setObjectName("mdi_widget0");
    w1->setObjectName("mdi_widget1");
    w2->setObjectName("mdi_widget2");
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
    file_model = new QFileSystemModel(this);
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

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        for(int n=0; n<5; n++)
        {
            QLabel *statusLabel1 = new QLabel();
            statusLabel1->setText(QString("SL %1").arg(n));

            mw->statusBar()->addWidget(statusLabel1);
        }
    }

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
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",             &MainBox::test });
    commands.append({ id++, "test2",            &MainBox::test2 });
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

    //    testbar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    // QList<MainBox::CMD>::iterator cmd_it = std::find_if(
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
void MainBox::heavy_function(void)
{
    QElapsedTimer timer;
    int cnt = 9;
    while(cnt > 0)
    {
        timer.start();
        while(timer.elapsed() < 1000)
        {

        }
        emit info(QString("%1").arg(cnt));
        cnt--;
    }
    emit info("OK");
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
#include <QMediaPlayer>

bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    QMediaPlayer *player = new QMediaPlayer;
    connect(player, &QMediaPlayer::durationChanged, this, [&](qint64 duration) {
        int seconds = (duration/1000) % 60;
        int minutes = (duration/60000) % 60;
        int hours = (duration/3600000) % 24;

        QTime time(hours, minutes,seconds);
        emit info(time.toString("hh:mm:ss"));

        emit info(QDateTime::fromSecsSinceEpoch(duration / 1000.0, Qt::UTC).toString("hh:mm:ss"));
    });

    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators("/dev/shm/Seal-Crazy lyrics-c45a8Htv3c0.webm.mp3")));
    player->setVolume(50);
    player->play();
#endif

#if 0
    QDateTime birth(QDate(1969, 11, 28), QTime(0, 0, 0), QTimeZone(Qt::UTC));
    QString birth_str = birth.toString("ddMMyyyy");
    emit info(QString("birth [%1]").arg(birth_str));
    int sum = 0;
    for(int i=0; i<birth_str.length(); i++)
    {
        sum += birth_str.mid(i, 1).toInt();
    }
    emit info(QString("sum %1").arg(sum));
#endif


#if 0
    int cnt = ui->te_test->document()->lineCount();
    emit info(QString("Lines %1").arg(cnt));

    QString data = ui->te_test->toPlainText();
    QStringList strList = data.split(QRegExp("[\n]"), Qt::SkipEmptyParts);
    emit info(QString("Lines %1").arg(strList.count()));
    for(int n=0; n<strList.count(); n++)
    {
        emit info(QString("line %1 [%2]")
                  .arg(n)
                  .arg(strList.at(n)));
    }
#endif

#if 0
    QString temp_str = "\t0 1  2   3    4     5";
    emit info(temp_str);
    temp_str = temp_str.replace(QRegExp("\\s{2,}"), " ").remove("\t");  //замена нескольких пробелов на один
    emit info(temp_str);
#endif

#if 0
    ui->te_test->setHtml("<a href='http://www.w3schools.com/'>Link!</a>aah");
#endif

#if 0
    QDateTime dt1(QDate(2020, 1,  1), QTime(0, 0, 0), QTimeZone(Qt::LocalTime));
    QDateTime dt2(QDate(2020, 12, 31), QTime(0, 0, 0), QTimeZone(Qt::LocalTime));
    emit info(QString("diff %1").arg(dt1.daysTo(dt2)));
#endif

#if 0
    QDateTime dt(QDate(1970, 1, 1), QTime(0, 0, 0), QTimeZone(Qt::LocalTime));
    //    dt.setDate(QDate(1970, 1, 1));
    //    dt.setTime(QTime(0, 0, 0));
    //    dt.setTimeZone(QTimeZone(Qt::LocalTime));
    emit info(QString("sec %1").arg(dt.toSecsSinceEpoch()));
    qDebug() << dt;

    qint64 sec_in_day = 24 * 60 * 60;
    dt = dt.addSecs(sec_in_day);
    emit info(QString("sec %1").arg(dt.toSecsSinceEpoch()));
    qDebug() << dt;
#endif

#if 0
    QList<QMdiSubWindow *> l_sw = ui->mdiArea->subWindowList();
    emit info(QString("cnt %1").arg(l_sw.count()));
#endif

#if 0
    QDateTime dt(QDate(2000, 1, 1), QTime(0, 0, 0));
    QString time = QString("%1").arg(dt.toTime_t(), 0, 16);
    QByteArray ba = QByteArray::fromHex(time.toLatin1());
    emit info(QString("dt [%1]").arg(ba.toHex().data()));
#endif

#if 0
    QString frame_name = "test_frame";
    QDockWidget *dw = topLevelWidget()->findChild<QDockWidget *>(frame_name);
    if(dw)
    {
        if(dw->x() > 0)
        {
            messagebox_info("INFO", "FOUND");
        }
        else
        {
            messagebox_critical("ERROR", "FAIL");
        }
    }
    else
    {
        emit error(QString("%1 not found")
                   .arg(frame_name));
    }
#endif

#if 0
    TestStack ts;
    ts.set("test");
    l_class.push(ts);

    TestStack ts2;
    ts2.set("test2");
    l_class.push(ts2);
#endif

#if 0
    qDebug() << "pointer_ptr" << pointer_ptr;
    qDebug() << "shared_data_ptr" << shared_data_ptr;
    qDebug() << "scoped_ptr" << scoped_ptr;
    qDebug() << "weak_ptr" << weak_ptr;
    qDebug() << "atomic_ptr" << atomic_ptr;

    qDebug() << "-----";

    QScopedPointer<QWidget> scoped_ptr(new QWidget());
    qDebug() << "scoped_ptr" << scoped_ptr;

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
bool MainBox::test2(void)
{
    if(l_class.count() > 0)
    {
        TestStack test = l_class.pop();
        emit info(test.get());
    }

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
