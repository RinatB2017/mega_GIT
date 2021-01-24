/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#ifndef OCV_WIDGET_HPP
#define OCV_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class OCV_widget;
}
//--------------------------------------------------------------------------------
class OCV_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit OCV_widget(QWidget *parent = nullptr);
    ~OCV_widget();

signals:
    bool f_tap(int pos_x, int pos_y);

public slots:
    void get_pixmap(QPixmap pixmap);

private slots:
    void refreshHSV(void);

private:
    Ui::OCV_widget *ui;

    Mat mOrigImage;
    QImage image;

    void init(void);

    void onLoad(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // OCV_WIDGET_HPP
