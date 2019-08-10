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

    createTestBar();

#if 1
    create_test_currency();
#else
    create_plot_currency();
#endif

    QVBoxLayout *vbox = new QVBoxLayout();
    QScrollArea *scroll = new QScrollArea(this);
    QMdiArea *area = new QMdiArea(this);

    //area->show();
    foreach (CandleStick_Box *box, plot_tickets)
    {
        area->addSubWindow(box);
    }
    //area->setViewMode(QMdiArea::SubWindowView);
    area->setViewMode(QMdiArea::TabbedView);
    emit debug(QString("area: %1 %2").arg(area->width()).arg(area->height()));

    scroll->setWidgetResizable(true);
    scroll->setWidget(area);
    vbox->addWidget(scroll);

    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    frame->setLayout(vbox);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->addWidget(frame);
    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void MainBox::create_test_currency(void)
{
    CandleStick_Box *box = new CandleStick_Box("EURUSD", this);
#if 0
    for(int n=0; n<16; n++)
    {
        QCandlestickSet *set = new QCandlestickSet;
        set->setTimestamp(1527802700 * n);
        //set->setTimestamp(n * 24 * 3600);
        set->setOpen(qrand() % 1000);
        set->setClose(qrand() % 1000);
        set->setHigh(qrand() % 1000);
        set->setLow(qrand() % 1000);
        box->append(set);
    }
    box->update_data();
#endif
    plot_tickets.append(box);
}
//--------------------------------------------------------------------------------
void MainBox::create_plot_currency(void)
{
    QStringList sl_currency;
    sl_currency << "EURUSD"
                   //                << "EURGBP"
                   //                << "EURCHF"
                   //                << "EURJPY"
                << "EURRUB"
                   //                << "EURHKD"
                   //                << "EURMXN"
                   //                << "EURTRY"
                   //                << "EURAUD"
                   //                << "EURNZD"
                   //                << "EURSGD"
                   //                << "EURZAR"
                   //                << "EURCAD"

                   //                << "USDCHF"
                   //                << "USDJPY"
                   //                << "USDCAD"
                   //                << "USDDKK"
                   //                << "USDCZK"
                   //                << "USDNOK"
                   //                << "USDPLN"
                << "USDRUB"
                   //                << "USDSEK"
                   //                << "USDSGD"
                   //                << "USDZAR"
                   //                << "USDHKD"
                   //                << "USDMXN"
                   //                << "USDTRY"
                   //                << "USDHUF"
                   //                << "USDCNH"

                   //                << "GBPUSD"
                   //                << "GBPCHF"
                   //                << "GBPJPY"
                   //                << "GBPCAD"
                   //                << "GBPAUD"
                   //                << "GBPNZD"

                   //                << "AUDUSD"
                   //                << "AUDJPY"
                   //                << "AUDCHF"
                   //                << "AUDCAD"
                   //                << "AUDNZD"

                   //                << "NZDUSD"
                   //                << "NZDJPY"
                   //                << "NZDCHF"
                   //                << "NZDCAD"

                   //                << "XAUUSD"
                   //                << "XAGUSD"
                   //                << "XAUEUR"
                   //                << "XAGEUR"

                   //                << "CADCHF"
                   //                << "CADJPY"

                   //                << "CHFJPY"
                   //                << "BTCUSD"
                   //                << "ETHUSD"
                   //                << "LTCUSD"
                   ;

    foreach (QString currency, sl_currency)
    {
        CandleStick_Box *box = new CandleStick_Box(currency, this);
        plot_tickets.append(box);
    }
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
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_load, SIGNAL(clicked()), this, SLOT(load()));
    connect(btn_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
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

            foreach (CandleStick_Box *plot_ticket, plot_tickets)
            {
                if(plot_ticket->get_ticket_name() == ticket_name)
                {
                    QCandlestickSet *set = new QCandlestickSet;
                    ok = create_set(sl, set);
                    if(ok)
                    {
                        plot_ticket->append(set);
                        plot_ticket->update_data();
                    }
                }
            }
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
#include "candlestick_box.hpp"
#include "qcandlestickset.h"
void MainBox::test(void)
{
#if 0
    CandleStick_Box *box = new CandleStick_Box("USDEUR");
    box->setMinimumSize(800, 600);
    box->show();

    for(int n=0; n<10; n++)
    {
        QCandlestickSet *set = new QCandlestickSet;
        set->setTimestamp(1435708800000 * n);
        //set->setTimestamp(n * 24 * 3600);
        set->setOpen(qrand() % 1000);
        set->setClose(qrand() % 1000);
        set->setHigh(qrand() % 1000);
        set->setLow(qrand() % 1000);
        box->append(set);
    }
    box->update_data();
#endif

#if 0
    QProcess *proccess = new QProcess(this);
    proccess->setProcessChannelMode(QProcess::SeparateChannels);
    //proccess->setReadChannel(QProcess::StandardOutput);

    connect(proccess, SIGNAL(started()),                 this, SLOT(started()));
    connect(proccess, SIGNAL(readyReadStandardOutput()), this, SLOT(read_data()));
    connect(proccess, SIGNAL(readyReadStandardError()),  this, SLOT(read_error()));
    connect(proccess, SIGNAL(finished(int)),             this, SLOT(finished(int)));
    connect(proccess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(process_error(QProcess::ProcessError)));

    proccess->start("wget", QStringList()
                    << "-rc"
                    << "-l1"
                    << "-np"
                    << "-nH"
                    << "--cut-dirs=1"
                    << "-A"
                    << "zip"
                    << "-P"
                    << "quotes"
                    << "http://www.forexite.com/free_forex_quotes/forex_history_arhiv.html");
#endif
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    //emit trace(Q_FUNC_INFO);
    emit info("Starting...");
}
//--------------------------------------------------------------------------------
void MainBox::read_data(void)
{
    //emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::read_error(void)
{
    //emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::finished(int state)
{
    //emit trace(Q_FUNC_INFO);
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
