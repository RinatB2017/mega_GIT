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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QStackedWidget;
class QToolButton;
class QGridLayout;
class QTabWidget;
class QLineEdit;
class Notebook;
class QToolBar;
class QFrame;

class QWebEngineView;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void set_address(void);
    void back(void);

    void test(void);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    bool is_blocked;
    QLineEdit *site_address;
    QWebEngineView *browser_site;
    QWebEngineView *browser_translate_en_ru;
    QWebEngineView *browser_translate_ru_en;

    QGridLayout *grid1;
    QGridLayout *grid2;
    QGridLayout *grid3;
    QGridLayout *grid4;

    QFrame *notebook_frame;
    QTabWidget *tab;

    Notebook *note;

    void init(void);

    void createTestBar(void);
    void prepare_browsers(void);
    void prepare_grids(void);
    void prepare_tab(void);
    void prepare_notebook(void);

    void create_widgets(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
