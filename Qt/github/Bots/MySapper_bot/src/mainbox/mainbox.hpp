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
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include </usr/include/opencv2/core/core.hpp>
#include </usr/include/opencv2/objdetect/objdetect.hpp>
#include </usr/include/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv2/highgui/highgui.hpp>
#include </usr/include/opencv2/opencv.hpp>
//--------------------------------------------------------------------------------
namespace Ui {
class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QTextEdit;
class QPushButton;
class QToolButton;
class QToolBar;
class QComboBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    bool run_kmines(void);
    bool find_circles(void);
    bool find_rectangles(void);
    bool find_contours(void);
    bool compare_pic(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    void init(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);

    void mouse_click(unsigned int button, QPoint pos);
    void mouse_release(unsigned int button);
    void mouse_move_to(QPoint pos);
    bool find_window(const QString programm_title, int *x, int *y, int *width, int *heigth);

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    int get_corr_x1(QPixmap *pix);
    int get_corr_y1(QPixmap *pix);
    int get_corr_x2(QPixmap *pix);
    int get_corr_y2(QPixmap *pix);

    IplImage *QImageToIplImage(const QImage *qImage, int *correct_width);
    IplImage *QImageToIplImage2(QImage qImage);
    QImage *IplImageToQImage(const IplImage *iplImage,
                             uchar **data,
                             double mini,
                             double maxi);

    bool analize(QPixmap *pixmap,
                 int inc_x,
                 int inc_y);
    QString get_digit(QPixmap pixmap, int x, int y);
    void get_color(QRgb rgb,
                   int *r,
                   int *g,
                   int *b);

protected:
    virtual void changeEvent(QEvent *event);
    virtual bool eventFilter(QObject*, QEvent* event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
