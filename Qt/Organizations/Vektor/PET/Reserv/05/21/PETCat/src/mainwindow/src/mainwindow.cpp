/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QtPrintSupport/QPrintDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QtNetwork>
#include <QtWidgets>
#include <QSplitter>
#include <QDebug>
#include <QFont>
#include <QList>
#include <QRect>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "logdock.hpp"
//--------------------------------------------------------------------------------
#include "findpetbox.hpp"
//--------------------------------------------------------------------------------
#include "helpbrowser.hpp"
#include "optionbox.hpp"
#include "aboutbox.hpp"
#include "printbox.hpp"
//--------------------------------------------------------------------------------
#include "creatorbox.hpp"
#include "viewerbox.hpp"
#include "scan_control.hpp"
#include "scanbox_hard.hpp"
#include "pet_demo.hpp"
#include "MKT_box.hpp"
#include "AD9106_box.hpp"
#include "calibration.hpp"
#include "testbox_1.hpp"
#include "testbox_2.hpp"
#include "oscilloscope.hpp"
#include "floodmap.hpp"
#include "histogram_consilience.hpp"
#include "histogram_energy.hpp"
#include "petcat_optionsbox.hpp"
//--------------------------------------------------------------------------------
#include "imitator_pet.hpp"
#include "imitator_brg_gui.hpp"
//--------------------------------------------------------------------------------
#include "update_firmware.hpp"
//--------------------------------------------------------------------------------
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
    load_translations();

    mainToolBar = 0;

#ifdef STYLE_FUSION
    QStyle *style = QStyleFactory::create("Fusion");
    if(style)
    {
        QApplication::setStyle(style);
    }
#endif

    createActions();
    createStatusBar();

    //---
    create_left_dock();
    create_central_dock();
    create_log_dock();
    //---
#ifdef NET_DEBUG
    create_net_dock();
