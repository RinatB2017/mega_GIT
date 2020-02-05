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

//struct rfc_CMD_PROP_RADIO_DIV_SETUP_s {
//   uint16_t commandNo;                  //!<        The command ID number 0x3807
//   uint16_t status;                     //!< \brief An integer telling the status of the command. This value is
//                                        //!<        updated by the radio CPU during operation and may be read by the
//                                        //!<        system CPU at any time.
//   rfc_radioOp_t *pNextOp;              //!<        Pointer to the next operation to run after this operation is done
//   ratmr_t startTime;                   //!<        Absolute or relative start time (depending on the value of <code>startTrigger</code>)
//   struct {
//      uint8_t triggerType:4;            //!<        The type of trigger
//      uint8_t bEnaCmd:1;                //!< \brief 0: No alternative trigger command<br>
//                                        //!<        1: CMD_TRIGGER can be used as an alternative trigger
//      uint8_t triggerNo:2;              //!<        The trigger number of the CMD_TRIGGER command that triggers this action
//      uint8_t pastTrig:1;               //!< \brief 0: A trigger in the past is never triggered, or for start of commands, give an error<br>
//                                        //!<        1: A trigger in the past is triggered as soon as possible
//   } startTrigger;                      //!<        Identification of the trigger that starts the operation
//   struct {
//      uint8_t rule:4;                   //!<        Condition for running next command: Rule for how to proceed
//      uint8_t nSkip:4;                  //!<        Number of skips + 1 if the rule involves skipping. 0: same, 1: next, 2: skip next, ...
//   } condition;
//   struct {
//      uint16_t modType:3;               //!< \brief 0: FSK<br>
//                                        //!<        1: GFSK<br>
//                                        //!<        Others: <i>Reserved</i>
//      uint16_t deviation:13;            //!<        Deviation (250 Hz steps)
//   } modulation;
//   struct {
//      uint32_t preScale:4;              //!<        Prescaler value
//      uint32_t :4;
//      uint32_t rateWord:21;             //!<        Rate word
//      uint32_t decimMode:3;             //!< \brief 0: Use automatic PDIF decimation<br>
//                                        //!<        1: Force PDIF decimation to 0<br>
//                                        //!<        3: Force PDIF decimation to 1<br>
//                                        //!<        5: Force PDIF decimation to 2<br>
//                                        //!<        Others: <i>Reserved</i>
//   } symbolRate;                        //!<        Symbol rate setting
//   uint8_t rxBw;                        //!<        Receiver bandwidth
//   struct {
//      uint8_t nPreamBytes:6;            //!< \brief 0: 1 preamble bit<br>
//                                        //!<        1--16: Number of preamble bytes<br>
//                                        //!<        18, 20, ..., 30: Number of preamble bytes<br>
//                                        //!<        31: 4 preamble bits<br>
//                                        //!<        32: 32 preamble bytes<br>
//                                        //!<        Others: <i>Reserved</i>
//      uint8_t preamMode:2;              //!< \brief 0: Send 0 as the first preamble bit<br>
//                                        //!<        1: Send 1 as the first preamble bit<br>
//                                        //!<        2: Send same first bit in preamble and sync word<br>
//                                        //!<        3: Send different first bit in preamble and sync word
//   } preamConf;
//   struct {
//      uint16_t nSwBits:6;               //!<        Number of sync word bits (8--32)
//      uint16_t bBitReversal:1;          //!< \brief 0: Use positive deviation for 1<br>
//                                        //!<        1: Use positive deviation for 0
//      uint16_t bMsbFirst:1;             //!< \brief 0: Least significant bit transmitted first<br>
//                                        //!<        1: Most significant bit transmitted first
//      uint16_t fecMode:4;               //!< \brief Select coding<br>
//                                        //!<        0: Uncoded binary modulation<br>
//                                        //!<        8: Long range mode<br>
//                                        //!<        10: Manchester coded binary modulation<br>
//                                        //!<        Others: <i>Reserved</i>
//      uint16_t :1;
//      uint16_t whitenMode:3;            //!< \brief 0: No whitening<br>
//                                        //!<        1: CC1101/CC2500 compatible whitening<br>
//                                        //!<        2: PN9 whitening without byte reversal<br>
//                                        //!<        3: <i>Reserved</i><br>
//                                        //!<        4: No whitener, 32-bit IEEE 802.15.4g compatible CRC<br>
//                                        //!<        5: IEEE 802.15.4g compatible whitener and 32-bit CRC<br>
//                                        //!<        6: No whitener, dynamically IEEE 802.15.4g compatible 16-bit or 32-bit CRC<br>
//                                        //!<        7: Dynamically IEEE 802.15.4g compatible whitener and 16-bit or 32-bit CRC
//   } formatConf;
//   struct {
//      uint16_t frontEndMode:3;          //!< \brief 0x00: Differential mode<br>
//                                        //!<        0x01: Single-ended mode RFP<br>
//                                        //!<        0x02: Single-ended mode RFN<br>
//                                        //!<        0x05 Single-ended mode RFP with external frontend control on RF pins (RFN and RXTX)<br>
//                                        //!<        0x06 Single-ended mode RFN with external frontend control on RF pins (RFP and RXTX)<br>
//                                        //!<        Others: <i>Reserved</i>
//      uint16_t biasMode:1;              //!< \brief 0: Internal bias<br>
//                                        //!<        1: External bias
//      uint16_t analogCfgMode:6;         //!< \brief 0x00: Write analog configuration.<br>
//                                        //!<        Required first time after boot and when changing frequency band
//                                        //!<        or front-end configuration<br>
//                                        //!<        0x2D: Keep analog configuration.<br>
//                                        //!<        May be used after standby or when changing mode with the same frequency
//                                        //!<        band and front-end configuration<br>
//                                        //!<        Others: <i>Reserved</i>
//      uint16_t bNoFsPowerUp:1;          //!< \brief 0: Power up frequency synth<br>
//                                        //!<        1: Do not power up frequency synth
//   } config;                            //!<        Configuration options
//   uint16_t txPower;                    //!<        Transmit power
//   uint32_t* pRegOverride;              //!< \brief Pointer to a list of hardware and configuration registers to override. If NULL, no
//                                        //!<        override is used.
//   uint16_t centerFreq;                 //!< \brief Center frequency of the frequency band used, in MHz; used for calculating some internal TX and RX parameters.
//                                        //!<        For a single channel RF system, this should be set equal to the RF frequency used.
//                                        //!<        For a multi channel RF system (e.g. frequency hopping spread spectrum), this should be set equal
//                                        //!<        to the center frequency of the frequency band used.
//   int16_t intFreq;                     //!< \brief Intermediate frequency to use for RX, in MHz on 4.12 signed format. TX will use same
//                                        //!<        intermediate frequency if supported, otherwise 0.<br>
//                                        //!<        0x8000: Use default.
//   uint8_t loDivider;                   //!<        LO frequency divider setting to use. Supported values: 2 (CC1350 only), 5, 6, 10, 12, 15, and 30
//};

