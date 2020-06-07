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
    ui->setupUi(this);

#ifdef QT_DEBUG
    init();
#else
    QTimer::singleShot(0, [this]{
        init();
    });
#endif
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
    createTestBar();

    ui->horizontalSlider->setRange(0, 1000);

    connect(ui->sb_1,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &MainBox::check_in);
    connect(ui->sb_2,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &MainBox::check_in);
    connect(ui->sb_res, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &MainBox::check_in);
    connect(ui->btn_ok, &QPushButton::clicked,  this,   &MainBox::victory);

    check_in();

    ui->toolButton->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon)));
    connect(ui->toolButton, &QToolButton::clicked,  this,   &MainBox::delete_string);

#if 0
    sw.reset(new SimpleWidget);
    sw->setProperty("windowTitle", "XXX");
    sw->show();
#endif

    //---
    connect(ui->btn_click,  &QPushButton::clicked,  [this]() {
        emit info("click");
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
    commands.append({ id++, "print property",   &MainBox::print_property });
    commands.append({ id++, "timer start",      &MainBox::timer_start });
    commands.append({ id++, "timer stop",       &MainBox::timer_stop });

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

    connect(btn_choice_test,    &QToolButton::clicked,  this,   &MainBox::choice_test);

    connect(cb_block,   &QCheckBox::clicked,    cb_test,            &QComboBox::setDisabled);
    connect(cb_block,   &QCheckBox::clicked,    btn_choice_test,    &QComboBox::setDisabled);

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
bool MainBox::load_property(QWidget *widget, const QString &property_name)
{
    Q_CHECK_PTR(widget);
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
    Q_CHECK_PTR(widget);
    return widget->setProperty(property_name.toLocal8Bit(), value);
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
void MainBox::readJson(const QString &filename)
{
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonParseError jerror;
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8(), &jerror);
    if(jerror.error != QJsonParseError::NoError)
    {
        emit error(jerror.errorString());
        return;
    }

    emit info(QString("%1").arg(d[0].toObject().isEmpty() ? "empty" : "NO empty"));
    emit info(QString("%1").arg(d[1].toObject().isEmpty() ? "empty" : "NO empty"));
    emit info(QString("%1").arg(d[2].toObject().isEmpty() ? "empty" : "NO empty"));

    QJsonObject obj;
    int index = 0;
    do {
        obj = d[index++].toObject();
        if(obj.isEmpty() == false)
        {
            emit info(QString("%1 %2")
                      .arg(obj.value("symbol").toString())
                      .arg(obj.value("askPrice").toString()));
        }
    } while(obj.isEmpty() == false);
    emit error(QString("index %1").arg(index));
}
//--------------------------------------------------------------------------------
void MainBox::f1(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::f2(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::f3(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::f4(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::f5(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
#include "croppicture.hpp"

bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");

#if 1
    QToolButton *btn = new QToolButton(ui->te_test);
    btn->show();
#endif

#if 0
    MyTextEdit *mte = new MyTextEdit();
    mte->show();
#endif

#if 0
    typedef struct
    {
        QString btn_text;
        QString file_str;
        void (MainBox::*func)(void);
    } BTN;
    QList<BTN> buttons;

    buttons.clear();
    buttons.append( { "F1", ICON_PROGRAMM, &MainBox::f1} );
    buttons.append( { "F2", ICON_PROGRAMM, &MainBox::f2} );
    buttons.append( { "F3", ICON_PROGRAMM, &MainBox::f3} );
    buttons.append( { "F4", ICON_PROGRAMM, &MainBox::f4} );
    buttons.append( { "F5", ICON_PROGRAMM, &MainBox::f5} );

    QWidget *widget = new QWidget();
    QGridLayout *grid = new QGridLayout();

    int row = 0;
    foreach (BTN btn, buttons)
    {
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap(btn.file_str));

        QPushButton *button = new QPushButton();
        button->setText(btn.btn_text);
        connect(button, &QPushButton::clicked,  this,   btn.func);

        grid->addWidget(label,  row,    0);
        grid->addWidget(button, row,    1);
        row++;
    }

    widget->setLayout(grid);
    widget->show();
#endif

#if 0
    MyLabel *pict = new MyLabel();
    connect_log_signals(pict, this);

    pict->setPixmap(QPixmap(":/s1.png"));
    pict->show();
#endif

#if 0
    CropPicture *pict = new CropPicture();
    connect_log_signals(pict, this);

    pict->load_pixmap(QPixmap(":/s1.png"));
    pict->show();
#endif

#if 0
    QPixmap p(ICON_PROGRAMM);
    int mul = 40;
    int w = p.width()  * mul;
    int h = p.height() * mul;

    QLabel *label = new QLabel();

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    label->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));

//    label->setPixmap(QPixmap(ICON_PROGRAMM));
//    label->setScaledContents(true);
    label->show();
#endif

#if 0
    QStringList sl;
    sl << " "
       << "."
       << ","
       << "-"
       << "\r"
       << "\n"
       << "\t"
       << "*"
       << "\\"
       << "("
       << ")"
       << ":"
       << "0"
       << "1"
       << "2"
       << "3"
       << "4"
       << "5"
       << "6"
       << "7"
       << "8"
       << "9";

    QString temp = ui->te_test->toPlainText();
    foreach (QString s, sl)
    {
        temp = temp.remove(s);
    }

    emit info(temp);
    emit info(QString("Size %1").arg(temp.length()));
#endif

#if 0
    QString temp = ui->te_test->property("markdown").toString();
    emit info(temp);
#endif

#if 0
    QPoint pos(10,10);

    QMouseEvent *me;
    me = new QMouseEvent(QEvent::MouseButtonPress  , pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::postEvent(ui->btn_click, me);

    me = new QMouseEvent(QEvent::MouseButtonRelease, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::postEvent(ui->btn_click, me);
#endif

#if 0
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << qint32(1000000);
    emit info(data.toHex());
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
bool MainBox::print_property(void)
{
    for(int index = 0; index < sw->metaObject()->propertyCount(); index++)
    {
        emit info(QString("%1").arg(sw->metaObject()->property(index).name()));
    }
    return true;
}
//--------------------------------------------------------------------------------
int MainBox::test_packet(PACKET packet)
{
    if(packet.a < 10)                       return 0;
    if((packet.b > 10) && (packet.b < 20))  return 0;
    if((packet.c > 20) && (packet.c < 30))  return 0;
    return 1;
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