#endif
    //---

    readSettings();

    setWindowTitle(NAME_PROGRAMM);
    setUnifiedTitleAndToolBarOnMac(true);
    setMinimumSize(1280, 600);
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow(void)
{
    if(md)
    {
        md->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void MainWindow::load_translations(void)
{
#ifdef ONLY_ENGLISH
    return;
#endif

    //---
    bool res = false;
    //---
    sysTranslator = new QTranslator(this);
#ifdef Q_OS_LINUX
    res = sysTranslator->load(QLatin1String("qt_ru"), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#else
    res = sysTranslator->load("qt_ru", ":/widgets");
#endif
    if(!res)
    {
        MyWidget::messagebox_critical("Error", "sysTranslator not loaded");
        qDebug() << "sysTranslator not loaded!";
        return;
    }
    qApp->installTranslator(sysTranslator);
    //---
    appTranslator = new QTranslator(this);
    res = appTranslator->load(QLatin1String(":/programm"));
    if(!res)
    {
        MyWidget::messagebox_critical("Error", "appTranslator not loaded!");
        qDebug() << "appTranslator not loaded!";
        return;
    }
    qApp->installTranslator(appTranslator);
    //---
}
//--------------------------------------------------------------------------------
void MainWindow::f_print(void)
{
    PrintBox *w = new PrintBox(this);
    if(w)
    {
        w->setObjectName("PrintBox");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_PRINT);
        main_tab->setCurrentWidget(sa);
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_petcat_options(void)
{
    PETCat_optionsBox *w = new PETCat_optionsBox(this);
    if(w)
    {
        w->setObjectName("PETCat_options");
        int btn = w->exec();
        if(btn != QDialog::Accepted) return;
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_scan_control(void)
{
    Scan_control *w = new Scan_control(this);
    if(w)
    {
        w->setObjectName("Scan_control");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_SCAN_CONTROL);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_scan_hard(void)
{
    pet_discovery_data_t pet_config;
    ScanBox_hard *w = new ScanBox_hard(pet_config, this);
    if(w)
    {
        w->setObjectName("ScanBox_hard");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_SCAN_HARD);
        main_tab->setCurrentWidget(sa);
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_floodmap(void)
{
    //TODO
    QString file_autoopen;
    QAction *action = dynamic_cast<QAction *>(sender());
    if(action)
    {
        file_autoopen = action->property("name").toString();
    }

    FloodMapBox *w = new FloodMapBox(file_autoopen, this);
    if(w)
    {
        w->setObjectName("FloodMapBox");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_FLOODMAP);
        main_tab->setCurrentWidget(sa);

        connect(this,   SIGNAL(update_metadata()),  w,  SLOT(refresh_metadata()));
        connect(w,  SIGNAL(signal_add_metadata_file()), this,   SLOT(f_add_metadata()));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_histogram_consilience(void)
{
    //TODO
    QString file_autoopen;
    QAction *action = dynamic_cast<QAction *>(sender());
    if(action)
    {
        file_autoopen = action->property("name").toString();
    }

    Histogram_consilience *w = new Histogram_consilience(file_autoopen, this);
    if(w)
    {
        w->setObjectName("Histogram_consilience");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_HISTOGRAM_CONSULIENCE);
        main_tab->setCurrentWidget(sa);

        connect(this,   SIGNAL(update_metadata()),  w,  SLOT(refresh_metadata()));
        connect(w,  SIGNAL(signal_add_metadata_file()), this,   SLOT(f_add_metadata()));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_histogram_energy(void)
{
    //TODO
    QString file_autoopen;
    QAction *action = dynamic_cast<QAction *>(sender());
    if(action)
    {
        file_autoopen = action->property("name").toString();
    }

    Histogram_energy *w = new Histogram_energy(file_autoopen, this);
    if(w)
    {
        w->setObjectName("Histogram_energy");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_HISTOGRAM_ENERGY);
        main_tab->setCurrentWidget(sa);

        connect(this,   SIGNAL(update_metadata()),  w,  SLOT(refresh_metadata()));
        connect(w,  SIGNAL(signal_add_metadata_file()), this,   SLOT(f_add_metadata()));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_demo(void)
{
    PET_demo *w = new PET_demo(NAME_DEMO, this);
    if(w)
    {
        w->setObjectName("PET_demo");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_DEMO);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_imitator_pet(void)
{
    Imitator_PET *w = new Imitator_PET(this);
    if(w)
    {
        w->setObjectName("Imitator_PET");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_IMITATOR_PET);
        main_tab->setCurrentWidget(sa);
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_imitator_brg(void)
{
    Imitator_BRG_GUI *w = new Imitator_BRG_GUI(this);
    if(w)
    {
        w->setObjectName("Imitator_BRG");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_IMITATOR_BRG);
        main_tab->setCurrentWidget(sa);
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_mkt(void)
{
    MKT_box *w = new MKT_box(this);
    if(w)
    {
        w->setObjectName("MKT_box");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_MKT);
        main_tab->setCurrentWidget(sa);
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_similator(void)
{
    AD9106_Box *w = new AD9106_Box(this);
    if(w)
    {
        w->setObjectName("Simulator");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_SIMULATOR);
        main_tab->setCurrentWidget(sa);
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_creator(void)
{
    CreatorBox *w = new CreatorBox(this);
    if(w)
    {
        w->setObjectName("CreatorBox");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_CREATOR);
        main_tab->setCurrentWidget(sa);
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_viewer(void)
{
    //TODO
    QString file_autoopen;
    QAction *action = dynamic_cast<QAction *>(sender());
    if(action)
    {
        file_autoopen = action->property("name").toString();
    }

    ViewerBox *w = new ViewerBox(file_autoopen, this);
    if(w)
    {
        w->setObjectName("ViewerBox");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_VIEWER);
        main_tab->setCurrentWidget(sa);

        connect(this,   SIGNAL(update_metadata()),
                w,      SLOT(refresh_metadata()));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_calibration(void)
{
    FindPetBox *f = new FindPetBox(PET_ADDR_BROADCAST, this);
    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }
    Calibration *w = new Calibration(f->get_current_pet_config(), this);
    if(w)
    {
        w->setObjectName("Calibration");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_CALIBRATION);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_update_firmware(void)
{
    FindPetBox *f = new FindPetBox(PET_ADDR_ROOT, this);

    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }
    UpdateFirmware *w = new UpdateFirmware(f->get_current_pet_config(), this);
    if(w)
    {
        w->setObjectName("UpdateFirmware");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_UPDATE_FIRMWARE);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_test_1(void)
{
    FindPetBox *f = new FindPetBox(PET_ADDR_BROADCAST, this);
    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }

    TestBox_1 *w = new TestBox_1(f->get_current_pet_config(), this);
    if(w)
    {
        w->setObjectName("TestBox_1");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_TEST_1);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_test_2(void)
{
    TestBox_2 *w = new TestBox_2(this);
    if(w)
    {
        w->setObjectName("TestBox_2");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_TEST_2);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_oscilloscope(void)
{
    FindPetBox *f = new FindPetBox(PET_ADDR_BROADCAST, this);

    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }

    OscilloscopeBox *w = new OscilloscopeBox(f->get_current_pet_config(), this);
    if(w)
    {
        w->setObjectName("OscilloscopeBox");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_OSCILLOSCOPE);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_options(void)
{
    FindPetBox *f = new FindPetBox(PET_ADDR_ROOT, this);   //TODO
    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }

    OptionBox *w = new OptionBox(f->get_current_pet_config(), this);
    if(w)
    {
        w->setObjectName("OptionBox");
        QScrollArea *sa = new QScrollArea(this);
        sa->setWidgetResizable(true);
        sa->setWidget(w);

        main_tab->addTab(sa, NAME_OPTIONS);
        main_tab->setCurrentWidget(sa);

        connect(w,  SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
        connect(w,  SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("cannot create class");
    }
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool MainWindow::f_delete_metadata(QString name)
{
    QStringList sl;
    bool ok = false;
    QFileInfo fi;
    sl.clear();
    fi.setFile(name);
    ok = md->find_data(fi.baseName(), &sl);
    if(ok)
    {
        emit debug("-----");
        foreach (QString name, sl)
        {
            emit debug(name);
        }
        emit debug("-----");
    }
    //---
    int btn = MyWidget::messagebox_question("Удаление записей",
                                            QString("Удалить %1 из метаданных").arg(name));
    if(btn == QMessageBox::Yes)
    {
        if(sl.count())
        {
            ok = md->delete_data(fi.baseName(), name);
            if(ok)
            {
                md->load_data(root);
            }
            else
            {
                MyWidget::messagebox_critical("Ошибка", "Ошибка базы данных");
            }
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainWindow::tv_main_clicked(QTreeWidgetItem *item, int)
{
    emit debug("tv_main_clicked");

    int id = item->data(0, Qt::UserRole).toInt();
    bool ok = false;
    switch(id)
    {
    case ID_SCAN_CONTROL:           f_scan_control();   break;
    case ID_SCAN_HARD:              f_scan_hard();      break;
    case ID_OSCILLOSCOPE:           f_oscilloscope();   break;
    case ID_FLOODMAP:               f_floodmap();       break;
    case ID_HISTOGRAM_CONSULIENCE:  f_histogram_consilience();  break;
    case ID_HISTOGRAM_ENERGY:       f_histogram_energy();   break;
    case ID_OPTIONS:                f_options();        break;
    case ID_DEMO:                   f_demo();           break;
    case ID_IMITATOR_PET:           f_imitator_pet();   break;
    case ID_IMITATOR_BRG:           f_imitator_brg();   break;
    case ID_CREATOR:                f_creator();        break;
    case ID_VIEWER:                 f_viewer();         break;
    case ID_CALIBRATION:            f_calibration();    break;
    case ID_TEST_1:                 f_test_1();         break;
    case ID_TEST_2:                 f_test_2();         break;
    case ID_MKT:                    f_mkt();            break;
    case ID_SIMULATOR:              f_similator();      break;
    case ID_UPDATE_FIRMWARE:        f_update_firmware();    break;

    case ID_METADATA_APPEND:
        if(md)
        {
            ok = md->f_add_metadata(); //ID_METADATA_APPEND
            if(ok)
            {
                emit update_metadata();
            }
        }
        break;

    case ID_METADATA_DELETE:
        f_viewer();
        break;

    default:
        if(id)
        {
            emit error(QString("unknown ID %1").arg(id));
        }
        break;
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_help(void)
{
    HelpBrowser::showPage("qrc:/index.html", false);
}
//--------------------------------------------------------------------------------
void MainWindow::f_about_programm(void)
{
    AboutBox *about = new AboutBox(ORGNAME,
                                   APPNAME,
                                   QString("%1.%2.%3 (сборка %4)")
                                   .arg(VER_MAJOR)
                                   .arg(VER_MINOR)
                                   .arg(VER_PATCH)
                                   .arg(VER_BUILD),
                                   tr("Author: Bikbao Rinat Zinorovich"));
    about->exec();
}
//--------------------------------------------------------------------------------
void MainWindow::add_file_menu(void)
{
    m_file   = menuBar()->addMenu(NAME_FILE);

    //---
    a_print = new QAction(this);
    a_print->setText(tr("Печать"));
    a_print->setToolTip(tr("Печать"));
    a_print->setStatusTip(tr("Печать"));
    a_print->setIcon(QIcon(":/images/agt_print.png"));
    connect(a_print, SIGNAL(triggered(bool)), this, SLOT(f_print()));

    a_petcat_options = new QAction(this);
    a_petcat_options->setText(tr("Настройки программы"));
    a_petcat_options->setToolTip(tr("Настройки программы"));
    a_petcat_options->setStatusTip(tr("Настройки программы"));
    a_petcat_options->setIcon(QIcon(NAME_RADIATION_ICON));
    connect(a_petcat_options, SIGNAL(triggered(bool)), this, SLOT(f_petcat_options()));

    a_exit = new QAction(this);
    a_exit->setText(tr("Выход"));
    a_exit->setToolTip(tr("Выход из программы"));
    a_exit->setStatusTip(tr("Выход из программы"));
    a_exit->setIcon(QIcon(":/images/exit.png"));
    a_exit->setShortcuts(QKeySequence::Quit);
    connect(a_exit, SIGNAL(triggered(bool)), qApp, SLOT(closeAllWindows()));
    //---
    a_print->setDisabled(true);
    //---
    m_file->addAction(a_print);
    m_file->addSeparator();
    m_file->addAction(a_petcat_options);
    m_file->addSeparator();
    m_file->addAction(a_exit);

    if(mainToolBar)
    {
        //mainToolBar->addAction(a_print);
        //mainToolBar->addAction(a_exit);
    }
}
//--------------------------------------------------------------------------------
void MainWindow::add_analize_menu(void)
{
    m_analize = menuBar()->addMenu(NAME_ANALIZE);
    //---
    a_floodmap = new QAction(this);
    a_floodmap->setText(NAME_FLOODMAP);
    a_floodmap->setToolTip(NAME_FLOODMAP);
    a_floodmap->setStatusTip(NAME_FLOODMAP);
    a_floodmap->setIcon(QIcon(NAME_FLOODMAP_ICON));
    connect(a_floodmap, SIGNAL(triggered(bool)), this, SLOT(f_floodmap()));

    a_oscilloscope = new QAction(this);
    a_oscilloscope->setText(NAME_OSCILLOSCOPE);
    a_oscilloscope->setToolTip(NAME_OSCILLOSCOPE);
    a_oscilloscope->setStatusTip(NAME_OSCILLOSCOPE);
    a_oscilloscope->setIcon(QIcon(NAME_OSCILLOSCOPE_ICON));
    connect(a_oscilloscope, SIGNAL(triggered(bool)), this, SLOT(f_oscilloscope()));

    //---
    a_histogram_consulience = new QAction(this);
    a_histogram_consulience->setText(NAME_HISTOGRAM_CONSULIENCE);
    a_histogram_consulience->setToolTip(NAME_HISTOGRAM_CONSULIENCE);
    a_histogram_consulience->setStatusTip(NAME_HISTOGRAM_CONSULIENCE);
    a_histogram_consulience->setIcon(QIcon(NAME_HISTOGRAM_CONSULIENCE_ICON));
    connect(a_histogram_consulience, SIGNAL(triggered(bool)), this, SLOT(f_histogram_consilience()));

    a_histogram_energy = new QAction(this);
    a_histogram_energy->setText(NAME_HISTOGRAM_ENERGY);
    a_histogram_energy->setToolTip(NAME_HISTOGRAM_ENERGY);
    a_histogram_energy->setStatusTip(NAME_HISTOGRAM_ENERGY);
    a_histogram_energy->setIcon(QIcon(NAME_HISTOGRAM_ENERGY_ICON));
    connect(a_histogram_energy, SIGNAL(triggered(bool)), this, SLOT(f_histogram_energy()));
    //---

    m_analize->addAction(a_oscilloscope);
    m_analize->addAction(a_floodmap);
    m_analize->addAction(a_histogram_consulience);
    m_analize->addAction(a_histogram_energy);

    if(mainToolBar)
    {
        //mainToolBar->addAction(a_floodmap);
        //mainToolBar->addAction(a_oscilloscope);
        //mainToolBar->addAction(a_histogram_consulience);
        //mainToolBar->addAction(a_histogram_energy);
        //mainToolBar->addAction(a_histogram_time);
    }
}
//--------------------------------------------------------------------------------
void MainWindow::add_tools_menu(void)
{
    m_tools = menuBar()->addMenu(NAME_TOOLS);
    //---

#ifdef SCAN_HARD
    a_scan_hard = new QAction(this);
    a_scan_hard->setText(NAME_SCAN_HARD);
    a_scan_hard->setToolTip(NAME_SCAN_HARD);
    a_scan_hard->setStatusTip(NAME_SCAN_HARD);
    a_scan_hard->setIcon(QIcon(NAME_SCAN_ICON));
    connect(a_scan_hard, SIGNAL(triggered(bool)), this, SLOT(f_scan_hard()));
#endif

#ifdef MKT
    a_mkt = new QAction(this);
    a_mkt->setText(NAME_MKT);
    a_mkt->setToolTip(NAME_MKT);
    a_mkt->setStatusTip(NAME_MKT);
    a_mkt->setIcon(QIcon(NAME_MKT));
    connect(a_mkt, SIGNAL(triggered(bool)), this, SLOT(f_mkt()));
#endif

#ifdef SIMULATOR
    a_similator = new QAction(this);
    a_similator->setText(NAME_SIMULATOR);
    a_similator->setToolTip(NAME_SIMULATOR);
    a_similator->setStatusTip(NAME_SIMULATOR);
    a_similator->setIcon(QIcon(NAME_SIMULATOR));
    connect(a_similator, SIGNAL(triggered(bool)), this, SLOT(f_similator()));
#endif

    a_options = new QAction(this);
    a_options->setText(NAME_OPTIONS);
    a_options->setToolTip(NAME_OPTIONS);
    a_options->setStatusTip(NAME_OPTIONS);
    a_options->setIcon(QIcon(":/images/exec.png"));
    connect(a_options, SIGNAL(triggered(bool)), this, SLOT(f_options()));

    m_tools->addSeparator();
#ifdef SCAN_HARD
    m_tools->addAction(a_scan_hard);
#endif
    m_tools->addSeparator();
#ifdef MKT
    m_tools->addAction(a_mkt);
#endif
#ifdef SIMULATOR
    m_tools->addAction(a_similator);
#endif
    m_tools->addSeparator();
    m_tools->addAction(a_options);

    if(mainToolBar)
    {
        //mainToolBar->addAction(a_pet_test);
        //mainToolBar->addAction(a_scan);
        mainToolBar->addAction(a_options);
    }
}
//--------------------------------------------------------------------------------
void MainWindow::add_demo_menu(void)
{
    m_demo = menuBar()->addMenu(NAME_DEMO);
    //---
    a_demo = new QAction(this);
    a_demo->setText(tr("Настройки ПЭТ-демо"));
    a_demo->setToolTip(tr("Настройки ПЭТ-демо"));
    a_demo->setStatusTip(tr("Настройки ПЭТ-демо"));
    a_demo->setIcon(QIcon(":/images/playlist.png"));
    connect(a_demo, SIGNAL(triggered(bool)), this, SLOT(f_demo()));
    //---
    m_demo->addAction(a_demo);
}
//--------------------------------------------------------------------------------
void MainWindow::add_windows_menu(void)
{
    m_windows = menuBar()->addMenu(tr("Окна"));
}
//--------------------------------------------------------------------------------
void MainWindow::add_help_menu(void)
{
    m_help = menuBar()->addMenu(tr("Помощь"));

    a_help = new QAction(this);
    a_help->setText(tr("Помошь"));
    a_help->setToolTip(tr("Помошь"));
    a_help->setStatusTip(tr("Помошь"));
    a_help->setIcon(QIcon(":/images/playlist.png"));
    connect(a_help, SIGNAL(triggered(bool)), this, SLOT(f_help()));

#ifndef QT_DEBUG
    a_help->setDisabled(true);
#endif

    a_about_programm = new QAction(this);
    a_about_programm->setIcon(QIcon(":/images/computer.png"));
    a_about_programm->setText(tr("О программе"));
    a_about_programm->setToolTip(tr("О программе..."));
    a_about_programm->setStatusTip(tr("О программе..."));
    connect(a_about_programm, SIGNAL(triggered(bool)), this, SLOT(f_about_programm()));

    //---
    m_help->addAction(a_help);
    m_help->addSeparator();
    m_help->addAction(a_about_programm);
    //---
    if(mainToolBar)
    {
        //mainToolBar->addAction(a_help);
        //mainToolBar->addSeparator();
        //mainToolBar->addAction(a_about_programm);
        //mainToolBar->addSeparator();
    }
}
//--------------------------------------------------------------------------------
void MainWindow::createActions(void)
{
    add_file_menu();
    //add_analize_menu();
    //add_tools_menu();
    //add_demo_menu();
    add_windows_menu();
    add_help_menu();
}
//--------------------------------------------------------------------------------
void MainWindow::createStatusBar(void)
{
    statusBar()->showMessage(tr("Готово"));
}
//--------------------------------------------------------------------------------
QToolButton *MainWindow::add_button(QToolBar *tool_bar,
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
    tool_button->setStatusTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainWindow::add_metadata_items(QTreeWidget *tv)
{
    md = new MetaData();
    Q_CHECK_PTR(md);

    connect(md, SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(md, SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    root = new QTreeWidgetItem(tv);
    Q_CHECK_PTR(root);
    root->setText(0,      NAME_METADATA);
    root->setToolTip(0,   NAME_METADATA);
    root->setStatusTip(0, NAME_METADATA);
    root->setData(0,    Qt::UserRole,   ID_METADATA);

    //---
    md->load_data(root);
    //---

    root->setExpanded(true);
    tv->addTopLevelItem(root);

    if(md)
    {
        md->set_root_item(root);
    }
}
//--------------------------------------------------------------------------------
void MainWindow::add_scan_items(QTreeWidget *tv)
{
    QTreeWidgetItem *ri_scan = new QTreeWidgetItem(tv);
    ri_scan->setText(0,         NAME_SCAN);
    ri_scan->setToolTip(0,      NAME_SCAN);
    ri_scan->setStatusTip(0,    NAME_SCAN);
    //---
    QTreeWidgetItem *ri_scan_control = new QTreeWidgetItem(ri_scan);
    //---
    ri_scan_control->setText(0,        NAME_SCAN_CONTROL);
    ri_scan_control->setToolTip(0,     NAME_SCAN_CONTROL);
    ri_scan_control->setStatusTip(0,   NAME_SCAN_CONTROL);
    ri_scan_control->setIcon(0,        QIcon(NAME_SCAN_CONTROL_ICON));
    ri_scan_control->setData(0,        Qt::UserRole,   ID_SCAN_CONTROL);
    //---
#ifdef SCAN_HARD
    QTreeWidgetItem *ri_scan_hard = new QTreeWidgetItem(ri_scan);
    //---
    ri_scan_hard->setText(0,        NAME_SCAN_HARD);
    ri_scan_hard->setToolTip(0,     NAME_SCAN_HARD);
    ri_scan_hard->setStatusTip(0,   NAME_SCAN_HARD);
    ri_scan_hard->setIcon(0,        QIcon(NAME_SCAN_ICON));
    ri_scan_hard->setData(0,        Qt::UserRole,   ID_SCAN_HARD);
#endif
    //---
    QTreeWidgetItem *i_oscilloscope = new QTreeWidgetItem(ri_scan);
    i_oscilloscope->setText(0, NAME_OSCILLOSCOPE);
    i_oscilloscope->setToolTip(0, NAME_OSCILLOSCOPE);
    i_oscilloscope->setStatusTip(0, NAME_OSCILLOSCOPE);
    i_oscilloscope->setIcon(0, QIcon(NAME_OSCILLOSCOPE_ICON));
    i_oscilloscope->setData(0,  Qt::UserRole,   ID_OSCILLOSCOPE);
    //---
    ri_scan->setExpanded(true);
    tv->addTopLevelItem(ri_scan);
}
//--------------------------------------------------------------------------------
void MainWindow::add_analize_items(QTreeWidget *tv)
{
    QTreeWidgetItem *ri_analize = new QTreeWidgetItem(tv);
    ri_analize->setText(0,      NAME_ANALIZE);
    ri_analize->setToolTip(0,   NAME_ANALIZE);
    ri_analize->setStatusTip(0, NAME_ANALIZE);
    //---
#ifdef VIEWER
    QTreeWidgetItem *i_viewer = new QTreeWidgetItem(ri_analize);
    i_viewer->setText(0, NAME_VIEWER);
    i_viewer->setToolTip(0, NAME_VIEWER);
    i_viewer->setStatusTip(0, NAME_VIEWER);
    i_viewer->setIcon(0, QIcon(NAME_VIEWER_ICON));
    i_viewer->setData(0,  Qt::UserRole,   ID_VIEWER);
#endif
    //---
    QTreeWidgetItem *i_floodmap = new QTreeWidgetItem(ri_analize);
    i_floodmap->setText(0, NAME_FLOODMAP);
    i_floodmap->setToolTip(0, NAME_FLOODMAP);
    i_floodmap->setStatusTip(0, NAME_FLOODMAP);
    i_floodmap->setIcon(0, QIcon(NAME_FLOODMAP_ICON));
    i_floodmap->setData(0,  Qt::UserRole,   ID_FLOODMAP);

    QTreeWidgetItem *i_histogram_consulience = new QTreeWidgetItem(ri_analize);
    i_histogram_consulience->setText(0, NAME_HISTOGRAM_CONSULIENCE);
    i_histogram_consulience->setToolTip(0, NAME_HISTOGRAM_CONSULIENCE);
    i_histogram_consulience->setStatusTip(0, NAME_HISTOGRAM_CONSULIENCE);
    i_histogram_consulience->setIcon(0, QIcon(NAME_HISTOGRAM_CONSULIENCE_ICON));
    i_histogram_consulience->setData(0,  Qt::UserRole,   ID_HISTOGRAM_CONSULIENCE);

    QTreeWidgetItem *i_histogram_energy = new QTreeWidgetItem(ri_analize);
    i_histogram_energy->setText(0, NAME_HISTOGRAM_ENERGY);
    i_histogram_energy->setToolTip(0, NAME_HISTOGRAM_ENERGY);
    i_histogram_energy->setStatusTip(0, NAME_HISTOGRAM_ENERGY);
    i_histogram_energy->setIcon(0, QIcon(NAME_HISTOGRAM_ENERGY_ICON));
    i_histogram_energy->setData(0,  Qt::UserRole,   ID_HISTOGRAM_ENERGY);

    ri_analize->setExpanded(true);
    tv->addTopLevelItem(ri_analize);
}
//--------------------------------------------------------------------------------
void MainWindow::add_options_items(QTreeWidget *tv)
{
    QTreeWidgetItem *ri_options = new QTreeWidgetItem(tv);
    ri_options->setText(0,      NAME_OPTIONS);
    ri_options->setToolTip(0,   NAME_OPTIONS);
    ri_options->setStatusTip(0, NAME_OPTIONS);
    //---
    QTreeWidgetItem *i_options = new QTreeWidgetItem(ri_options);
    i_options->setText(0, NAME_OPTIONS);
    i_options->setToolTip(0, NAME_OPTIONS);
    i_options->setStatusTip(0, NAME_OPTIONS);
    i_options->setIcon(0, QIcon(NAME_OPTIONS_ICON));
    i_options->setData(0, Qt::UserRole,   ID_OPTIONS);
    //---
    ri_options->setExpanded(true);
    tv->addTopLevelItem(ri_options);
}
//--------------------------------------------------------------------------------
void MainWindow::add_test_items(QTreeWidget *tv)
{
    QTreeWidgetItem *ri_test = new QTreeWidgetItem(tv);
    ri_test->setText(0,      NAME_TESTS);
    ri_test->setToolTip(0,   NAME_TESTS);
    ri_test->setStatusTip(0, NAME_TESTS);
    //---
#ifdef DEMO_PET
    QTreeWidgetItem *i_demo = new QTreeWidgetItem(ri_test);
    i_demo->setText(0, NAME_DEMO);
    i_demo->setToolTip(0, NAME_DEMO);
    i_demo->setStatusTip(0, NAME_DEMO);
    i_demo->setIcon(0, QIcon(NAME_DEMO_ICON));
    i_demo->setData(0,  Qt::UserRole,   ID_DEMO);
#endif
    //---
#ifdef IMITATOR_PET
    QTreeWidgetItem *i_imitator_PET = new QTreeWidgetItem(ri_test);
    i_imitator_PET->setText(0, NAME_IMITATOR_PET);
    i_imitator_PET->setToolTip(0, NAME_IMITATOR_PET);
    i_imitator_PET->setStatusTip(0, NAME_IMITATOR_PET);
    i_imitator_PET->setIcon(0, QIcon(NAME_IMITATOR_PET_ICON));
    i_imitator_PET->setData(0,  Qt::UserRole,   ID_IMITATOR_PET);
#endif

#ifdef IMITATOR_BRG
    QTreeWidgetItem *i_imitator_BRG = new QTreeWidgetItem(ri_test);
    i_imitator_BRG->setText(0, NAME_IMITATOR_BRG);
    i_imitator_BRG->setToolTip(0, NAME_IMITATOR_BRG);
    i_imitator_BRG->setStatusTip(0, NAME_IMITATOR_BRG);
    i_imitator_BRG->setIcon(0, QIcon(NAME_IMITATOR_BRG_ICON));
    i_imitator_BRG->setData(0,  Qt::UserRole,   ID_IMITATOR_BRG);
#endif


#ifdef CREATOR
    QTreeWidgetItem *i_creator = new QTreeWidgetItem(ri_test);
    i_creator->setText(0, NAME_CREATOR);
    i_creator->setToolTip(0, NAME_CREATOR);
    i_creator->setStatusTip(0, NAME_CREATOR);
    i_creator->setIcon(0, QIcon(NAME_CREATOR_ICON));
    i_creator->setData(0,  Qt::UserRole,   ID_CREATOR);
#endif

#ifdef CALIBRATION
    QTreeWidgetItem *i_calibration = new QTreeWidgetItem(ri_test);
    i_calibration->setText(0, NAME_CALIBRATION);
    i_calibration->setToolTip(0, NAME_CALIBRATION);
    i_calibration->setStatusTip(0, NAME_CALIBRATION);
    i_calibration->setIcon(0, QIcon(NAME_CALIBRATION_ICON));
    i_calibration->setData(0,  Qt::UserRole, ID_CALIBRATION);
#endif

#ifdef TEST_1
    QTreeWidgetItem *i_test_1 = new QTreeWidgetItem(ri_test);
    i_test_1->setText(0, NAME_TEST_1);
    i_test_1->setToolTip(0, NAME_TEST_1);
    i_test_1->setStatusTip(0, NAME_TEST_1);
    i_test_1->setIcon(0, QIcon(NAME_TEST_1_ICON));
    i_test_1->setData(0,  Qt::UserRole, ID_TEST_1);
#endif

#ifdef TEST_2
    QTreeWidgetItem *i_test_2 = new QTreeWidgetItem(ri_test);
    i_test_2->setText(0, NAME_TEST_2);
    i_test_2->setToolTip(0, NAME_TEST_2);
    i_test_2->setStatusTip(0, NAME_TEST_2);
    i_test_2->setIcon(0, QIcon(NAME_TEST_2_ICON));
    i_test_2->setData(0,  Qt::UserRole,   ID_TEST_2);
#endif
    //---
    ri_test->setExpanded(true);
    tv->addTopLevelItem(ri_test);
}
//--------------------------------------------------------------------------------
void MainWindow::add_other_items(QTreeWidget *tv)
{
#if (defined MKT) || (defined SIMULATOR) || (defined UPDATE_FIRMWARE)
    QTreeWidgetItem *ri_other = new QTreeWidgetItem(tv);
    ri_other->setText(0,      NAME_OTHER);
    ri_other->setToolTip(0,   NAME_OTHER);
    ri_other->setStatusTip(0, NAME_OTHER);
#endif

#ifdef MKT
    QTreeWidgetItem *i_mkt = new QTreeWidgetItem(ri_other);
    i_mkt->setText(0, NAME_MKT);
    i_mkt->setToolTip(0, NAME_MKT);
    i_mkt->setStatusTip(0, NAME_MKT);
    i_mkt->setIcon(0, QIcon(NAME_MKT_ICON));
    i_mkt->setData(0, Qt::UserRole,   ID_MKT);
#endif
#ifdef SIMULATOR
    QTreeWidgetItem *i_simulator = new QTreeWidgetItem(ri_other);
    i_simulator->setText(0, NAME_SIMULATOR);
    i_simulator->setToolTip(0, NAME_SIMULATOR);
    i_simulator->setStatusTip(0, NAME_SIMULATOR);
    i_simulator->setIcon(0, QIcon(NAME_SIMULATOR_ICON));
    i_simulator->setData(0, Qt::UserRole,   ID_SIMULATOR);
#endif
#ifdef UPDATE_FIRMWARE
    QTreeWidgetItem *i_update_firmware = new QTreeWidgetItem(ri_other);
    i_update_firmware->setText(0, NAME_UPDATE_FIRMWARE);
    i_update_firmware->setToolTip(0, NAME_UPDATE_FIRMWARE);
    i_update_firmware->setStatusTip(0, NAME_UPDATE_FIRMWARE);
    i_update_firmware->setIcon(0, QIcon(NAME_UPDATE_FIRMWARE_ICON));
    i_update_firmware->setData(0, Qt::UserRole,   ID_UPDATE_FIRMWARE);
#endif
    //---
#if (defined MKT) || (defined SIMULATOR) || (defined UPDATE_FIRMWARE)
    ri_other->setExpanded(true);
    tv->addTopLevelItem(ri_other);
#endif
    Q_UNUSED(tv);
}
//--------------------------------------------------------------------------------
void MainWindow::create_left_dock(void)
{
    tv_main = new QTreeWidget();
    tv_main->header()->hide();

    add_metadata_items(tv_main);
    add_scan_items(tv_main);
    add_analize_items(tv_main);
    add_options_items(tv_main);
    add_test_items(tv_main);
    add_other_items(tv_main);

    QDockWidget *dock = new QDockWidget(tr("Задачи"), this);
    m_windows->addAction(dock->toggleViewAction());
    dock->setObjectName("tasks");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(tv_main);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    tv_main->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tv_main, SIGNAL(customContextMenuRequested(QPoint)),        this, SLOT(popup(QPoint)));
    connect(tv_main, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),   this, SLOT(tv_main_clicked(QTreeWidgetItem*,int)));
}
//--------------------------------------------------------------------------------
void MainWindow::popup(QPoint point)
{
    //FIXME
    int id = tv_main->itemAt(point)->data(0, Qt::UserRole).toInt();
    if((id != ID_METADATA_APPEND) && (id != ID_METADATA_DELETE) && (id != ID_METADATA))
    {
        return;
    }

    QMenu *a_menu = new QMenu(NAME_ANALIZE);
    a_menu->setIcon(QIcon(NAME_RADIATION_ICON));

    QMenu *popup_menu = new QMenu(this);

    QAction *add_file_action = new QAction(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton),
                                           QObject::tr("Добавить файл"),
                                           this);
    QAction *del_file_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                           QObject::tr("Удалить файл"),
                                           this);

    //TODO
    QAction *a_viewer = new QAction(QIcon(NAME_VIEWER_ICON),
                                    NAME_VIEWER,
                                    this);
    QAction *a_floodmap = new QAction(QIcon(NAME_FLOODMAP_ICON),
                                      NAME_FLOODMAP,
                                      this);
    QAction *a_h_consulience = new QAction(QIcon(NAME_HISTOGRAM_CONSULIENCE_ICON),
                                           NAME_HISTOGRAM_CONSULIENCE,
                                           this);
    QAction *a_h_energy = new QAction(QIcon(NAME_HISTOGRAM_ENERGY_ICON),
                                      NAME_HISTOGRAM_ENERGY,
                                      this);

    a_viewer->setProperty("name", tv_main->itemAt(point)->text(0));
    a_viewer->setProperty("fullname", tv_main->itemAt(point)->toolTip(0));

    a_floodmap->setProperty("name", tv_main->itemAt(point)->text(0));
    a_floodmap->setProperty("fullname", tv_main->itemAt(point)->toolTip(0));

    a_h_consulience->setProperty("name", tv_main->itemAt(point)->text(0));
    a_h_consulience->setProperty("fullname", tv_main->itemAt(point)->toolTip(0));

    a_h_energy->setProperty("name", tv_main->itemAt(point)->text(0));
    a_h_energy->setProperty("fullname", tv_main->itemAt(point)->toolTip(0));


    add_file_action->setProperty("name", tv_main->itemAt(point)->text(0));
    add_file_action->setProperty("fullname", tv_main->itemAt(point)->toolTip(0));

    del_file_action->setProperty("name", tv_main->itemAt(point)->text(0));
    del_file_action->setProperty("fullname", tv_main->itemAt(point)->toolTip(0));

    switch(id)
    {
    case ID_METADATA:
        a_menu->setEnabled(false);

        a_viewer->setEnabled(false);
        a_floodmap->setEnabled(false);
        a_h_consulience->setEnabled(false);
        a_h_energy->setEnabled(false);

        add_file_action->setEnabled(true);
        del_file_action->setEnabled(false);
        break;

    case ID_METADATA_APPEND:
        a_menu->setEnabled(false);

        a_viewer->setEnabled(false);
        a_floodmap->setEnabled(false);
        a_h_consulience->setEnabled(false);
        a_h_energy->setEnabled(false);

        add_file_action->setEnabled(true);
        del_file_action->setEnabled(false);
        break;

    case ID_METADATA_DELETE:
        a_menu->setEnabled(true);

        a_viewer->setEnabled(true);
        a_floodmap->setEnabled(true);
        a_h_consulience->setEnabled(true);
        a_h_energy->setEnabled(true);

        add_file_action->setEnabled(false);
        del_file_action->setEnabled(true);
        break;

    default:
        break;
    }

    a_menu->addAction(a_viewer);
    a_menu->addAction(a_floodmap);
    a_menu->addAction(a_h_consulience);
    a_menu->addAction(a_h_energy);

    popup_menu->addMenu(a_menu);
    popup_menu->addAction(add_file_action);
    popup_menu->addAction(del_file_action);

    connect(add_file_action,  SIGNAL(triggered()), this, SLOT(f_add_metadata()));
    connect(del_file_action,  SIGNAL(triggered()), this, SLOT(f_del_metadata()));

    connect(a_viewer,           SIGNAL(triggered(bool)),    this,   SLOT(f_viewer()));
    connect(a_floodmap,         SIGNAL(triggered(bool)),    this,   SLOT(f_floodmap()));
    connect(a_h_consulience,    SIGNAL(triggered(bool)),    this,   SLOT(f_histogram_consilience()));
    connect(a_h_energy,         SIGNAL(triggered(bool)),    this,   SLOT(f_histogram_energy()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void MainWindow::f_add_metadata(void)
{
    if(md)
    {
        bool ok = md->f_add_metadata(); //f_add_metadata
        if(ok)
        {
            emit update_metadata();
        }
    }
}
//--------------------------------------------------------------------------------
void MainWindow::f_del_metadata(void)
{
    QAction *s = dynamic_cast<QAction *>(sender());
    if(s == nullptr)
    {
        return;
    }
    if(md)
    {
        bool ok = md->f_del_metadata(s->property("name").toString(),
                                     s->property("fullname").toString());
        if(ok)
        {
            emit update_metadata();
        }
    }
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void MainWindow::create_central_dock(void)
{
    main_tab = new QTabWidget;
    main_tab->setAttribute(Qt::WA_DeleteOnClose);
    main_tab->setMovable(true);
    main_tab->setTabsClosable(true);
    setCentralWidget(main_tab);

    connect(main_tab,   SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close(int)));
}
//--------------------------------------------------------------------------------
void MainWindow::create_log_dock(void)
{
    LogDock *ld = new LogDock("Протокол работы", this);
    m_windows->addAction(ld->toggleViewAction());
    ld->setObjectName("log");

    connect(this,   SIGNAL(info(QString)),      ld, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)),     ld, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)),     ld, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(message(QString)),   ld, SLOT(messageLog(QString)));

    addDockWidget(Qt::BottomDockWidgetArea, ld);
}
//--------------------------------------------------------------------------------
#ifdef NET_DEBUG
void MainWindow::create_net_dock(void)
{
    LogDock *ld = new LogDock("Протокол сетевого обмена", this);
    m_windows->addAction(ld->toggleViewAction());
    ld->setObjectName("net");

    connect(this,   SIGNAL(send_net_log(QString)),      ld, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(receive_net_log(QString)),   ld, SLOT(errorLog(QString)));

    addDockWidget(Qt::BottomDockWidgetArea, ld);
}
#endif
//--------------------------------------------------------------------------------
void MainWindow::tab_close(int index)
{
    QWidget *w = main_tab->widget(index);
    if(w == nullptr)
    {
        return;
    }

    QString title = "Закрытие окна";
    QString text = "Вы уверены, что хотите закрыть окно?";
    //---
    //TODO
    QScrollArea *sa = dynamic_cast<QScrollArea *>(w);
    if(sa)
    {
        ScanBox_hard *sh_obj = dynamic_cast<ScanBox_hard *>(sa->widget());
        if(sh_obj) sh_obj->get_answer(&text);

        PET_demo *pd_obj = dynamic_cast<PET_demo *>(sa->widget());
        if(pd_obj) pd_obj->get_answer(&text);
    }
    //---

    int btn = MyWidget::messagebox_question(title, text);

    if(btn != MyWidget::Yes)
    {
        return;
    }

    w->disconnect();
    w->close();
    w->deleteLater();
    main_tab->removeTab(index);
}
//--------------------------------------------------------------------------------
void MainWindow::readSettings(void)
{
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    restoreGeometry(settings->value("geometry").toByteArray());
    restoreState(settings->value("windowState").toByteArray());
}
//--------------------------------------------------------------------------------
void MainWindow::writeSettings(void)
{
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    settings->setValue("geometry",      saveGeometry());
    settings->setValue("windowState",   saveState());
}
//--------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    int btn = MyWidget::messagebox_question("Выход", "Выйти из программы?");

    if(btn == MyWidget::Yes)
    {
        //---
        while (main_tab->count())
        {
            QScrollArea *w = dynamic_cast<QScrollArea *>(main_tab->widget(0));
            if(w)
            {
                QWidget *child = w->widget();
                if(child)
                {
                    child->disconnect();
                    bool ok = child->close();
                    if(ok)
                    {
                        qDebug() << "close" << child->objectName();
                        child->deleteLater();
                        main_tab->removeTab(0);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        //---

        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
//--------------------------------------------------------------------------------
