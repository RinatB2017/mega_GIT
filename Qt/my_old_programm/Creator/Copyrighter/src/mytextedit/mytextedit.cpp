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
#include "mytextedit.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    connect_log();
}
//--------------------------------------------------------------------------------
void MyTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();

    QAction *open = new QAction(tr("Open"), this);
    QAction *save = new QAction(tr("Save"), this);

    open->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    save->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(open, SIGNAL(triggered()), this, SLOT(open_file()));
    connect(save, SIGNAL(triggered()), this, SLOT(save_file()));

    menu->addAction(open);
    menu->addAction(save);
    menu->exec(event->globalPos());
    delete menu;
}
//--------------------------------------------------------------------------------
void MyTextEdit::connect_log(void)
{
    if(parentWidget())
    {
        //qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        //qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void MyTextEdit::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data);
#endif
}
//--------------------------------------------------------------------------------
void MyTextEdit::open_file(void)
{
    QFileDialog *dlg = 0;

    dlg = new QFileDialog;
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname.txt");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        emit debug(QString(tr("open %1")).arg(filename));
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QByteArray ba = file.readAll();
            if(ba.isEmpty() == false)
            {
                setText(ba.data());
            }
            file.close();
        }
        else
        {
            emit error(QString(tr("file %1 not open")).arg(filename));
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void MyTextEdit::save_file(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname.txt");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        emit debug(filename);
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qint64 bytes = file.write(toPlainText().toLocal8Bit());
            emit info(QString(tr("save %1 bytes")).arg(bytes));
            file.close();
        }
        else
        {
            emit error(QString(tr("file %1 not open")).arg(filename));
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
