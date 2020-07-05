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
#ifdef Q_OS_LINUX
#   include <sys/sysinfo.h>
#endif
#include <qmath.h>
//--------------------------------------------------------------------------------
#include "ui_test_candlestick_box_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "csvreader.hpp"
#include "test_candlestick_box_mainbox.hpp"

#include "candlestick_box.hpp"
#include "qcandlestickset.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    parent(parent)
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
QVector<QPointF> MainBox::circle(qreal x, qreal y, qreal r)
{
    QVector<QPointF> vector1;
    for (qreal i = 0; i <= 3.14/2; i += 0.1)
    {
        double sin = qSin(i);
        double cos = qCos(i);
        vector1.append(QPointF(x + r*cos, y + r*sin));
        vector1.append(QPointF(x + r*cos, y - r*sin));
        vector1.append(QPointF(x - r*cos, y + r*sin));
        vector1.append(QPointF(x - r*cos, y - r*sin));
    }
    return vector1;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    init_process();
    load_widgets();

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        mw->add_dock_widget("CandleStickBox", "candlestickbox", Qt::LeftDockWidgetArea, ui->CandleStick_widget);
        setVisible(false);
    }
}
//--------------------------------------------------------------------------------
void MainBox::init_process(void)
{
    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::SeparateChannels);
    process->setReadChannel(QProcess::StandardOutput);

    connect(process, SIGNAL(started()),                 this, SLOT(started()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(read_data()));
    connect(process, SIGNAL(readyReadStandardError()),  this, SLOT(read_error()));
    connect(process, SIGNAL(finished(int)),             this, SLOT(finished(int)));
    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(process_error(QProcess::ProcessError)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_load = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_DialogOpenButton),
                                       "load",
                                       "load");
    QToolButton *btn_save = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_DialogSaveButton),
                                       "save",
                                       "save");
    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       QIcon(),
                                       "test",
                                       "test");
    QToolButton *btn_test2 = add_button(testbar,
                                        new QToolButton(this),
                                        QIcon(),
                                        "test2",
                                        "test2");
    QToolButton *btn_test3 = add_button(testbar,
                                        new QToolButton(this),
                                        QIcon(),
                                        "test3",
                                        "test3");

    connect(btn_load,   &QToolButton::clicked,  this,   &MainBox::load);
    connect(btn_save,   &QToolButton::clicked,  this,   &MainBox::save);
    connect(btn_test,   &QToolButton::clicked,  this,   &MainBox::test);
    connect(btn_test2,  &QToolButton::clicked,  this,   &MainBox::test2);
    connect(btn_test3,  &QToolButton::clicked,  this,   &MainBox::test3);
}
//--------------------------------------------------------------------------------
bool MainBox::create_set(QStringList sl,
                         QCandlestickSet *set)
{
    if(sl.size() != 7)
    {
        emit error(QString("bad len %1").arg(sl.size()));
        return false;
    }
    QString s_ticket = sl.at(0);
    QString s_date   = sl.at(1);
    QString s_time   = sl.at(2);
    QString s_open   = sl.at(3);
    QString s_high   = sl.at(4);
    QString s_low    = sl.at(5);
    QString s_close  = sl.at(6);

    bool ok = false;
    //---
    int year = 0;
    int month = 0;
    int day = 0;
    if(s_date.length() == 8)
    {
        year  = s_date.left(4).toInt(&ok);
        month = s_date.mid(4, 2).toInt(&ok);
        day   = s_date.right(2).toInt(&ok);
    }

    QDateTime dt;
    QDate date(year, month, day);
    dt.setDate(date);
    QDateTime dt2 = dt.addSecs(s_time.toInt());
    //---

    qreal d_open  = s_open.toDouble(&ok);
    qreal d_high  = s_high.toDouble(&ok);
    qreal d_low   = s_low.toDouble(&ok);
    qreal d_close = s_close.toDouble(&ok);

    if(set)
    {
        set->setTimestamp(dt2.toTime_t());
        set->setOpen(d_open);
        set->setHigh(d_high);
        set->setLow(d_low);
        set->setClose(d_close);
    }

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load(void)
{
#if 0
    //QString filename = "011109.txt";
    QString filename = "140618.txt";
#else
    QFileDialog *dlg;
    QString filename;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptOpen);
    dlg->setNameFilter(tr("TXT files (*.txt)"));
    dlg->setDefaultSuffix(tr("txt"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    dlg->deleteLater();

    if(filename.isEmpty())
    {
        emit error("Файл не выбран!");
        return;
    }
    //emit info(filename);
#endif

    QStringList t_sl;
    CsvReader *csv = new CsvReader(nullptr, filename);
    csv->set_new_separator(',');
    if(csv->Open())
    {
        QList<QStringList> str = csv->CSVRead();
        int cnt = 0;
        foreach (QStringList sl, str)
        {
            //<TICKER>,<DTYYYYMMDD>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>
            bool ok = false;
            QString ticket_name = sl.at(0);
            //---
            foreach (QString t_name, t_sl)
            {
                if(t_name == ticket_name)
                {
                    ok = true;
                }
            }
            if(ok == false)
            {
                //emit info(ticket_name);
                t_sl.append(ticket_name);
            }
            //---

//            foreach (CandleStick_Box *plot_ticket, plot_tickets)
//            {
//                if(plot_ticket->get_ticket_name() == ticket_name)
//                {
//                    QCandlestickSet *set = new QCandlestickSet;
//                    ok = create_set(sl, set);
//                    if(ok)
//                    {
//                        plot_ticket->append(set);
//                        plot_ticket->update_data();
//                    }
//                }
//            }
        }
        foreach (QString t_name, t_sl)
        {
            emit info(t_name);
        }
        emit info(QString("cnt = %1").arg(cnt));
        emit info("OK");
    }
    else
    {
        emit error(QString("File %1 not open").arg(filename));
    }
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    emit info("save");
}
//--------------------------------------------------------------------------------
void MainBox::print_set(QCandlestickSet *set)
{
    emit info(QString("%1\t%2\t%3\t%4\t%5")
              .arg(set->open())
              .arg(set->high())
              .arg(set->low())
              .arg(set->close())
              .arg((quint64)set->timestamp()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    ui->CandleStick_widget->set_ticket_name("TEST");

#if 0
    QRandomGenerator generator;
    int delta = 0;
    for(int n=0; n<100; n++)
    {
        QCandlestickSet *set = new QCandlestickSet();
        set->setTimestamp(1435708800000 + delta);
        //set->setTimestamp(n * 24 * 3600);
        set->setOpen(generator() % 1000);
        set->setClose(generator() % 1000);
        set->setHigh(generator() % 1000);
        set->setLow(generator() % 1000);
        ui->CandleStick_widget->append(set);

        delta += 100000;
    }
#else
    QCandlestickSet *set1 = new QCandlestickSet();
    set1->setOpen(0.0202550100000000);
    set1->setHigh(0.0205355100000000);
    set1->setLow(0.0200902600000000);
    set1->setClose(0.0202730700000000);
    //volume":"3944.0084858500000000
    set1->setTimestamp(1572393600000);
    print_set(set1);
    ui->CandleStick_widget->append(set1);

    QCandlestickSet *set2 = new QCandlestickSet();
    set2->setOpen(0.0202350100000000);
    set2->setHigh(0.0202876700000000);
    set2->setLow(0.0198971300000000);
    set2->setClose(0.0200359800000000);
    //volume":"4332.6834116200000000
    set2->setTimestamp(1572422400000);
    print_set(set2);
    ui->CandleStick_widget->append(set2);

    QCandlestickSet *set3 = new QCandlestickSet();
    set3->setOpen(0.0200417600000000);
    set3->setHigh(0.0200871100000000);
    set3->setLow(0.0198633900000000);
    set3->setClose(0.0199950000000000);
    //volume":"1326.6261286100000000;
    set3->setTimestamp(1572451200000);
    print_set(set3);
    ui->CandleStick_widget->append(set3);

    QCandlestickSet *set4 = new QCandlestickSet();
    set4->setOpen(0.0200088700000000);
    set4->setHigh(0.0201535900000000);
    set4->setLow(0.0199500000000000);
    set4->setClose(0.0200250000000000);
    //volume":"1633.7763925100000000
    set4->setTimestamp(1572480000000);
    print_set(set4);
    ui->CandleStick_widget->append(set4);

    QCandlestickSet *set5 = new QCandlestickSet();
    set5->setOpen(0.0200149700000000);
    set5->setHigh(0.0200248800000000);
    set5->setLow(0.0195836700000000);
    set5->setClose(0.0197245300000000);
    //volume":"3544.8681055200000000
    set5->setTimestamp(1572508800000);
    print_set(set5);
    ui->CandleStick_widget->append(set5);

    QCandlestickSet *set6 = new QCandlestickSet();
    set6->setOpen(0.0197050400000000);
    set6->setHigh(0.0200310600000000);
    set6->setLow(0.0196817400000000);
    set6->setClose(0.0199250000000000);
    //volume":"4773.6334983000000000
    set6->setTimestamp(1572537600000);
    print_set(set6);
    ui->CandleStick_widget->append(set6);

    QCandlestickSet *set7 = new QCandlestickSet();
    set7->setOpen(0.0199200100000000);
    set7->setHigh(0.0200208200000000);
    set7->setLow(0.0198200000000000);
    set7->setClose(0.0198200000000000);
    //volume":"2491.0707443400000000
    set7->setTimestamp(1572566400000);
    print_set(set7);
    ui->CandleStick_widget->append(set7);

    QCandlestickSet *set8 = new QCandlestickSet();
    set8->setOpen(0.0198349900000000);
    set8->setHigh(0.0199600000000000);
    set8->setLow(0.0195860200000000);
    set8->setClose(0.0197458000000000);
    //volume":"3124.0585905100000000
    set8->setTimestamp(1572595200000);
    print_set(set8);
    ui->CandleStick_widget->append(set8);

    QCandlestickSet *set9 = new QCandlestickSet();
    set9->setOpen(0.0197458000000000);
    set9->setHigh(0.0200600000000000);
    set9->setLow(0.0195363000000000);
    set9->setClose(0.0198050000000000);
    //volume":"1382.2579215800000000
    set9->setTimestamp(1572624000000);
    print_set(set9);
    ui->CandleStick_widget->append(set9);

    QCandlestickSet *set10 = new QCandlestickSet();
    set10->setOpen(0.0198199700000000);
    set10->setHigh(0.0198550000000000);
    set10->setLow(0.0197350000000000);
    set10->setClose(0.0198100000000000);
    //volume":"487.7146311400000000
    set10->setTimestamp(1572652800000);
    print_set(set10);
    ui->CandleStick_widget->append(set10);
#endif
    ui->CandleStick_widget->update_data();
    ui->CandleStick_widget->show();
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    QStringList args;
    args << "-rc";
    args << "-l1";
    args << "-np";
    args << "-nH";
    args << "--cut-dirs=1";
    args << "-A";
    args << "zip";
    args << "-P";
    args << "quotes";
    args << "http://www.forexite.com/free_forex_quotes/forex_history_arhiv.html";

    QString temp;
    foreach (QString str, args)
    {
        temp.append(str);
        temp.append(" ");
    }
    emit debug(temp);

    process->start("wget", args);
}
//--------------------------------------------------------------------------------
void MainBox::test3(void)
{
    ui->CandleStick_widget->test2();
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Starting...");
}
//--------------------------------------------------------------------------------
void MainBox::read_data(void)
{
    Q_CHECK_PTR(process);
    QByteArray data = process->readAllStandardOutput();
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::read_error(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::finished(int state)
{
    emit trace(Q_FUNC_INFO);
    emit info(QString("Finished: %1").arg(state));
}
//--------------------------------------------------------------------------------
void MainBox::process_error(QProcess::ProcessError err)
{
    emit trace(Q_FUNC_INFO);
    switch(err)
    {
    case QProcess::FailedToStart:    emit error("FailedToStart");    break;
    case QProcess::Crashed:          emit error("Crashed");          break;
    case QProcess::Timedout:         emit error("Timedout");         break;
    case QProcess::ReadError:        emit error("ReadError");        break;
    case QProcess::WriteError:       emit error("WriteError");       break;
    case QProcess::UnknownError:     emit error("UnknownError");     break;
    }
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

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
