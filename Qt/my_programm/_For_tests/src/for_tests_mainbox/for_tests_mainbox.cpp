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

#include <stdint.h>
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

//---
struct modulation{
   uint16_t modType:3;               //!< \brief 0: FSK<br>
                                     //!<        1: GFSK<br>
                                     //!<        Others: <i>Reserved</i>
   uint16_t deviation:13;            //!<        Deviation (250 Hz steps)
};

struct symbolRate{
   uint32_t preScale:4;              //!<        Prescaler value
   uint32_t :4;
   uint32_t rateWord:21;             //!<        Rate word
   uint32_t decimMode:3;             //!< \brief 0: Use automatic PDIF decimation<br>
                                     //!<        1: Force PDIF decimation to 0<br>
                                     //!<        3: Force PDIF decimation to 1<br>
                                     //!<        5: Force PDIF decimation to 2<br>
                                     //!<        Others: <i>Reserved</i>
};                        //!<        Symbol rate setting

struct preamConf{
   uint8_t nPreamBytes:6;            //!< \brief 0: 1 preamble bit<br>
                                     //!<        1--16: Number of preamble bytes<br>
                                     //!<        18, 20, ..., 30: Number of preamble bytes<br>
                                     //!<        31: 4 preamble bits<br>
                                     //!<        32: 32 preamble bytes<br>
                                     //!<        Others: <i>Reserved</i>
   uint8_t preamMode:2;              //!< \brief 0: Send 0 as the first preamble bit<br>
                                     //!<        1: Send 1 as the first preamble bit<br>
                                     //!<        2: Send same first bit in preamble and sync word<br>
                                     //!<        3: Send different first bit in preamble and sync word
};

struct formatConf{
   uint16_t nSwBits:6;               //!<        Number of sync word bits (8--32)
   uint16_t bBitReversal:1;          //!< \brief 0: Use positive deviation for 1<br>
                                     //!<        1: Use positive deviation for 0
   uint16_t bMsbFirst:1;             //!< \brief 0: Least significant bit transmitted first<br>
                                     //!<        1: Most significant bit transmitted first
   uint16_t fecMode:4;               //!< \brief Select coding<br>
                                     //!<        0: Uncoded binary modulation<br>
                                     //!<        8: Long range mode<br>
                                     //!<        10: Manchester coded binary modulation<br>
                                     //!<        Others: <i>Reserved</i>
   uint16_t :1;
   uint16_t whitenMode:3;            //!< \brief 0: No whitening<br>
                                     //!<        1: CC1101/CC2500 compatible whitening<br>
                                     //!<        2: PN9 whitening without byte reversal<br>
                                     //!<        3: <i>Reserved</i><br>
                                     //!<        4: No whitener, 32-bit IEEE 802.15.4g compatible CRC<br>
                                     //!<        5: IEEE 802.15.4g compatible whitener and 32-bit CRC<br>
                                     //!<        6: No whitener, dynamically IEEE 802.15.4g compatible 16-bit or 32-bit CRC<br>
                                     //!<        7: Dynamically IEEE 802.15.4g compatible whitener and 16-bit or 32-bit CRC
};
//---

union u_modulation
{
    quint16 value;
    struct modulation{
       uint16_t modType:3;               //!< \brief 0: FSK<br>
                                         //!<        1: GFSK<br>
                                         //!<        Others: <i>Reserved</i>
       uint16_t deviation:13;            //!<        Deviation (250 Hz steps)
    } bytes;
};

union u_symbolRate
{
    quint32 value;
    struct symbolRate{
       uint32_t preScale:4;              //!<        Prescaler value
       uint32_t :4;
       uint32_t rateWord:21;             //!<        Rate word
       uint32_t decimMode:3;             //!< \brief 0: Use automatic PDIF decimation<br>
                                         //!<        1: Force PDIF decimation to 0<br>
                                         //!<        3: Force PDIF decimation to 1<br>
                                         //!<        5: Force PDIF decimation to 2<br>
                                         //!<        Others: <i>Reserved</i>
    } bytes;
};

union u_preamConf
{
    quint8 value;
    struct preamConf{
       uint8_t nPreamBytes:6;            //!< \brief 0: 1 preamble bit<br>
                                         //!<        1--16: Number of preamble bytes<br>
                                         //!<        18, 20, ..., 30: Number of preamble bytes<br>
                                         //!<        31: 4 preamble bits<br>
                                         //!<        32: 32 preamble bytes<br>
                                         //!<        Others: <i>Reserved</i>
       uint8_t preamMode:2;              //!< \brief 0: Send 0 as the first preamble bit<br>
                                         //!<        1: Send 1 as the first preamble bit<br>
                                         //!<        2: Send same first bit in preamble and sync word<br>
                                         //!<        3: Send different first bit in preamble and sync word
    } bytes;
};

