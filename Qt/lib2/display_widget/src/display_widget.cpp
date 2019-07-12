/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "display_widget.hpp"
#include "myfiledialog.hpp"
//--------------------------------------------------------------------------------
Display_widget::Display_widget(QWidget *parent)
    : MyWidget(parent)
{
    max_x = SIZE_X;
    max_y = SIZE_Y;

    d_width  = max_x * led_width;
    d_height = max_y * led_height;

    setFixedSize(d_width, d_height);
    update();
}
//--------------------------------------------------------------------------------
void Display_widget::set_color(int x, int y, QColor color)
{
#ifdef ROTATE_DISPLAY
    int index = x*cnt_y + y;
#else
    int index = y*max_x + x;
#endif
    if(index > MAX_BUF)
    {
        return;
    }
    leds[index] = color;
}
//--------------------------------------------------------------------------------
QColor Display_widget::get_color(int x, int y)
{
#ifdef ROTATE_DISPLAY
    int index = x*cnt_y + y;
#else
    int index = y*max_x + x;
#endif
    if(index > MAX_BUF)
    {
        return Qt::black;
    }
    return leds[index];
}
//--------------------------------------------------------------------------------
bool Display_widget::set_size_led(int value)
{
    if(value < MIN_SIZE_LED)
    {
        emit error("size_led < MIN_SIZE_LED");
        return false;
    }
    if(value > MAX_SIZE_LED)
    {
        emit error("size_led too large");
        return false;
    }

    led_width  = value;
    led_height = value;

    return true;
}
//--------------------------------------------------------------------------------
int Display_widget::get_size_x(void)
{
    return max_x;
}
//--------------------------------------------------------------------------------
int Display_widget::get_size_y(void)
{
    return max_y;
}
//--------------------------------------------------------------------------------
void Display_widget::load_image(void)
{
    MyFileDialog *dlg = new MyFileDialog("dlg_temp", "dlg_temp");
    dlg->setNameFilter("Графические фвйлы (*.jpg)");
    dlg->setDefaultSuffix("jpg");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QPixmap pixmap(files.at(0));

        QPixmap new_pixmap = pixmap.scaled(get_size_x(),
                                           get_size_y(),
                                           Qt::KeepAspectRatio);
        QImage image = new_pixmap.toImage();

        int w = new_pixmap.width();
        int h = new_pixmap.height();

        emit info(QString("w %1").arg(w));
        emit info(QString("h %1").arg(h));

        for(int y=0; y<h; y++)
        {
            for(int x=0; x<w; x++)
            {
                set_color(x, y, image.pixelColor(x, y));
            }
        }
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void Display_widget::save_image(void)
{
    QImage *image = new QImage(max_x, max_y, QImage::Format_RGBA8888);
    //QImage *image = new QImage(cnt_x, cnt_y, QImage::Format_RGB888);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            QColor color = get_color(x, max_y - 1 - y);
            image->setPixelColor(x, y, color);
        }
    }

#if 0
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(*image));
    label->show();
#else
    MyFileDialog *dlg = nullptr;

    dlg = new MyFileDialog("display_widget", "display_widget");
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter("Графические файлы");
    dlg->setNameFilters(QStringList()
                        << "*.png"
                        << "*.jpg"
                        << "*.bmp"
                        << "*.gif");
    dlg->setDefaultSuffix("png");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    //dlg->setDirectory(".");
    dlg->selectFile("picture");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        bool ok = image->save(filename);
        if(ok)
        {
            messagebox_info("Info", QString("Файл %1 успешно создан").arg(filename));
        }
        else
        {
            messagebox_critical("Error", QString("Файл %1 не удалось создать").arg(filename));
        }
    }
    dlg->deleteLater();
    if(image != nullptr)
    {
        delete image;
    }
#endif
}
//--------------------------------------------------------------------------------
void Display_widget::clear(void)
{
    for(int n=0; n<MAX_BUF; n++)
    {
        leds[n] = Qt::black;
    }
    update();
}
//--------------------------------------------------------------------------------
void Display_widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(led_color));
    painter.drawRect(0, 0, width(), height());

    painter.setPen(border_color);
    for(int y=0; y<max_y; y++)
    {
        painter.drawLine(0, y * led_height, width(), y * led_height);
    }
    for(int x=0; x<max_x; x++)
    {
        painter.drawLine(x * led_width, 0, x * led_width, height());
    }

    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
#ifdef ROTATE_DISPLAY
            int index = show_begin_address + x*cnt_y + (cnt_y - 1 - y);
#else
            int index = (y*max_x + x);
#endif
            painter.setBrush(QBrush(leds[index]));
            painter.setBrush(QBrush(get_color(x, y)));
            painter.drawRect(x*led_width,
                             y*led_height,
                             led_width,
                             led_height);
        }
    }
}
//--------------------------------------------------------------------------------
void Display_widget::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Display_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Display_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Display_widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
