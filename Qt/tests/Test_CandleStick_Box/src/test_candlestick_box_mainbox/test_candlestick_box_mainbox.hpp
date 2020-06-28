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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
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
class CandleStick_Box;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

private slots:
    void load(void);
    void save(void);
    void test(void);
    void test2(void);

    void started(void);
    void read_data(void);
    void read_error(void);
    void finished(int state);
    void process_error(QProcess::ProcessError err);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;
    QWidget *parent;
    int index = 0;

    QList<CandleStick_Box *> plot_tickets;
    QPointer<QProcess> process;

#ifdef GRAPHER
    GrapherBox *grapher;
#endif

    void init(void);

    void createTestBar(void);
    void create_test_currency(void);
    void create_plot_currency(void);

    bool create_set(QStringList sl, QCandlestickSet *set);

    QVector<QPointF> circle(qreal x, qreal y, qreal r);
    QVector<QLine> circle_line(float x, float y, float r);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
