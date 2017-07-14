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
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>
#include <QScrollArea>
#include <QToolBar>
#include <QWidget>
#include <QEvent>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
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

    tab = new QTabWidget(this);
    ui->verticalLayout->addWidget(tab);

    QTabWidget *crystal_clear = new QTabWidget(this);
    QTabWidget *oxygen = new QTabWidget(this);
    QTabWidget *nuvola = new QTabWidget(this);
    QTabWidget *gnome = new QTabWidget(this);
    QTabWidget *hicolor = new QTabWidget(this);
    QTabWidget *lol = new QTabWidget(this);

    ui->drop_frame->installEventFilter(this);

    tab->addTab(crystal_clear, "crystal clear");
    tab->addTab(oxygen, "oxygen");
    tab->addTab(nuvola, "nuvola");
    tab->addTab(gnome, "gnome");
    tab->addTab(hicolor, "hicolor");
    tab->addTab(lol, "lol");

    add_icons(gnome, ":/gnome/actions");
    add_icons(gnome, ":/gnome/animations");
    add_icons(gnome, ":/gnome/apps");
    add_icons(gnome, ":/gnome/categories");
    add_icons(gnome, ":/gnome/devices");
    add_icons(gnome, ":/gnome/emblems");
    add_icons(gnome, ":/gnome/emotes");
    add_icons(gnome, ":/gnome/mimetypes");
    add_icons(gnome, ":/gnome/places");
    add_icons(gnome, ":/gnome/status");

    add_icons(hicolor, ":/hicolor/actions");
    add_icons(hicolor, ":/hicolor/apps");
    add_icons(hicolor, ":/hicolor/mimetypes");
    add_icons(hicolor, ":/hicolor/places");

    add_icons(crystal_clear, ":/crystal_clear/actions");
    add_icons(crystal_clear, ":/crystal_clear/apps");
    add_icons(crystal_clear, ":/crystal_clear/devices");
    add_icons(crystal_clear, ":/crystal_clear/filesystems");
    add_icons(crystal_clear, ":/crystal_clear/kdm");
    add_icons(crystal_clear, ":/crystal_clear/mimetypes");

    add_icons(oxygen, ":/oxygen/actions");
    add_icons(oxygen, ":/oxygen/animations");
    add_icons(oxygen, ":/oxygen/apps");
    add_icons(oxygen, ":/oxygen/categories");
    add_icons(oxygen, ":/oxygen/devices");
    add_icons(oxygen, ":/oxygen/emblems");
    add_icons(oxygen, ":/oxygen/emotes");
    add_icons(oxygen, ":/oxygen/mimetypes");
    add_icons(oxygen, ":/oxygen/places");
    add_icons(oxygen, ":/oxygen/status");

    add_icons(nuvola, ":/nuvola/actions");
    add_icons(nuvola, ":/nuvola/apps");
    add_icons(nuvola, ":/nuvola/categories");
    add_icons(nuvola, ":/nuvola/devices");
    add_icons(nuvola, ":/nuvola/emblems");
    add_icons(nuvola, ":/nuvola/mimetypes");
    add_icons(nuvola, ":/nuvola/places");
    add_icons(nuvola, ":/nuvola/status");

    add_icons(lol, ":/lol/actions");
    add_icons(lol, ":/lol/apps");
    add_icons(lol, ":/lol/devices");
    add_icons(lol, ":/lol/filesystems");
    add_icons(lol, ":/lol/mimetypes");
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

    // qDebug() << "(MainWindow *)" << mw;
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::add_icons(QTabWidget *page,
                        const QString &catalog_name,
                        int max_x)
{
    QWidget *view = new QWidget;
    QGridLayout *gbox = new QGridLayout;
    gbox->setMargin(0);
    gbox->setSpacing(0);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addLayout(gbox);
    hbox->addStretch();

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addStretch();

    view->setLayout(vbox);

    QDirIterator it(catalog_name, QDir::Files, QDirIterator::Subdirectories);
    int n=0;
    int x=0;
    int y=0;

    QStringList files;
    files.clear();
    while (it.hasNext())
    {
        files << it.next();
    }
    files.sort();

    foreach (QString filename, files)
    {
        QPixmap pixmap;
        pixmap.load(filename);
        QIcon icon(pixmap);
        QToolButton *btn = new QToolButton(this);
        //btn->setCheckable(true);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(icon);
        btn->setToolTip(filename);
        btn->setObjectName(filename);

        QFileInfo fi(filename);
        QString name = fi.fileName();
        btn->setProperty("filename", name);

        connect(btn, SIGNAL(pressed()), this, SLOT(drag()));

        gbox->addWidget(btn, y, x);
        if(x < (max_x - 1))
        {
            x++;
        }
        else
        {
            x=0;
            y++;
        }
        n++;
    }
#if 0
    emit info(QString("in %1 found %2 files")
              .arg(catalog_name)
              .arg(n));
#endif

    QScrollArea *area = new QScrollArea(this);
    area->setWidget(view);
    area->setWidgetResizable(true);

    QFile file(catalog_name);
    QFileInfo fileInfo(file.fileName());
    page->addTab(area, fileInfo.fileName());
}
//--------------------------------------------------------------------------------
void MainBox::drag(void)
{
    QToolButton *btn = (QToolButton *)sender();
    setCursor(btn->icon().pixmap(32, 32));
    current_png = btn->property("filename").toString();
    emit info(current_png);
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *obj, QEvent *ev)
{
    if ((obj == ui->drop_frame) && (ev->type() == QEvent::MouseButtonPress))
    {
        QToolButton *btn = new QToolButton();
        if(current_png.isEmpty() == false)
        {
            btn->setIcon(cursor().pixmap());
            x++;
            if(x>3)
            {
                x=0;
                y++;
            }
            ui->grid->addWidget(btn, y, x);
            cursor().pixmap().save(current_png);
        }
        return true;
    }
    return QObject::eventFilter(obj, ev);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    int cnt = 0;
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    foreach (QToolButton *button, allobj)
    {
        if(button->isChecked())
            cnt++;
    }
    if(cnt == 0)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Нет выбранных иконок!"));
        return;
    }

    QFileDialog *dlg;
    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("qrc files (*.qrc)"));
    dlg->setDefaultSuffix(tr("qrc"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec() == false)
        return;
    QStringList files = dlg->selectedFiles();
    emit debug(files.at(0));
    emit info(dlg->directory().dirName());

    cnt = 0;
    QString path = QString("%1/")
            .arg(dlg->directory().dirName());
    QString prefix = "temp";
    QString qrc_file;
    qrc_file.append("<RCC>\n");
    qrc_file.append(QString("    <qresource prefix=\"/%1\">\n")
                    .arg(prefix));
    foreach (QToolButton *button, allobj)
    {
        if(button->isChecked())
        {
            cnt++;

            QString filename = button->objectName();
            if(filename.isEmpty() == false)
            {
                QFile file(filename);
                QFileInfo fileInfo(file.fileName());

                qrc_file.append(QString("        <file>%1</file>\n")
                                .arg(fileInfo.fileName()));

                QString full_path = QString("%1%2")
                        .arg(path)
                        .arg(fileInfo.fileName());

                QPixmap pixmap;
                pixmap.load(filename);
                pixmap.save(full_path);
            }
        }
    }
    qrc_file.append("    </qresource>\n");
    qrc_file.append("</RCC>\n");

    QFile file(files.at(0));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(qrc_file.toLocal8Bit());
        file.close();
    }
    emit info(qrc_file);

    emit debug(QString(tr("обработано %1")).arg(cnt));
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
