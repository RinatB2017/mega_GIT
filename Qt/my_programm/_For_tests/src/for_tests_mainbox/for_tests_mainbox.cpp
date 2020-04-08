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
#include "ui_for_tests_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "for_tests_mainbox.hpp"

#include "simplewidget.hpp"

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

    if(sw)
    {
        sw->deleteLater();
    }

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

    createTestBar();

    ui->horizontalSlider->setRange(0, 1000);

    connect(ui->sb_1,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_2,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_res, SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->btn_ok, SIGNAL(clicked(bool)),      this,   SLOT(victory()));
    check_in();

    ui->toolButton->setIcon((QIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon))));
    connect(ui->toolButton, &QToolButton::clicked,  this,   &MainBox::delete_string);

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
    load_widgets();

#if 1
    sw = new SimpleWidget();
    sw->show();
#endif

    //TODO xxx
    ui->btn_ok->setProperty("xxx", 1);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainBox::test_2);
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

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QCheckBox *cb_block = new QCheckBox("block");
    cb_block->setObjectName("cb_block");
    testbar->addWidget(cb_block);

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

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
    return qrand() % 10;
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
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setValidator(ipValidator);
    lineEdit->show();
}
//--------------------------------------------------------------------------------
bool MainBox::create_color_block(int width,
                                 int height,
                                 int w_border,
                                 QColor color_border,
                                 QColor color,
                                 QString path,
                                 QString filename)
{
    QPixmap *pixmap = new QPixmap(width, height);
    QPainter *painter = new QPainter(pixmap);
    painter->fillRect(0,
                      0,
                      pixmap->width(),
                      pixmap->height(),
                      QBrush(color_border));
    painter->fillRect(w_border,
                      w_border,
                      pixmap->width()-w_border*2,
                      pixmap->height()-w_border*2,
                      QBrush(color));

    bool ok = pixmap->save(path + filename);
    if(ok)
    {
        emit info(QString("File %1 successfully created").arg(path + filename));
    }
    else
    {
        emit error(QString("Error create %1").arg(path + filename));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::test_assert(int value)
{
    Q_ASSERT(value > 5);    // если значение > 5, то всё нормально
    return value != 0;
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
bool MainBox::load_property(QWidget *widget, const QString &property_name)
{
    QVariant v = widget->property(property_name.toLocal8Bit());
    if(v.isValid() == false)
    {
        return false;
    }
    emit info(v.toString());
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::save_property(QWidget *widget, const QString &property_name, QVariant value)
{
    return widget->setProperty(property_name.toLocal8Bit(), value);
}
//--------------------------------------------------------------------------------
#include "creatorbutton.hpp"

bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");

#if 1
    CreatorButton *cb = new CreatorButton();
    connect_log_signals(cb, this);
    cb->show();
#endif

#if 0
    QTextEdit *te = new QTextEdit();
    te->setTextColor(QColor(Qt::red));
    te->insertPlainText("red");
    te->setTextColor(QColor(Qt::green));
    te->insertPlainText("green");
    te->setTextColor(QColor(Qt::blue));
    te->insertPlainText("blue");
    te->show();
#endif

#if 0
    timer->start(500);
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
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");

#if 0
    timer->stop();
#endif

#if 0
    setProperty("is_exit", false);
#endif


#if 0
    emit info("Текст <font style=\"color:red\">красный</font>");
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    //emit info("Test_2()");

#if 0
    emit info(QString("cnt %1").arg(cnt++));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_5()");
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
    emit debug("load_setting !");
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    emit debug("save_setting !");
}
//--------------------------------------------------------------------------------
