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
//--------------------------------------------------------------------------------
#include "ui_icons_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "icons_mainbox.hpp"
#include "myfiledialog.hpp"
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

    tab = new QTabWidget(this);

    QTabWidget *crystal_clear = new QTabWidget(this);
    QTabWidget *oxygen = new QTabWidget(this);
    QTabWidget *nuvola = new QTabWidget(this);
    QTabWidget *gnome = new QTabWidget(this);
    QTabWidget *hicolor = new QTabWidget(this);
    QTabWidget *lol = new QTabWidget(this);

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

    Q_CHECK_PTR(tab);

    QVBoxLayout *vbox = new QVBoxLayout;
    setLayout(vbox);

    layout()->addWidget(tab);

    QPushButton *btn_clean = new QPushButton("Очистить");
    QPushButton *btn_save  = new QPushButton("Сохранить");

    connect(btn_clean,  SIGNAL(clicked(bool)),  this,   SLOT(clean()));
    connect(btn_save,   SIGNAL(clicked(bool)),  this,   SLOT(save()));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(btn_clean);
    hbox->addWidget(btn_save);
    hbox->addStretch(1);

    QWidget *w = new QWidget;
    w->setLayout(hbox);

    layout()->addWidget(w);
}
//--------------------------------------------------------------------------------
void MainBox::clean(void)
{
    emit info("Очистка нажатых кнопок");
    foreach (QToolButton *btn, l_buttons)
    {
        if(btn)
        {
            btn->setChecked(false);
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    emit info("save");
    int cnt = 0;
    foreach (QToolButton *btn, l_buttons)
    {
        if(btn)
        {
            if(btn->isChecked())
            {
                cnt++;
#ifdef QT_DEBUG
                QString filename = btn->property("filename").toString();
                QString catalog_name = btn->property("catalog_name").toString();

                emit debug("---");
                emit debug(QString("filename %1").arg(filename));
                emit debug(QString("catalog_name %1").arg(catalog_name));
#endif
            }
        }
    }
    if(cnt == 0)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Нет выбранных иконок!"));
        return;
    }
    MyFileDialog *dlg = 0;
    dlg = new MyFileDialog("MyFileDialog", "MyFileDialog");
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("qrc files (*.qrc)"));
    dlg->setDefaultSuffix(tr("qrc"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec() == false)
    {
        return;
    }

    QStringList files = dlg->selectedFiles();
    emit debug(files.at(0));
    emit info(dlg->directory().dirName());

    cnt = 0;
    QString path = QString("%1/")
            .arg(dlg->directory().absolutePath());
    QString prefix = "icons";
    QString qrc_file;
    qrc_file.append("<RCC>\n");
    qrc_file.append(QString("    <qresource prefix=\"/%1\">\n")
                    .arg(prefix));
    foreach (QToolButton *button, l_buttons)
    {
        if(button->isChecked())
        {
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

                emit info(QString("pixmap.load %1").arg(filename));
                emit info(QString("pixmap.save %1").arg(full_path));
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
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
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
void MainBox::add_icons(QTabWidget *page,
                        const QString &catalog_name,
                        int max_x)
{
    Q_CHECK_PTR(page);

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
        btn->setCheckable(true);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(icon);
        btn->setToolTip(filename);
        btn->setObjectName(filename);
        l_buttons.append(btn);

        QFileInfo fi(filename);
        QString name = fi.fileName();
        btn->setProperty("filename", name);
        btn->setProperty("catalog_name", catalog_name);

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

    QScrollArea *area = new QScrollArea(this);
    area->setWidget(view);
    area->setWidgetResizable(true);

    QFile file(catalog_name);
    QFileInfo fileInfo(file.fileName());
    page->addTab(area, fileInfo.fileName());
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
