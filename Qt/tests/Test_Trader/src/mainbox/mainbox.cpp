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
#include <QVBoxLayout>
#include <QToolButton>
#include <QStringList>
#include <QComboBox>
#include <QToolBar>
#include <QVector>
#include <QPointF>

#include <QScrollArea>
#include <QMdiArea>

#include <QTextStream>
#include <QProcess>
#include <QVector>
#include <sys/sysinfo.h>

#include <QDebug>

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
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"

#include "logbox.hpp"

#include "grapherbox.hpp"
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
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

#if 0
    QStringList sl;
    sl.clear();

    sl << "EURUSD"
       << "EURGBP"
       << "EURCHF"
       << "EURJPY"
       << "EURCAD"

       << "USDCAD"
       << "USDCHF"
       << "USDJPY"

       << "GPBUSD"
       << "GBPCHF"
       << "GBPJPY"

       << "AUDCAD"
       << "AUDJPY"

       << "NZDUSD"
       << "NZDJPY"

       << "CHFJPY"
       << "XAGUSD";

    foreach(QString ticket, sl)
    {
        Plot *temp_plot = new Plot(ticket, this);
        temp_plot->setMode(1);
        plot_tickets.append(temp_plot);
    }
#else
    Plot *temp_plot = new Plot("EURUSD", this);
    temp_plot->setMode(1);
    temp_plot->setMinimumSize(800, 600);
    plot_tickets.append(temp_plot);
#endif

    CsvReader *csv = new CsvReader(0, "011109.txt");
    csv->set_new_separator(',');
    if(csv->Open())
    {
        QList<QStringList> str = csv->CSVRead();
        //int time = 0;
        foreach (QStringList sl, str)
        {
            //<TICKER>,<DTYYYYMMDD>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>
            bool ok;
            QString ticket_name = sl.at(0);
            foreach (Plot *plot_ticket, plot_tickets)
            {
                if(plot_ticket->get_ticket_name() == ticket_name)
                {
                    //int data = sl.at(1).toInt(&ok);
                    //if(!ok) data = 0;
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

                    plot_ticket->append(time, open, close, low, high);
                }
                plot_ticket->update_ticket();
            }
        }
    }
    else
    {
        qDebug() << "file 011109.txt not open";
    }
    index = 0;

    QVBoxLayout *vbox = new QVBoxLayout();
    QScrollArea *scroll = new QScrollArea(this);
    QMdiArea *area = new QMdiArea(this);
    //area->cascadeSubWindows();
    foreach (Plot *plot, plot_tickets)
    {
        area->addSubWindow(plot);
    }
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
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw ==nullptr)
    {
        return;
    }

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
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    Plot *d_plot = new Plot("EURUSD");
    d_plot->setMinimumSize(640, 480);
    d_plot->setMode(1);
    d_plot->show();
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
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
