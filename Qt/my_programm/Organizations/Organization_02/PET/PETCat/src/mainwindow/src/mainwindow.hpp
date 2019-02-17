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
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
class MetaData;
class LogDock;
//--------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow* parent = nullptr);
    ~MainWindow(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void signal_is_shows_info(bool);
    void signal_is_shows_debug(bool);
    void signal_is_shows_error(bool);
    void signal_is_shows_trace(bool);

    void send_net_log(const QString &);
    void receive_net_log(const QString &);

    void update_metadata(void);

public slots:
    void f_print(void);
    void f_help(void);
    void f_about_programm(void);
    void f_petcat_options(int index = ID_OPTIONS);

    void f_scan_control(void);
    void f_scan_hard(void);
    void f_floodmap(void);
    void f_oscilloscope(void);
    void f_histogram_consilience(void);
    void f_histogram_energy(void);
    void f_mkt(void);
    void f_similator(void);
    void f_calibration(void);
    void f_test_1(void);
    void f_test_2(void);
    void f_options(void);
    void f_demo(void);
    void f_imitator_pet(void);
    void f_imitator_brg(void);
    void f_creator(void);
    void f_viewer(void);

    void f_update_firmware(void);

    void popup(QPoint point);
    void f_add_metadata(void);
    void f_del_metadata(void);

    void tv_main_clicked(QTreeWidgetItem *item, int);
    void tab_close(int index);

    void slot_send_net_log(QString data);
    void slot_receive_net_log(QString data);

private:
#ifndef ONLY_ENGLISH
    QTranslator *appTranslator;
    QTranslator *sysTranslator;
#endif

    QMenu   *m_file;
    QMenu   *m_analize;
    QMenu   *m_tools;
    QMenu   *m_demo;
    QMenu   *m_windows;
    QMenu   *m_help;

    //---
    QMenu   *m_task;
    QMenu   *m_scan_menu;
    QMenu   *m_options;
    QMenu   *m_tests;
    QMenu   *m_other;
    QAction *a_scan;
    QAction *a_scan_control;
    QAction *a_scan_hard;
    QAction *a_oscilloscope;
    QAction *a_viewer;
    QAction *a_floodmap;
    QAction *a_histogram_consulience;
    QAction *a_histogram_energy;
    QAction *a_options;
    QAction *a_demo;
    QAction *a_imitator;
    QAction *a_calibration;
    QAction *a_test_1;
    QAction *a_test_2;
    QAction *a_update_firmware;
    //---

    QToolBar *mainToolBar;

    QAction *a_exit;

    QAction *a_print;
    QAction *a_help;
    QAction *a_about_programm;
    QAction *a_petcat_options;

    QAction *a_mkt;
    QAction *a_similator;
    QAction *a_creator;

    QTabWidget *main_tab;

    QTreeWidget *tv_main;

    LogDock *ld;
    LogDock *nld;

    MetaData *md;
    bool f_delete_metadata(QString name);

    void createActions(void);

    void add_file_menu(void);
    void add_tools_menu(void);
    void add_task_menu(void);
    void add_windows_menu(void);
    void add_help_menu(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    //---
    QTreeWidgetItem *root = 0;
    //---
    void add_metadata_items(QTreeWidget *tv);
    void add_scan_items(QTreeWidget *tv);
    void add_analize_items(QTreeWidget *tv);
    void add_options_items(QTreeWidget *tv);
    void add_test_items(QTreeWidget *tv);
    void add_other_items(QTreeWidget *tv);
    //---

    void create_left_dock(void);
    void create_central_dock(void);
    void create_log_dock(void);
#ifdef NET_DEBUG
    void create_net_dock(void);
#endif

    void load_translations(void);

    void readSettings(void);
    void writeSettings(void);

protected:
    void closeEvent(QCloseEvent *event);

};
//--------------------------------------------------------------------------------
#endif
