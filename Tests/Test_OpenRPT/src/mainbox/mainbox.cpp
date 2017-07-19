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
#include <QToolButton>
#include <QPrinter>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "previewdialog.h"
#include "renderobjects.h"
#include "parsexmlutils.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "defines.hpp"
#include "mainbox.hpp"
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

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
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
    rect->setRect(QRectF(1/2.54f, 1/2.54f, 1, 1));

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
    text->setSize(QSizeF(4.0f, 2.0f));
    //text->setFlags(Qt::AlignRight | Qt::AlignTop);
    text->setRotation(45.0f);
    text->setText("русский текст");

    page->addPrimitive(rect);
    page->addPrimitive(line);
    page->addPrimitive(line2);
    page->addPrimitive(text);

    document->addPage(page);

    QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    PreviewDialog *dlg = new PreviewDialog(document, printer);
    dlg->setWindowIcon(QIcon(ICON_PROGRAMM));
    int res = dlg->exec();
    if(res)
    {
        qDebug() << page->primitives();
    }
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
