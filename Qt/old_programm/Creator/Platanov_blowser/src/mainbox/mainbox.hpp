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
class PersistentCookieJar;
class QProgressBar;
class QToolButton;
class QPushButton;
class QLineEdit;
class QToolBar;
class WebView;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent = 0);
    ~MainBox();

private slots:
    void run(void);
    void web_started(void);
    void web_finished(bool state);

    void slot_livejournal(void);
    void slot_odnoklassniki(void);
    void slot_twitter(void);
    void slot_facebook(void);
    void slot_platanov(void);

private:
    Ui::MainBox *ui = 0;
    bool is_loaded = false;

    WebView *browser = 0;
    QProgressBar *progressBar = 0;
    QLineEdit *address = 0;

    QPushButton *btn_livejournal = 0;
    QPushButton *btn_odnoklassniki = 0;
    QPushButton *btn_twitter = 0;
    QPushButton *btn_facebook = 0;
    QPushButton *btn_platanov = 0;

    void init(void);

    void createTestBar(void);

    void load_setting(void);
    void save_setting(void);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
