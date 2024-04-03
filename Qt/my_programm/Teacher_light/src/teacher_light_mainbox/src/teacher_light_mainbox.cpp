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
#include "ui_teacher_light_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "notebook.hpp"
#include "teacher_light_mainbox.hpp"
#include "defines.hpp"
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
    if(note)
    {
        note->check_modified();
        delete note;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::prepare_tab(void)
{
    QLabel *lbl_picture = new QLabel(this);
    lbl_picture->setPixmap(QPixmap(":/english_table.png"));

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

    tab->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_notebook(void)
{
    note = new Notebook(this);

#ifndef USE_DOCKS
    QVBoxLayout *vbox_other = new QVBoxLayout();
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    vbox_other->setMargin(0);
#endif
    vbox_other->setSpacing(0);
    vbox_other->addWidget(tab);
    vbox_other->addWidget(note);

    notebook_frame = new QFrame(this);
    notebook_frame->setLayout(vbox_other);
    notebook_frame->setFixedWidth(notebook_frame->sizeHint().width());
#endif
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
    prepare_grids();
    prepare_tab();
    prepare_notebook();

    //---
#ifdef USE_DOCKS
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QDockWidget *dock_note = new QDockWidget(tr("notebook"), this);
    Q_ASSERT(dock_note);

    QDockWidget *dock_tab = new QDockWidget(tr("tab"), this);
    Q_ASSERT(dock_tab);

    dock_note->setObjectName("dock_note");
    dock_tab->setObjectName("dock_tab");

    dock_note->setWidget(note);
    dock_tab->setWidget(tab);

    //mw->add_windowsmenu_action(dock_note, dock_note->toggleViewAction());
    //mw->add_windowsmenu_action(dock_tab, dock_tab->toggleViewAction());

    mw->addDockWidget(Qt::TopDockWidgetArea,    dock_tab);
    mw->addDockWidget(Qt::BottomDockWidgetArea, dock_note);

    setHidden(true);    //TODO а так можно, интересно?
    //---
#else
    QHBoxLayout *main_box = new QHBoxLayout();
    main_box->addWidget(notebook_frame);
    setLayout(main_box);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    create_widgets();
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
