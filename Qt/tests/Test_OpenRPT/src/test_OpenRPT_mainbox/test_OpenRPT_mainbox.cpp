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
#include <QPrinter>
//--------------------------------------------------------------------------------
#include "ui_test_OpenRPT_mainbox.h"
//--------------------------------------------------------------------------------
#include "previewdialog.h"
#include "renderobjects.h"
#include "parsexmlutils.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "defines.hpp"
#include "test_OpenRPT_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
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
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit debug(tr("тест"));

    ORODocument *document = new ORODocument();
    OROPage *page = new OROPage();

    ORBarcodeData * bc = new ORBarcodeData();
    //bc->setBrush(QBrush(QColor(Qt::red)));

    ORORect *rect = new ORORect(bc);
    rect->setRect(QRectF(1/2.54, 1/2.54, 1, 1));

    OROLine *line = new OROLine(bc);
    line->setStartPoint(QPointF(1, 1));
    line->setEndPoint(QPointF(3, 2));

    OROLine *line2 = new OROLine(bc);
    line2->setStartPoint(QPointF(1, 1));
    line2->setEndPoint(QPointF(5, 5));

    QFont font("Arial", 24);
    OROTextBox *text = new OROTextBox(bc);
    text->setPosition(QPointF(3, 3));
    text->setFont(font);
    text->setSize(QSizeF(4.0, 2.0));
    //text->setFlags(Qt::AlignRight | Qt::AlignTop);
    text->setRotation(45.0);
    text->setText("русский текст");

    page->addPrimitive(rect);
    page->addPrimitive(line);
    page->addPrimitive(line2);
    page->addPrimitive(text);

    document->addPage(page);

    QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    PreviewDialog *dlg = new PreviewDialog(document, printer);
    int res = dlg->exec();
    if(res)
    {
        qDebug() << page->primitives();
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
