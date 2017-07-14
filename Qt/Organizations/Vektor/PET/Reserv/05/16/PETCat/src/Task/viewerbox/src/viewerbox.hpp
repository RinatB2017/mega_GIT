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
#ifndef VIEWERBOX_HPP
#define VIEWERBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFile>
//--------------------------------------------------------------------------------
namespace Ui {
    class ViewerBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QSpinBox;
//--------------------------------------------------------------------------------
#include "mycombobox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
typedef struct event_src
{
    uint8_t src;
    quint32 old_ts;
    quint32 cnt_ts;
} event_src_t;
//--------------------------------------------------------------------------------
class ViewerBox : public MyWidget
{
    Q_OBJECT

public:
    explicit ViewerBox(QString file_auto_open, QWidget *parent);
    ~ViewerBox();

public slots:
    void refresh_metadata(void);

private slots:
    void show_second(void);
    void show_event_dat3(void);

    void show_event(int pos, pet_event event);

private:
    Ui::ViewerBox *ui = 0;

    QList<event_src_t> l_event_src;

    MyComboBox *cb_metadata = 0;
    bool convert_metadata(QString name, QString *result);

    void init(void);
    void init_widgets(void);

    void add_src(uint8_t src, quint32 ts);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
