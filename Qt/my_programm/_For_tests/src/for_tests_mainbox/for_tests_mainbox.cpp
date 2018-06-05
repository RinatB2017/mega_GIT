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
    if(test_widget)
    {
        test_widget->close();
        test_widget->deleteLater();
    }
    save_widgets("for_test");

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    //---
    test_widget = new QWidget;
    test_widget->setMinimumSize(400, 400);
    test_widget->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    test_widget->show();
    //---

    createTestBar();

    connect(ui->sb_1,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_2,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_res, SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->btn_ok, SIGNAL(clicked(bool)),      this,   SLOT(victory()));
    check_in();

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
    load_widgets("for_test");
}
//--------------------------------------------------------------------------------
void MainBox::check_in(void)
{
    int a = ui->sb_1->value();
    int b = ui->sb_2->value();
    int c = ui->sb_res->value();

#if 0
    emit debug("check_in");
    emit debug(QString("a %1").arg(a));
    emit debug(QString("b %1").arg(b));
    emit debug(QString("c %1").arg(c));
#endif

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
    commands.append({ ID_TEST_6, "test 6", 0 });

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
}
//--------------------------------------------------------------------------------
#if 0
//TODO как нибудь потом
bool MainBox::no_exit(void)
{
    return false;
}
#endif
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
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
    QMessageBox::information(0,"","info");
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
    /* Create a regular expression with a string
     * as a repeating element
     */
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    /* Create a validation regular expression
     * using a regular expression
     */
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    /* Set Validator on QLineEdit */
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setValidator(ipValidator);
    lineEdit->show();
}
//--------------------------------------------------------------------------------
#pragma pack(push, 1)

typedef struct pet_frame {
    uint8_t  addr;          /* Адрес ведомого устройства */
    uint8_t  cmd;           /* Код команды */
    uint16_t len;           /* Размер поля данных (байт) */
    uint8_t  data[];        /* Данные или код ошибки выполнения команды */
} pet_frame_t;

typedef struct pet_event {
    uint8_t  src;           /* Источник события */
    uint8_t  flags;         /* Информационное поле (флаги) события */
    uint16_t ts_fract;      /* Метка времени - дробная часть */
    uint32_t ts;            /* Метка времени - целая часть */
    uint16_t xp;            /* Безразмерное значение координатного сигнала X+ */
    uint16_t xn;            /* Безразмерное значение координатного сигнала X- */
    uint16_t yp;            /* Безразмерное значение координатного сигнала Y+ */
    uint16_t yn;            /* Безразмерное значение координатного сигнала Y- */
} pet_event_t;

#pragma pack(pop)
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");

#if 1
    emit debug(QString("sizeof %1").arg(sizeof(pet_frame)));

    QByteArray ba;
    ba.resize(sizeof(pet_frame) + sizeof(pet_event));
    pet_frame_t *header = (pet_frame_t *)ba.data();
    pet_event_t *data   = (pet_event_t *)header->data;

    header->addr = 0;
    header->cmd = 1;
    header->len = sizeof(pet_event_t);

    data->src = 2;
    data->flags = 3;
    data->ts_fract = 4;
    data->ts = 5;
    data->xp = 6;
    data->xn = 7;
    data->yp = 8;
    data->yn = 9;

    //---
    QByteArray ba2;
    ba2.clear();
    ba2.append(ba);

    pet_frame_t *header2 = (pet_frame_t *)ba2.data();
    pet_event_t *data2   = (pet_event_t *)header->data;

    emit info("---");
    emit info(QString("addr = %1").arg(header2->addr));
    emit info(QString("cmd = %1").arg(header2->cmd));
    emit info(QString("len = %1").arg(header2->len));
    emit info("---");
    emit info(QString("src = %1").arg(data2->src));
    emit info(QString("flags = %1").arg(data2->flags));
    emit info(QString("ts_fract = %1").arg(data2->ts_fract));
    emit info(QString("ts = %1").arg(data2->ts));
    emit info(QString("xp = %1").arg(data2->xp));
    emit info(QString("xn = %1").arg(data2->xn));
    emit info(QString("yp = %1").arg(data2->yp));
    emit info(QString("yn = %1").arg(data2->yn));
    emit info("---");
#endif

#if 0
    check_tooltips();
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");

#if 0
    emit info("info");
    emit debug("debug");
    emit error("error");
    emit trace("trace");

    emit colorLog("Cyan", QColor(Qt::cyan), QColor(Qt::black));
    emit colorLog("Yellow", QColor(Qt::yellow), QColor(Qt::white));
#endif

#if 0
    emit info(__FILE__);
    emit info(QString("line %1").arg(__LINE__));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_2()");

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
quint32 MainBox::test(const QByteArray ba)
{
    emit trace(Q_FUNC_INFO);
    quint32 temp = 0;

    for(int n=0; n<ba.length(); n++)
    {
        temp += (char)ba.at(n);
    }

    return temp;
}
//--------------------------------------------------------------------------------
