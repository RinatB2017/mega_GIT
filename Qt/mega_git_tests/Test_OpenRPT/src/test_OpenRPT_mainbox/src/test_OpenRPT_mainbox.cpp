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
#include <QPrintPreviewWidget>
#include <QPrintDialog>
#include <QPageSize>
#include <QPrinter>
//--------------------------------------------------------------------------------
#include "ui_test_OpenRPT_mainbox.h"
//--------------------------------------------------------------------------------
#include "previewdialog.h"
#include "renderobjects.h"
#include "parsexmlutils.h"

#include "orprintrender.h"
#include "orprerender.h"
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

    setVisible(false);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw != nullptr);

    commands.clear(); int id = 0;
    commands.append({ id++, "example 0", &MainBox::test_0 });
    commands.append({ id++, "example 1", &MainBox::test_1 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

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

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);

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
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setFullPage(true);
    printer->setPageSize(QPrinter::A4);
    printer->setOrientation(QPrinter::Landscape);

    PreviewDialog *dlg = new PreviewDialog(document, printer);
    int res = dlg->exec();
    if(res)
    {
        emit info(QString("primitives %1").arg(page->primitives()));
    }

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);

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
    printer->setOrientation(QPrinter::Landscape);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setPageSize(QPrinter::A4);
    printer->setFullPage(true);

    ORPrintRender render;
    render.setupPrinter(document, printer);
    render.setPrinter(printer);
    render.render(document);

    QString pdf_filename = "print.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pdf_filename));

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
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
