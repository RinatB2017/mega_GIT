/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_navi_transmitter_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "navi_transmitter_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
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
    save_widgets("Transmitter");

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    read_fake_data();

    timer = new QTimer(this);
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(update()));

    ui->sb_interval->setRange(0, 0xFFFF);
    connect(ui->btn_start,  SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));

    ui->serialWidget->set_caption("RS232");
    ui->serialWidget->add_menu(2);
    ui->serialWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    connect(ui->btn_GCA,    SIGNAL(clicked(bool)),  this,   SLOT(test_GGA()));
    connect(ui->btn_GSA,    SIGNAL(clicked(bool)),  this,   SLOT(test_GSA()));
    connect(ui->btn_GSV,    SIGNAL(clicked(bool)),  this,   SLOT(test_GSV()));
    connect(ui->btn_RMC,    SIGNAL(clicked(bool)),  this,   SLOT(test_RMC()));
    connect(ui->btn_VTG,    SIGNAL(clicked(bool)),  this,   SLOT(test_VTG()));
    connect(ui->btn_GLL,    SIGNAL(clicked(bool)),  this,   SLOT(test_GLL()));
    connect(ui->btn_ZDA,    SIGNAL(clicked(bool)),  this,   SLOT(test_ZDA()));

    connect(ui->btn_PIREA,  SIGNAL(clicked(bool)),  this,   SLOT(test_PIREA()));
    connect(ui->btn_PIRFV,  SIGNAL(clicked(bool)),  this,   SLOT(test_PIRFV()));
    connect(ui->btn_PIRGK,  SIGNAL(clicked(bool)),  this,   SLOT(test_PIRGK()));
    connect(ui->btn_PIRRA,  SIGNAL(clicked(bool)),  this,   SLOT(test_PIRRA()));

    connect(this,               SIGNAL(send(QByteArray)),   ui->serialWidget,   SLOT(input(QByteArray)));
    connect(ui->serialWidget,   SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));

    load_widgets("Transmitter");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit error(tr("test"));
}
//--------------------------------------------------------------------------------
void MainBox::read_fake_data(void)
{
    QFile file(":/test_data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    fake_data.clear();
    index_fake_data = 0;

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        fake_data.append(line);
    }
    max_index_fake_data = fake_data.count();
    emit info(QString("Found %1 record").arg(max_index_fake_data));
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray)
{

}
//--------------------------------------------------------------------------------
void MainBox::start(void)
{
    if(timer)
    {
        timer->start(ui->sb_interval->value());
    }
}
//--------------------------------------------------------------------------------
void MainBox::stop(void)
{
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
    if(ui->serialWidget->isOpen() == false)
    {
        emit error("serial not open!");
        timer->stop();
        return;
    }

    emit send(fake_data.at(index_fake_data));
    ui->le_current_record->setText(QString("%1").arg(index_fake_data));

    index_fake_data++;
    if(index_fake_data > max_index_fake_data)
    {
        index_fake_data = 0;
    }
}
//--------------------------------------------------------------------------------
QString MainBox::get_checksum(const QString &data)
{
    QString temp;
    uchar sum;
    int n;

    if(data.length()<3)
    {
        emit error(QString("len = %1").arg(data.length()));
        return "";
    }
    temp = data.mid(0, data.length());

    sum = 0;
    for(n=0; n<temp.length(); n++)
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        sum ^= temp.at(n).toAscii();
#else
        sum ^= temp.at(n).toLatin1();
#endif

    return QString("%1").arg(sum, 2, 16, QLatin1Char('0')).toUpper();
}
//--------------------------------------------------------------------------------
void MainBox::test_GGA(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("GL");
    message.append("GGA,");
    message.append("123456.12,");  // время обсервации UTC (часы, минуты, целая и дробная часть секунд)
    message.append("1234.1234,");  // широта  ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("N,");          // север/юг
    message.append("12345.1234,"); // долгота ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("E,");          // восток/запад
    message.append("0,");          // показатель качества обсервации 0-1-2
    message.append("12,");         // число НКА в решении
    message.append("0.1,");        // величина горизонтального геометрического фактора
    message.append("2.3,");        // высота над средним уровнем моря
    message.append("M,");          // единица измерения высоты - метры
    message.append("0.1,");        // превышение геоида на эллипсоидом WGS-84
    message.append("M,");          // единица измерения - метры
    message.append("0.1,");        // возраст дифференциальных поправок (время в секундах)
    message.append("1234");        // идентификатор дифферинциальной станции 0000-1024

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_GSA(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("GL");
    message.append("GSA,");
    message.append("A,");      // тип управления   A-M
    message.append("2,");      // режим работы     1-2-3
    message.append("1,2,3,4,5,6,7,8,9,10,11,12,"); // номера НКА в решении
    message.append("1.2,");    // пространственный геометрический фактор ухудшения точности (PDOP)
    message.append("3.4,");    // геометрический фактор ухудшения точности в плане (HDOP)
    message.append("5.6");     // геометрический фактор ухудшения точности в плане (VDOP)

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_GSV(void)
{
    emit error("test_GSV: пока не сделано");
}
//--------------------------------------------------------------------------------
void MainBox::test_RMC(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("GL");
    message.append("RMC,");
    message.append("112233.44,");  // время UTC (часы, минуты, целая и дробная часть секунд)
    message.append("A,");          // статус     A-D-V
    message.append("1234.1234,");  // широта  ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("N,");          // север/юг
    message.append("12345.1234,"); // долгота ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("E,");          // восток/запад
    message.append("1.2,");        // наземная скорость, в узлах
    message.append("3.4,");        // наземный курс, в градусах
    message.append("01022012,");   // дата день-месяц-год
    message.append("5.6,");        // магнитное склонение в градусах
    message.append("E,");          // восток/запад
    message.append("A");           // режим местоопределения     A-D-E-M-S-N

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_VTG(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("GL");
    message.append("VTG,");
    message.append("1.2,");    // наземный курс в градусах
    message.append("T,");      // истинный T
    message.append("3.4,");    // наземный курс в градусах
    message.append("M,");      // магнитный M
    message.append("5.6,");    // наземная скорость
    message.append("Т,");      // в узлах
    message.append("7.8,");    // наземная скорость
    message.append("K,");      // в км/ч
    message.append("A");       // режим местоопределения   A-D-E-M-S-N

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_GLL(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("GL");
    message.append("GLL,");
    message.append("1234.1234,");  // широта  ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("N,");          // север/юг
    message.append("12345.1234,"); // долгота ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("E,");          // восток/запад
    message.append("123456.12,");  // время обсервации UTC (часы, минуты, целая и дробная часть секунд)
    message.append("A,");          // статус A-D-V
    message.append("A");           // режим местоопределения     A-D-E-M-S-N

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_ZDA(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("GL");
    message.append("ZDA,");
    message.append("123456.12,");  // время обсервации UTC (часы, минуты, целая и дробная часть секунд)
    message.append("01,");         // день UTC  (от 01 до 31)
    message.append("02,");         // месяц UTC (от 01 до 12)
    message.append("2012,");       // год UTC
    message.append("01,");         // часы локальной временной зоны (от 00 до +- 13)
    message.append("02");          // минуты локальной временной зоны (от 00 до 59)

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_PIREA(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("PIREA,");
    message.append("5");  // результат теста (код ошибки)

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_PIRFV(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("PIRFV,");
    message.append("12.34");  // номер версии встроенного ПО приемника

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_PIRGK(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("PIRGK,");
    message.append("123456.12,");      // время UTC (часы, минуты, целая и дробная часть секунд)
    message.append("1,");              // индикатор качества GNSS 0-1
    message.append("1.2,");            // координата X, м
    message.append("500000.1000000,"); // координата Y, м
    message.append("3.4,");            // высота, м
    message.append("5.6,");            // скорость, м/с
    message.append("7.8,");            // курс, в градусах
    message.append("01022012,");       // дата день-месяц-год
    message.append("3.4,");            // геометрический фактор ухудшения точности в плане (HDOP)
    message.append("5.6,");            // геометрический фактор ухудшения точности в плане (VDOP)
    message.append("7");               // количество НКА в решении

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_PIRRA(void)
{
    QByteArray data;
    QString message;
    QString checksum;

    message.clear();
    message.append("PIRRA,");
    message.append("1,");
    message.append("2,");
    message.append("3,");
    message.append("4,");
    message.append("5");

    checksum = get_checksum(message);

    data.clear();
    data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    emit send(data);
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
