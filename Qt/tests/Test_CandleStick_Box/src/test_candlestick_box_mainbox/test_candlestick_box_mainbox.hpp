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
#include <QtCharts/QCandlestickSet>
QT_CHARTS_USE_NAMESPACE

#include "qcandlestickset.h"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;

class CandleStick_Box;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

private slots:
    void load(void);
    void save(void);
    void test(void);

    void started(void);
    void read_data(void);
    void read_error(void);
    void finished(int state);
    void process_error(QProcess::ProcessError err);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;
    QWidget *parent = 0;
    int index = 0;

    QList<CandleStick_Box *> plot_tickets;

#ifdef GRAPHER
    GrapherBox *grapher = 0;
#endif

    void init(void);

    void createTestBar(void);
    void create_test_currency(void);
    void create_plot_currency(void);

    bool create_set(QStringList sl, QCandlestickSet *set);

    QVector<QPointF> circle(float x, float y, float r);
    QVector<QLine> circle_line(float x, float y, float r);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
