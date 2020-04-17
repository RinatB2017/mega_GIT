/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

private slots:
    void choice_test(void);
    bool test(void);

    void started(void);
    void finished(int result);
    void process_error(QProcess::ProcessError p_error);
    void readData(void);

    void f_create_screenshot(void);
    void f_show_screeshot(const QString &filename);
    void f_screen_tap(void);

    void onLoad(void);
    void refreshHSV(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    QProcess *myProcess = nullptr;
    bool f_busy = false;
    bool binary_data = false;
    int process_result = 0;
    QStringList sl_data;

    int s_width = 0;
    int s_height = 0;

    Mat mOrigImage;

    void init(void);
    void createTestBar(void);

    void run_program(const QString program,
                     const QStringList arguments);

    bool f_get_file_screeshot(void);
    bool f_get_screeshot(void);
    bool f_devices(void);
    bool f_tap(int pos_x, int pos_y);

    bool f_swipe(int x1, int y1, int x2, int y2, int delay);
    bool f_test_swipe_LR(void);
    bool f_test_swipe_RL(void);
    bool f_test_swipe_UD(void);
    bool f_test_swipe_DU(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