//typedef struct {
//    uint32_t            nInactivityTimeout;      ///< Inactivity timeout in microseconds.
//                                                 ///< The default value is 0xFFFFFFFF (infinite).

//    uint32_t            nPowerUpDuration;        ///< A custom power-up duration in microseconds.
//                                                 ///< If 0, the RF driver will start with a conservative value and measure the actual time during the first power-up.
//                                                 ///< The default value is 0.

//    RF_Callback         pPowerCb;                ///< \deprecated Power up callback, will be removed future versions, see RF_Params::pClienteventCb instead.
//                                                 ///< The default value is NULL.

//    RF_Callback         pErrCb;                  ///< \deprecated Callback function for driver error events.

//    uint16_t            nPowerUpDurationMargin;  ///< An additional safety margin to be added to #RF_Params::nPowerUpDuration.
//                                                 ///< This is necessary because of other hardware and software interrupts
//                                                 ///< preempting the RF driver interrupt handlers and state machine.
//                                                 ///< The default value is platform-dependent.

//    uint16_t            nPhySwitchingDurationMargin; ///< An additional safety margin to be used to calculate when conflicts shall be evaluated run-time.

//    RF_ClientCallback   pClientEventCb;          ///< Callback function for client-related events.
//                                                 ///< The default value is NULL.

//    RF_ClientEventMask  nClientEventMask;        ///< Event mask used to subscribe certain client events.
//                                                 ///< The purpose is to keep the number of callback executions small.
//} RF_Params;

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
    save_widgets(APPNAME);

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
    load_widgets(APPNAME);
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
#include "keeper.hpp"
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");

#if 1
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

#if 0
    QString res = load_string("test");
    if(res.isEmpty())
    {
        emit error("string is empty");
    }
    else
    {
        emit info(res);
    }
#endif

#if 0
    QList<QVariant> lv;
    Keeper::get_array("group", "name", &lv);
#endif

#if 0
    for(int n=0; n<10; n++)
    {
        ui->comboBox->addItem(QString("a%1").arg(n));
    }
#endif

#if 0
    Keeper::set_value("x", 123);
    Keeper::set_value("name", "Имя");
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");

#if 0
    QList<QVariant> lv;
    for(int n=0; n<ui->comboBox->count(); n++)
    {
        ui->comboBox->setCurrentIndex(n);
        lv.append(ui->comboBox->currentText());
    }
    Keeper::set_array("group", "name", lv);
#endif

#if 0
    QVariant x;
    QVariant name;
    Keeper::get_value("x", &x);
    Keeper::get_value("name", &name);
    emit info(QString("%1").arg(x.toInt()));
    emit info(QString("%1").arg(name.toString()));
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
    emit debug("load_setting !");
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
