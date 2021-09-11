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
#include <QWebEngineView>
//--------------------------------------------------------------------------------
#include "ui_teacher_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "notebook.hpp"
#include "teacher_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    is_blocked(false)
{
    init();;
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(note)
    {
        note->check_modified();
        note->deleteLater();
    }
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::prepare_browsers(void)
{
    browser_site = new QWebEngineView(this);
    browser_translate_en_ru = new QWebEngineView(this);
    browser_translate_ru_en = new QWebEngineView(this);

    browser_site->setUrl(QUrl(DEFAULT_SITE));

    browser_translate_en_ru->setUrl(QUrl(DEFAULT_TRANSLATE_EN_RU));
    browser_translate_ru_en->setUrl(QUrl(DEFAULT_TRANSLATE_RU_EN));
}
//--------------------------------------------------------------------------------
void MainBox::prepare_tab(void)
{
    QLabel *lbl_picture = new QLabel();
    lbl_picture->setPixmap(QPixmap(":/english_table.png"));

    site_address->setText(DEFAULT_SITE);

    QWidget *w = new QWidget(this);
    w->setLayout(grid1);

    QWidget *w2 = new QWidget(this);
    w2->setLayout(grid2);

    QWidget *w3 = new QWidget(this);
    w3->setLayout(grid3);

    QWidget *w4 = new QWidget(this);
    w4->setLayout(grid4);

    tab = new QTabWidget(this);
    tab->addTab(lbl_picture, "table");
    tab->addTab(w,  "grid 1");
    tab->addTab(w2, "grid 2");
    tab->addTab(w3, "grid 3");
    tab->addTab(w4, "grid 4");
    //tab->setFixedSize(tab->sizeHint());
    tab->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_notebook(void)
{
    note = new Notebook(this);

    QVBoxLayout *vbox_other = new QVBoxLayout();
    vbox_other->setMargin(0);
    vbox_other->setSpacing(0);
    vbox_other->addWidget(tab);
    vbox_other->addWidget(note);

    notebook_frame = new QFrame(this);
    notebook_frame->setLayout(vbox_other);
    notebook_frame->setFixedWidth(notebook_frame->sizeHint().width());
}
//--------------------------------------------------------------------------------
void MainBox::prepare_grids(void)
{
    QList<QStringList> lists;
    QStringList sl;
    sl.clear(); sl << "Что? Какой?"     << "WHAT?";     lists.append(sl);
    sl.clear(); sl << "Кто?"            << "WHO?";      lists.append(sl);
    sl.clear(); sl << "Где? Куда?"      << "WHERE?";    lists.append(sl);
    sl.clear(); sl << "Когда?"          << "WHEN?";     lists.append(sl);
    sl.clear(); sl << "Почему? Зачем?"  << "WHY?";      lists.append(sl);
    sl.clear(); sl << "Как?"            << "HOW?";      lists.append(sl);

    grid1 = new QGridLayout();
    int y = 0;
    foreach (QStringList sl, lists)
    {
        int x = 0;
        foreach (QString text, sl)
        {
            QLabel *label = new QLabel(text);
            grid1->addWidget(label, y, x);
            x++;
        }
        y++;
    }
    //---

    lists.clear();
    sl.clear(); sl << ""    << "Кто?" << "Кого? Кому?"   << "Чей";      lists.append(sl);
    sl.clear(); sl << "Я"   << "I"    << "ME"            << "MY";       lists.append(sl);
    sl.clear(); sl << "Ты"  << "YOU"  << "YOU"           << "YOUR";     lists.append(sl);
    sl.clear(); sl << "Он"  << "HIM"  << "HIM"           << "HIS";      lists.append(sl);
    sl.clear(); sl << "Она" << "HER"  << "HER"           << "HER";      lists.append(sl);
    sl.clear(); sl << "Мы"  << "US"   << "US"            << "OUR";      lists.append(sl);
    sl.clear(); sl << "Они" << "THEM" << "THEM"          << "THEIR";    lists.append(sl);

    grid2 = new QGridLayout();
    y = 0;
    foreach (QStringList sl, lists)
    {
        int x = 0;
        foreach (QString text, sl)
        {
            QLabel *label = new QLabel(text);
            label->setAlignment(Qt::AlignHCenter);
            grid2->addWidget(label, y, x);
            x++;
        }
        y++;
    }
    //---

    lists.clear();
    sl.clear(); sl << "понедельник" << "Monday";    lists.append(sl);
    sl.clear(); sl << "вторник"     << "Tuesday";   lists.append(sl);
    sl.clear(); sl << "среда"       << "Wednesday"; lists.append(sl);
    sl.clear(); sl << "четверг"     << "Thursday";  lists.append(sl);
    sl.clear(); sl << "пятница"     << "Friday";    lists.append(sl);
    sl.clear(); sl << "суббота"     << "Saturday";  lists.append(sl);
    sl.clear(); sl << "воскресение" << "Sunday";    lists.append(sl);

    grid3 = new QGridLayout();
    y = 0;
    foreach (QStringList sl, lists)
    {
        int x = 0;
        foreach (QString text, sl)
        {
            QLabel *label = new QLabel(text);
            grid3->addWidget(label, y, x);
            x++;
        }
        y++;
    }
    //---

    lists.clear();
    sl.clear(); sl << "январь"      << "January";   lists.append(sl);
    sl.clear(); sl << "февраль"     << "February";  lists.append(sl);
    sl.clear(); sl << "март"        << "March";     lists.append(sl);
    sl.clear(); sl << "апрель"      << "April";     lists.append(sl);
    sl.clear(); sl << "май"         << "May";       lists.append(sl);
    sl.clear(); sl << "июнь"        << "June";      lists.append(sl);
    sl.clear(); sl << "июль"        << "July";      lists.append(sl);
    sl.clear(); sl << "август"      << "August";    lists.append(sl);
    sl.clear(); sl << "сентябрь"    << "September"; lists.append(sl);
    sl.clear(); sl << "октябрь"     << "October";   lists.append(sl);
    sl.clear(); sl << "ноябрь"      << "November";  lists.append(sl);
    sl.clear(); sl << "декабрь"     << "December";  lists.append(sl);

    grid4 = new QGridLayout();
    y = 0;
    foreach (QStringList sl, lists)
    {
        int x = 0;
        foreach (QString text, sl)
        {
            QLabel *label = new QLabel(text);
            grid4->addWidget(label, y, x);
            x++;
        }
        y++;
    }
    //---
}
//--------------------------------------------------------------------------------
void MainBox::create_widgets(void)
{
    prepare_browsers();
    prepare_grids();
    prepare_tab();
    prepare_notebook();

#ifdef ULTRAWIDE
    QSplitter *h_splitter = new QSplitter(Qt::Horizontal);
    QSplitter *v_splitter = new QSplitter(Qt::Vertical);
    h_splitter->setObjectName("h_splitter");
    v_splitter->setObjectName("v_splitter");

    h_splitter->setChildrenCollapsible(false);
    v_splitter->setChildrenCollapsible(false);

    browser_site->setParent(h_splitter);

    browser_translate_en_ru->setParent(v_splitter);
    browser_translate_ru_en->setParent(v_splitter);

    v_splitter->addWidget(browser_translate_en_ru);
    v_splitter->addWidget(browser_translate_ru_en);

    h_splitter->addWidget(browser_site);
    h_splitter->addWidget(v_splitter);

    QHBoxLayout *main_box = new QHBoxLayout();
    main_box->addWidget(h_splitter);
    main_box->addWidget(notebook_frame);
#else
    QSplitter *v_splitter = new QSplitter(Qt::Vertical);
    QSplitter *h_splitter = new QSplitter(Qt::Horizontal);
    h_splitter->setObjectName("h_splitter");
    v_splitter->setObjectName("v_splitter");

    v_splitter->setChildrenCollapsible(false);
    h_splitter->setChildrenCollapsible(false);

    browser_site->setParent(v_splitter);

    browser_translate_en_ru->setParent(h_splitter);
    browser_translate_ru_en->setParent(h_splitter);

    v_splitter->addWidget(browser_site);
    v_splitter->addWidget(h_splitter);

    QHBoxLayout *main_box = new QHBoxLayout();
    main_box->addWidget(v_splitter);
    main_box->addWidget(notebook_frame);
#endif
    setLayout(main_box);
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif
    create_widgets();

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_back = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaSeekBackward),
                                       "back",
                                       "back");

    site_address = new QLineEdit(this);
    testbar->addWidget(site_address);

    QToolButton *btn_set_address = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "set address",
                                              "set address");

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                       "test",
                                       "test");

    connect(site_address,       SIGNAL(returnPressed()),    this,   SLOT(set_address()));
    connect(btn_set_address,    SIGNAL(clicked(bool)),      this,   SLOT(set_address()));
    connect(btn_back, SIGNAL(clicked(bool)), this, SLOT(back()));

    connect(btn_test, SIGNAL(clicked(bool)), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::set_address(void)
{
    QString address = site_address->text();
    if(address.isEmpty())
    {
        emit error("address is empty!");
        return;
    }

    if(!address.toLower().startsWith("http"))
    {
        QString temp;
        temp.clear();
        temp.append("http://");
        temp.append(address);
        address = temp;

        site_address->setText(temp);
    }

    browser_site->setUrl(QUrl(address));
}
//--------------------------------------------------------------------------------
void MainBox::back(void)
{
    browser_site->back();
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QList<QSplitter *> allobj = findChildren<QSplitter *>();

    foreach(QSplitter *obj, allobj)
    {
        qDebug() << obj->objectName() << obj->pos();
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
bool MainBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        emit info("MouseButtonPress");
        //return true;
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        emit info("MouseButtonRelease");
        //return true;
    }
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
