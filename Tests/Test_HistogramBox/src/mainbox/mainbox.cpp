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
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include <QComboBox>
#include <QDateTime>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "qwt_symbol.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "histogrambox.hpp"
#include "mainbox.hpp"
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
    all_break = true;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    all_break = false;

    createTestBar();

    histogram = new HistogramBox(this);
    histogram->set_title("тест");
    histogram->set_title_axis_X("тест X");
    histogram->set_title_axis_Y("тест Y");

#if 1
    histogram->add_histogram(0, tr("red"), Qt::red);
    histogram->add_histogram(1, tr("green"), Qt::green);
    histogram->add_histogram(2, tr("blue"), Qt::blue);
#else
    for(int n=0; n<2; n++)
    {
        QColor color = QColor(rand() % 255, rand() % 255, rand() % 255);
        histogram->add_histogram(n, QString(tr("histogram %1")).arg(n), color);
    }
#endif

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(histogram);
    setLayout(hbox);
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("toolBar");

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

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

    mw->load_html();
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    mw->save_html();
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
#if 0
    histogram->test();
#endif

#if 1
    emit info(tr("test begin"));
    //histogram->clear();
    histogram->push_btn_Horizontal(true);
    histogram->push_btn_Vertical(true);
    int pos_x = 0;
    for(int x=0; x<histogram->get_histograms_count(); x++)
    {
        if(all_break) return;
        QCoreApplication::processEvents();
        histogram->add_histogram_data(x, pos_x, 5, qrand() % 100);
        pos_x += 7;
    }
    emit info(tr("test end"));
#endif
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
