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
#include <QContextMenuEvent>
#include <QLibraryInfo>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QToolButton>
#include <QFileDialog>
#include <QProcess>
#include <QToolBar>
#include <QWidget>
#include <QTimer>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mytextedit.hpp"
#include "mainbox.hpp"
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

    QVBoxLayout *vbox = new QVBoxLayout;

    srcTextEdit = new MyTextEdit(this);
    dstTextEdit = new MyTextEdit(this);

    vbox->addWidget(srcTextEdit);
    vbox->addWidget(dstTextEdit);

    setLayout(vbox);

    //srcTextEdit->setReadOnly(true);

#if 0
    QString temp = "Правила системы  \n"
            "1. Статья продается ОДИН раз. После продажи, все права на статью переходят к покупателю. Вы, как копирайтер, не имеете права дальнейшего распространения или продажи данной статьи в любом виде. \n"
            "Перепродажи статей запрещены.  \n"
            "2. Покупатель имеет право высказать претензии по поводу качества статьи или наличия копий статьи в интернете в течение суток с момента покупки.\n"
            "3. Выплаты производятся на 2-е сутки после заказа на вывод.\n"
            "4. Размещаемая статья должна быть написана Вами.\n"
            "5. Размещая статью, вы подтверждаете, что она не распространялась ранее (интернет, печатные издания и любые другие способы), не была сканирована с бумажного носителя, не была куплена у третьих лиц.\n"
            "6. Эротические рассказы и другие материалы, которые запрещены к распространению законодательством, продавать запрещено! \n"
            "7. Передача каких либо контактных данных или любого способа связи вне системы (e-mail, ICQ, телефоны, skype, номера кошельков, адреса любых сайтов с контактной информацией, намеки.    ";
    dstTextEdit->setText(temp);
    textChanged();
#endif

    createTestBar();

    connect(srcTextEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(dstTextEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    textChanged();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    // qDebug() << "(MainWindow *)" << mw;
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
    QString temp;
    temp = srcTextEdit->toPlainText().simplified();
    if(temp.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    QStringList sl = temp.split(".");
    foreach (QString text, sl)
    {
        if(text.isEmpty() == false)
        {
            dstTextEdit->append(QString("%1.").arg(text.replace(",", ",бля, ").simplified()));
            //dstTextEdit->append(QString("%1.").arg(text.simplified()));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::textChanged(void)
{
    QString clean_src = srcTextEdit->toPlainText().remove(" ");
    QString clean_dst = dstTextEdit->toPlainText().remove(" ");

    MainWindow *parent = (MainWindow *)parentWidget();
    parent->set_status1_text(QString(tr("text %1 clean text %2"))
                             .arg(srcTextEdit->toPlainText().size())
                             .arg(clean_src.length()));
    parent->set_status2_text(QString(tr("text %1 clean text %2"))
                             .arg(dstTextEdit->toPlainText().size())
                             .arg(clean_dst.length()));
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
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
