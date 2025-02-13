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

#include <qwt_plot_seriesitem.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_plot.h>
#include <qmath.h>

#include "traderplot.hpp"
#include "csvreader.hpp"
#include "plot.h"
//--------------------------------------------------------------------------------
#include "ui_test_trader_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_trader_mainbox.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
QVector<QPointF> MainBox::circle(float x, float y, float r)
{
    QVector<QPointF> vector1;
    for (float i = 0; i <= 3.14/2; i += 0.1)
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
    create_plot_currency();

    QVBoxLayout *vbox = new QVBoxLayout();
    QScrollArea *scroll = new QScrollArea(this);
    QMdiArea *area = new QMdiArea(this);

    //area->show();
    foreach (Plot *plot, plot_tickets)
    {
        area->addSubWindow(plot);
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    hbox->setMargin(0);
#endif
    hbox->addWidget(frame);
    setLayout(hbox);
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
        Plot *plot = new Plot(currency, this);
        plot->setMode(1);
        plot_tickets.append(plot);
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

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
void MainBox::load(void)
{
#if 0
    //QString filename = "011109.txt";
    QString filename = "140618.txt";
#else
    QFileDialog *dlg = 0;
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
    delete dlg;

    if(filename.isEmpty())
    {
        emit error("Файл не выбран!");
        return;
    }
    //emit info(filename);
#endif

    QStringList t_sl;
    CsvReader *csv = new CsvReader(0, filename);
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

            foreach (Plot *plot_ticket, plot_tickets)
            {
                if(plot_ticket->get_ticket_name() == ticket_name)
                {
                    //int data = sl.at(1).toInt(&ok);
                    //if(!ok) data = 0;
                    //emit info(QString("%1").arg(data));

                    int time = sl.at(2).toInt(&ok);
                    if(!ok) time = 0;
                    float open = sl.at(3).toFloat(&ok);
                    if(!ok) open = 0;
                    float high = sl.at(4).toFloat(&ok);
                    if(!ok) high = 0;
                    float low = sl.at(5).toFloat(&ok);
                    if(!ok) low = 0;
                    float close = sl.at(6).toFloat(&ok);
                    if(!ok) close = 0;

                    if(time > 0)
                    {
#if 0
                        emit info(QString("ticket: %1 %2 %3 %4 %5")
                                  .arg(time)
                                  .arg(open)
                                  .arg(close)
                                  .arg(low)
                                  .arg(high));
#endif
                        plot_ticket->append(time, open, close, low, high);
                        cnt++;
                        plot_ticket->update_ticket();
                        plot_ticket->test();
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
void MainBox::test(void)
{
#if 1
    Plot *d_plot = new Plot("EURUSD");
    d_plot->setMode(1);
    d_plot->setMinimumSize(640, 480);
    d_plot->show();

    plot_tickets.at(0)->test();
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
    emit trace(Q_FUNC_INFO);
    emit info("Starting...");
}
//--------------------------------------------------------------------------------
void MainBox::read_data(void)
{
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
