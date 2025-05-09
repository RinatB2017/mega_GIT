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
#include <QColorDialog>
#include <QFileDialog>
#include <QProcess>

#include <QtCharts/QCandlestickSet>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QT_CHARTS_USE_NAMESPACE
#endif

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
    virtual ~MainBox();

private slots:
    void load(void);
    void save(void);
    void test(void);
    void test2(void);
    void test3(void);

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

    QPointer<QProcess> process;

#ifdef GRAPHER
    GrapherBox *grapher;
#endif

    void init(void);
    void init_process(void);
    void createTestBar(void);
    bool create_set(QStringList sl, QCandlestickSet *set);

    QVector<QPointF> circle(qreal x, qreal y, qreal r);
    QVector<QLine> circle_line(float x, float y, float r);

    void print_set(QCandlestickSet *set);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
