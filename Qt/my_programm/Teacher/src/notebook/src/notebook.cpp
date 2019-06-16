/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#   include <QStackedWidget>
#   include <QApplication>
#   include <QMessageBox>
#   include <QVBoxLayout>
#   include <QToolButton>
#   include <QPushButton>
#   include <QTextEdit>
#   include <QStyle>
#endif
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "notebook.hpp"
//--------------------------------------------------------------------------------
#include "qxmlputget.h"
//--------------------------------------------------------------------------------
Notebook::Notebook(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Notebook::~Notebook()
{
    if(sw)
    {
        sw->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void Notebook::init(void)
{
    alphabet.clear();
    alphabet << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M"
             << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z";

    sw = new QStackedWidget(this);

    QVBoxLayout *btns = new QVBoxLayout();
    btns->setSpacing(0);
    btns->setMargin(0);

    foreach (QString text, alphabet)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setText(text);
        btn->setProperty("ID", text);
        btn->setFixedHeight(20);
        btns->addWidget(btn);

        QTextEdit *te = new QTextEdit(this);
        //te->append(text);
        te->setProperty("property_ID", text);

        //---
        QStringList sl;
        bool ok = load_data(text, &sl);
        if(ok)
        {
            foreach (QString data, sl)
            {
                te->append(data);
                te->document()->setModified(false);
            }
        }
        //---

        sw->addWidget(te);

        connect(btn, SIGNAL(clicked(bool)), this, SLOT(btn_click()));
    }
    btns->addStretch(1);

    btn_save_all = new QPushButton();
    btn_save_all->setObjectName("btn_save_all");
    btn_save_all->setText("save all");
    btn_save_all->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));
    connect(btn_save_all, SIGNAL(clicked(bool)), this, SLOT(save_all()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(sw);
    vbox->addWidget(btn_save_all);

    QHBoxLayout *e = new QHBoxLayout();
    e->addLayout(btns);
    e->addLayout(vbox);

    setLayout(e);

    //setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void Notebook::save_all(void)
{
    QStringList alphabet;
    alphabet << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M"
             << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z";

    QXmlPut *xmlPut = new QXmlPut("Alphabet");
    if(!xmlPut)
    {
        emit error("file not open!");
        return;
    }
    foreach (QString param, alphabet)
    {
        xmlPut->descend(param);
        QStringList sl;
        bool ok = save_data(param, &sl);
        if(ok)
        {
            foreach (QString text, sl)
            {
                xmlPut->putString("name", text);
            }
        }
        xmlPut->rise();
    }
    xmlPut->save(ALPHABET_FILE);
}
//--------------------------------------------------------------------------------
void Notebook::btn_click(void)
{
    QToolButton *btn = (QToolButton *)sender();
    if(!btn) return;

    QString name = btn->property("ID").toString();
    if(name.isEmpty()) return;

    for(int n=0; n<sw->count(); n++)
    {
        QWidget *w = sw->widget(n);
        if(w)
        {
            if(name == w->property("property_ID"))
            {
                sw->setCurrentIndex(n);
                return;
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool Notebook::load_data(QString name, QStringList *data)
{
    QXmlGet *xmlGet = new QXmlGet();
    bool ok = xmlGet->load(ALPHABET_FILE);
    if(!ok)
    {
        emit error(QString("load %1 failed").arg(ALPHABET_FILE));
        return false;
    }

    ok = xmlGet->findAndDescend(name);
    if(ok)
    {
        ok = true;
        while(ok)
        {
            ok = xmlGet->findNext("name");
            if(ok)
            {
                QString value = xmlGet->getString("");
                *data << value;
            }
        }
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Notebook::save_data(QString name, QStringList *data)
{
    for(int n=0; n<sw->count(); n++)
    {
        QWidget *w = sw->widget(n);
        if(w)
        {
            if(name == w->property("property_ID"))
            {
                QTextEdit *te = (QTextEdit *)w;
                QString str=te->toPlainText();
                QStringList sl=str.split('\n');
                foreach (QString line, sl)
                {
                    *data << line;
                }
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void Notebook::check_modified(void)
{
    bool not_saved = false;
    for(int n=0; n<sw->count(); n++)
    {
         QTextEdit *te = dynamic_cast<QTextEdit *>(sw->widget(n));
         if(te)
         {
             if(te->document()->isModified())
             {
                 qDebug() << n;
                 not_saved = true;
             }
         }
    }
    if(not_saved)
    {
        int btn = messagebox_question("Записная книжка не сохранена", "Сохранить ?");
        if(btn == QMessageBox::Yes)
        {
            save_all();
        }
    }
}
//--------------------------------------------------------------------------------
void Notebook::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Notebook::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Notebook::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Notebook::save_setting(void)
{

}
//--------------------------------------------------------------------------------
