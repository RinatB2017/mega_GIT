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
#include <QApplication>
#include <QMouseEvent>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
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
    load_config();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    //---
    QStringList sl;
    sl.append("head0");
    sl.append("head1");
    sl.append("head2");
    sl.append("head3");
    sl.append("head4");

    ui->tableWidget->setColumnCount(sl.count());
    ui->tableWidget->setHorizontalHeaderLabels(sl);

    //QHeaderView *header = ui->tableWidget->horizontalHeader();
    //header->setIndexWidget(QModelIndex().child(0, 0), new QCheckBox("head", ui->tableWidget));
    //ui->tableWidget->horizontalHeader()->setIndexWidget(QModelIndex().child(1, 0), new QCheckBox("head", ui->tableWidget));

    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //---

    MyMainWindow *mw = dynamic_cast<MyMainWindow *>(parent());
    if(mw)
    {
        connect(mw, SIGNAL(notifySignal()), this,   SLOT(test_1()));
    }

    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    Q_CHECK_PTR(tool_bar);
    Q_CHECK_PTR(tool_button);
    if(tool_bar == nullptr)
    {
        return nullptr;
    }
    if(tool_button == nullptr)
    {
        return nullptr;
    }

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *toolBar = new QToolBar("testbar");
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QCheckBox *cb_block = new QCheckBox("block");
    toolBar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    toolBar->setFixedWidth(toolBar->sizeHint().width() + 10);
}
//--------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------
#include <QThread>
void MainBox::thread_is_finished(void)
{
    emit info("thread_is_finished");
}
//--------------------------------------------------------------------------------
void MainBox::create_thread(void)
{
    QThread *thread = new QThread;

    MyThread *worker = new MyThread;
    connect(worker, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(worker, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(worker, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(worker, SIGNAL(message(QString)),   this, SIGNAL(message(QString)));

    connect(thread, SIGNAL(started()),  worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, SIGNAL(finished()), this, SLOT(thread_is_finished()));

    worker->moveToThread(thread);

    thread->start();
}
//--------------------------------------------------------------------------------
#include <QRCodeGenerator.hpp>

void MainBox::updateQRImage(void)
{
    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;

    CQR_Encode qrEncode;

    QByteArray ba;
    ba.clear();
    for(int n=0; n<2754; n++)   //2376 2953
    {
        ba.append(n);
    }
    emit info(QString("len %1 bytes").arg(ba.size()));
    bool successfulEncoding = qrEncode.EncodeData( levelIndex, versionIndex, bExtent, maskIndex, ba.toHex().data(), ba.size() );

    if ( !successfulEncoding )
    {
        emit error("!successfulEncoding");
        return;
    }

    int qrImageSize = qrEncode.m_nSymbleSize;

    // Создаем двумерный образ кода
    int encodeImageSize = qrImageSize + ( QR_MARGIN * 2 );
    QImage encodeImage( encodeImageSize, encodeImageSize, QImage::Format_Mono );
    encodeImage.fill( 1 );

    // Создать двумерный образ кода
    for ( int i = 0; i < qrImageSize; i++ )
        for ( int j = 0; j < qrImageSize; j++ )
            if ( qrEncode.m_byModuleData[i][j] )
                encodeImage.setPixel( i + QR_MARGIN, j + QR_MARGIN, 0 );

    QLabel *image_label = new QLabel();
    image_label->setPixmap( QPixmap::fromImage( encodeImage.scaled(600, 600) ) );
    image_label->show();
}
//--------------------------------------------------------------------------------
void MainBox::get_param(QString *str, QWidget *widget, QString w_name)
{
    int R = widget->palette().color(QPalette::WindowText).red();
    int G = widget->palette().color(QPalette::WindowText).red();
    int B = widget->palette().color(QPalette::WindowText).red();

    int b_R = widget->palette().background().color().red();
    int b_G = widget->palette().background().color().green();
    int b_B = widget->palette().background().color().blue();

    (*str).append(w_name);
    (*str).append(" {");
    (*str).append("\n");
    (*str).append(QString("    color:#%1%2%3;")
                  .arg(R, 0, 16)
                  .arg(G, 0, 16)
                  .arg(B, 0, 16));
    (*str).append("\n");
    (*str).append(QString("    background-color:#%1%2%3;")
                  .arg(b_R, 0, 16)
                  .arg(b_G, 0, 16)
                  .arg(b_B, 0, 16));
    (*str).append("\n");
    (*str).append("}");
    (*str).append("\n");
}
//--------------------------------------------------------------------------------
void MainBox::get_color(QPalette::ColorRole role, QString r_name)
{
    int R = palette().color(role).red();
    int G = palette().color(role).red();
    int B = palette().color(role).red();

    QString str;
    str.append(QString("%1    color:#%2%3%4;")
               .arg(r_name)
               .arg(R, 0, 16)
               .arg(G, 0, 16)
               .arg(B, 0, 16));
    str.append("\n");

    emit info(str);
}
//--------------------------------------------------------------------------------
#include <QPlainTextEdit>
#include <QStackedWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QSizeGrip>
#include <QToolTip>
#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include <QDateEdit>
#include <QScrollBar>
#include <QHeaderView>
#include <QSizeGrip>
#include <QMenu>
#include <QDockWidget>
#include <QHeaderView>
#include <QProgressBar>

bool MainBox::test_0(void)
{
    //emit info("Test_0()");

    //updateQRImage();

#if 0
    QString str;
    //get_param(&str, new QToolTip,   "QToolTip");
    get_param(&str, new QWidget,    "QWidget");
    get_param(&str, new QCheckBox,  "QCheckBox");
    get_param(&str, new QGroupBox,  "QGroupBox");
    get_param(&str, new QRadioButton,   "QRadioButton");
    get_param(&str, new QMenuBar,   "QMenuBar");
    get_param(&str, new QMenu,   "QMenu");
    //get_param(&str, new QAbstractItemView,   "QAbstractItemView");
    get_param(&str, new QTabWidget,   "QTabWidget");
    get_param(&str, new QLineEdit,   "QLineEdit");
    get_param(&str, new QAbstractScrollArea,   "QAbstractScrollArea");
    get_param(&str, new QScrollBar,   "QScrollBar");
    get_param(&str, new QTextEdit,   "QTextEdit");
    get_param(&str, new QPlainTextEdit,   "QPlainTextEdit");
    get_param(&str, new QHeaderView(Qt::Horizontal),   "QHeaderView");
    get_param(&str, new QSizeGrip(this),   "QSizeGrip");
    get_param(&str, new QMainWindow,   "QMainWindow");
    get_param(&str, new QFrame,   "QFrame");
    get_param(&str, new QStackedWidget,   "QStackedWidget");
    get_param(&str, new QToolBar,   "QToolBar");
    get_param(&str, new QToolButton,   "QToolButton");
    get_param(&str, new QPushButton,   "QPushButton");
    get_param(&str, new QComboBox,   "QComboBox");
    get_param(&str, new QAbstractSpinBox,   "QAbstractSpinBox");
    get_param(&str, new QLabel,   "QLabel");
    get_param(&str, new QTabWidget,   "QTabWidget");
    get_param(&str, new QTabBar,   "QTabBar");
    get_param(&str, new QDockWidget,   "QDockWidget");
    get_param(&str, new QTreeView,   "QTreeView");
    get_param(&str, new QListView,   "QListView");
    get_param(&str, new QSlider,   "QSlider");
    get_param(&str, new QTableView,   "QTableView");
    //get_param(&str, new QTableCornerButton,   "QTableCornerButton");
    get_param(&str, new QStatusBar,   "QStatusBar");
    get_param(&str, new QFrame,   "QFrame");
    get_param(&str, new QSplitter,   "QSplitter");
    get_param(&str, new QProgressBar,   "QProgressBar");
    get_param(&str, new QDateEdit,   "QDateEdit");

    emit info(str);
#endif

#if 1
    get_color(QPalette::WindowText, "QPalette::WindowText");
    get_color(QPalette::Button,     "QPalette::Button");
    get_color(QPalette::Light,      "QPalette::Light");
    get_color(QPalette::Text,       "QPalette::Text");
    get_color(QPalette::BrightText, "QPalette::BrightText");
    get_color(QPalette::ButtonText, "QPalette::ButtonText");
    get_color(QPalette::Base,       "QPalette::Base");
    get_color(QPalette::Window,     "QPalette::Window");
    get_color(QPalette::Shadow,     "QPalette::Shadow");
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    //QApplication::setStyle(QStyleFactory::create("Plastique"));

    messagebox_question("Вопрос", "Шо, опять?");
    //messagebox_critical("Вопрос", "Шо, опять?");
    //QMessageBox::question(this, "Вопрос", "Шо, опять?");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
    {
        if (static_cast<QMouseEvent*>(e)->button() == Qt::RightButton)
        {
            return true;
        }
    }
    return false;
}
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
