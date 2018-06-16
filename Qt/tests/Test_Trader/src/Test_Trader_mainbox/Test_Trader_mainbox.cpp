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

#include <sys/sysinfo.h>

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
#include "ui_Test_Trader_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "Test_Trader_mainbox.hpp"
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
#if 0
EURUSD
GBPUSD
USDCHF
USDJPY
EURGBP
EURCHF
EURJPY
GBPCHF
GBPJPY
CHFJPY
USDCAD
EURCAD
AUDUSD
AUDJPY
NZDUSD
NZDJPY
XAUUSD
XAGUSD
USDCZK
USDDKK
EURRUB
USDHUF
USDNOK
USDPLN
USDRUB
USDSEK
USDSGD
USDZAR
USDHKD
USDMXN
USDTRY
EURHKD
EURMXN
EURTRY
EURAUD
EURNZD
EURSGD
EURZAR
XAUEUR
XAGEUR
GBPCAD
GBPAUD
GBPNZD
AUDCHF
AUDCAD
AUDNZD
NZDCHF
NZDCAD
CADCHF
CADJPY
BTCUSD
ETHUSD
LTCUSD
USDCNH
#endif
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    create_plot_currency();

    QVBoxLayout *vbox = new QVBoxLayout();
    QScrollArea *scroll = new QScrollArea(this);
    QMdiArea *area = new QMdiArea(this);

    area->show();
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
    hbox->setMargin(0);
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

//    Plot *plot_EURUSD = new Plot("EURUSD", this);
//    plot_EURUSD->setMode(1);
//    //plot_EURUSD->setMinimumSize(800, 600);

//    Plot *plot_USDRUB = new Plot("USDRUB", this);
//    plot_USDRUB->setMode(1);
//    //plot_USDRUB->setMinimumSize(800, 600);

//    Plot *plot_EURRUB = new Plot("EURRUB", this);
//    plot_EURRUB->setMode(1);
//    //plot_EURRUB->setMinimumSize(800, 600);

//    plot_tickets.append(plot_EURUSD);
//    plot_tickets.append(plot_USDRUB);
//    plot_tickets.append(plot_EURRUB);
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
    dlg->deleteLater();

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
    Plot *d_plot = new Plot("EURUSD");
    d_plot->setMode(1);
    d_plot->setMinimumSize(640, 480);
    d_plot->show();

    //plot_tickets.at(0)->test();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