union u_formatConf
{
    quint16 value;
    struct formatConf{
       uint16_t nSwBits:6;               //!<        Number of sync word bits (8--32)
       uint16_t bBitReversal:1;          //!< \brief 0: Use positive deviation for 1<br>
                                         //!<        1: Use positive deviation for 0
       uint16_t bMsbFirst:1;             //!< \brief 0: Least significant bit transmitted first<br>
                                         //!<        1: Most significant bit transmitted first
       uint16_t fecMode:4;               //!< \brief Select coding<br>
                                         //!<        0: Uncoded binary modulation<br>
                                         //!<        8: Long range mode<br>
                                         //!<        10: Manchester coded binary modulation<br>
                                         //!<        Others: <i>Reserved</i>
       uint16_t :1;
       uint16_t whitenMode:3;            //!< \brief 0: No whitening<br>
                                         //!<        1: CC1101/CC2500 compatible whitening<br>
                                         //!<        2: PN9 whitening without byte reversal<br>
                                         //!<        3: <i>Reserved</i><br>
                                         //!<        4: No whitener, 32-bit IEEE 802.15.4g compatible CRC<br>
                                         //!<        5: IEEE 802.15.4g compatible whitener and 32-bit CRC<br>
                                         //!<        6: No whitener, dynamically IEEE 802.15.4g compatible 16-bit or 32-bit CRC<br>
                                         //!<        7: Dynamically IEEE 802.15.4g compatible whitener and 16-bit or 32-bit CRC
    } bytes;
};

#pragma pack (pop)
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
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");

#if 1
    emit info(metaObject()->className());
#endif

#if 0
    const QString program = "kpat";
    const QStringList arguments = QStringList();
    QProcess process;
    process.setWorkingDirectory(".");
    process.start(program, arguments);
    bool res = process.waitForFinished(-1);
    emit info(QString("programm return %1").arg(res ? "true" : "false"));
#endif

#if 0
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);
    QList<QWidget *> l_widgets = mw->findChildren<QWidget *>();
    emit info(QString("find %1 widgets").arg(l_widgets.count()));

    foreach (QWidget *w, l_widgets)
    {
        if(w->objectName().left(3) != "qt_")
        {
            emit info(w->objectName());
        }
    }
#endif

#if 0
    u_modulation u_m;
    u_m.value = 0x000000a1;
    emit info("--- modulation ---");
    emit info(QString("modType 0x%1").arg(u_m.bytes.modType, 0, 16));
    emit info(QString("deviation0x%1").arg(u_m.bytes.deviation, 0, 16));

    u_symbolRate u_s;
    u_s.value = 0x0033330f;
    emit info("--- symbolRate ---");
    emit info(QString("preScale 0x%1").arg(u_s.bytes.preScale, 0, 16));
    emit info(QString("rateWord 0x%1").arg(u_s.bytes.rateWord, 0, 16));
    emit info(QString("decimMode 0x%1").arg(u_s.bytes.decimMode, 0, 16));

    u_preamConf  u_p;
    u_p.value = 0x00000002;
    emit info("--- preamConf ---");
    emit info(QString("preamMode 0x%1").arg(u_p.bytes.preamMode, 0, 16));
    emit info(QString("nPreamBytes 0x%1").arg(u_p.bytes.nPreamBytes, 0, 16));

    u_formatConf u_f;
    u_f.value = 0x00002820;
    emit info("--- formatConf ---");
    emit info(QString("fecMode 0x%1").arg(u_f.bytes.fecMode, 0, 16));
    emit info(QString("nSwBits 0x%1").arg(u_f.bytes.nSwBits, 0, 16));
    emit info(QString("bMsbFirst 0x%1").arg(u_f.bytes.bMsbFirst, 0, 16));
    emit info(QString("whitenMode 0x%1").arg(u_f.bytes.whitenMode, 0, 16));
    emit info(QString("bBitReversal 0x%1").arg(u_f.bytes.bBitReversal, 0, 16));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");

#if 0
    emit info("Текст <font style=\"color:red\">красный</font>");
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
    //emit debug("load_setting !");
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    //emit debug("save_setting !");
}
//--------------------------------------------------------------------------------
