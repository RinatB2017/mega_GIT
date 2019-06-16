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
#include "ui_show_films_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "treefilms.hpp"
#include "qxmlputget.h"
#include "show_films_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::load_translations(void)
{
    bool res;
    QTranslator *sysTranslator;

    sysTranslator = new QTranslator(this);
    res = sysTranslator->load(QLatin1String("qt_ru"), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if(!res)
        emit error(tr("sysTranslator not loaded!"));

    qApp->installTranslator(sysTranslator);
}
//--------------------------------------------------------------------------------
void MainBox::init_tree_widget(void)
{
    tree_films = new TreeFilms(this);
    lblFilmPicture = new QLabel(this);
    lblFilmPicture->setMinimumSize(MAX_WIDTH, MAX_HEIGHT);

    picture_vbox = new QVBoxLayout();
    picture_hbox = new QHBoxLayout();
    picture_vbox->setMargin(0);
    picture_vbox->setSpacing(0);
    picture_hbox->setMargin(0);
    picture_hbox->setSpacing(0);
    picture_hbox->addStretch();
    picture_hbox->addWidget(lblFilmPicture);
    picture_hbox->addStretch();
    picture_vbox->addStretch();
    picture_vbox->addLayout(picture_hbox);
    picture_vbox->addStretch();

    ui->hbox->addWidget(tree_films, 1);
    ui->hbox->addLayout(picture_vbox, 1);

    connect(tree_films, SIGNAL(click_film(QString)),
            this, SLOT(show_picture(QString)));
    connect(this, SIGNAL(double_click()),
            tree_films, SLOT(double_click()));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        connect(tree_films, SIGNAL(set_status_text(QString)),
                mw, SLOT(set_status1_text(QString)));
    }
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    load_translations();
    init_tree_widget();
    create_menu();

    updateText();

    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    size(),
                                    qApp->desktop()->availableGeometry()));

    lblFilmPicture->installEventFilter(this);
    lblFilmPicture->setScaledContents(true);
    lblFilmPicture->setCursor(Qt::PointingHandCursor);

    current_picture_name = FOLDER_FILE;
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        emit double_click();
        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
void MainBox::show_picture(const QString &data)
{
    qreal multiplier = 1.0;
    qreal width  = MAX_WIDTH;
    qreal height = MAX_HEIGHT;
    QPixmap new_picture;

    if(data.isEmpty())
    {
        current_picture_name = SPACE_FILE;
    }
    else
    {
        if(QFile(data).exists() == false)
            current_picture_name = SPACE_FILE;
        else
            current_picture_name = data;
    }

    current_picture.load(current_picture_name);
    multiplier = static_cast<double>(lblFilmPicture->height()) / static_cast<double>(current_picture.height());
    width = current_picture.width() * multiplier;
    height = current_picture.height() * multiplier;

    new_picture = current_picture.scaled(static_cast<int>(width),
                                         static_cast<int>(height),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
    lblFilmPicture->setPixmap(new_picture);
}
//--------------------------------------------------------------------------------
void MainBox::resizeEvent(QResizeEvent *event)
{
    QPixmap new_picture;
    double width  = MAX_WIDTH;
    double height = MAX_HEIGHT;

    if(current_picture_name.isEmpty() == false)
    {
        current_picture.load(current_picture_name);

        double fixed_multplier = static_cast<double>(MAX_WIDTH) / static_cast<double>(MAX_HEIGHT);
        double real_multplier = static_cast<double>(tree_films->width()) / static_cast<double>(tree_films->height());
        if(fixed_multplier > real_multplier)
        {
            width = tree_films->width();
            height = (width * fixed_multplier) + 0.5;
        }
        else
        {
            height = tree_films->height();
            width = (height * fixed_multplier) + 0.5;
        }

        new_picture = current_picture.scaled(static_cast<int>(width),
                                             static_cast<int>(height),
                                             Qt::KeepAspectRatioByExpanding,
                                             Qt::SmoothTransformation);
        lblFilmPicture->setPixmap(new_picture);
    }
    Q_UNUSED(event);
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    tree_films->save();
}
//--------------------------------------------------------------------------------
void MainBox::select_last_film(void)
{
    if(last_film->text().isEmpty()) return;

    QTreeWidgetItemIterator items(tree_films);
    while (*items)
    {
        if((*items)->text(0) == last_film->text().remove("&"))
        {
            tree_films->setCurrentItem(*items);
        }
        *items++;
    }
}
//--------------------------------------------------------------------------------
void MainBox::create_menu(void)
{
    change_player_action = new QAction(this);
    change_player_action->setIcon(QIcon(QLatin1String(":/mainwindow/mplayer.png")));

    save_action = new QAction(this);
    save_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    last_film = new QAction(this);
    last_film->setIcon(QIcon(QLatin1String(":/mainwindow/mplayer.png")));
    last_film->setText("Сваты 2");

    connect(change_player_action, SIGNAL(triggered()), this, SLOT(change_player()));
    connect(save_action, SIGNAL(triggered()), this, SLOT(save()));
    connect(last_film, SIGNAL(triggered()), this, SLOT(select_last_film()));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    mw->add_filemenu_action(0, change_player_action);
    mw->add_filemenu_separator(1);
    mw->add_filemenu_action(2, save_action);
    mw->add_filemenu_separator(3);
    mw->add_filemenu_action(4, last_film);
    mw->add_filemenu_separator(5);
}
//--------------------------------------------------------------------------------
void MainBox::change_player(void)
{
    QFileDialog *dlg = new QFileDialog();
    dlg->setWindowTitle(QObject::tr("change player"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    //??? dlg->selectFile(engine->get_default_player());

    int button = dlg->exec();
    if(button != QFileDialog::Accepted) return;

    QString exe_file = dlg->selectedFiles().at(0);
    if(exe_file.isEmpty()) return;

    QFileInfo fi(exe_file);
    if(fi.isExecutable() == false) return;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);

    change_player_action->setText(QObject::tr("change player"));
    save_action->setText(QObject::tr("save"));
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
