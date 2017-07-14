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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class GrapherBox;
class Multimeter_frame;
class Multimeter_V786_2;
//--------------------------------------------------------------------------------
class MainBox : public QWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private slots:
    void test(void);
    void find_device(void);
    void log(const QString &text);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;

    GrapherBox *grapher;
    Multimeter_V786_2 *multimeter;

    Multimeter_frame *frame_00;
    Multimeter_frame *frame_01;
    Multimeter_frame *frame_02;
    Multimeter_frame *frame_03;
    Multimeter_frame *frame_04;
    Multimeter_frame *frame_05;
    Multimeter_frame *frame_06;
    Multimeter_frame *frame_07;
    Multimeter_frame *frame_08;
    Multimeter_frame *frame_09;
    Multimeter_frame *frame_10;
    Multimeter_frame *frame_11;
    Multimeter_frame *frame_12;
    Multimeter_frame *frame_13;
    Multimeter_frame *frame_14;
    Multimeter_frame *frame_15;

    int curve_00;
    int curve_01;
    int curve_02;
    int curve_03;
    int curve_04;
    int curve_05;
    int curve_06;
    int curve_07;
    int curve_08;
    int curve_09;
    int curve_10;
    int curve_11;
    int curve_12;
    int curve_13;
    int curve_14;
    int curve_15;

    void init(void);
    void connect_log(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);
    void block_interface(bool state);

protected:
    virtual void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
