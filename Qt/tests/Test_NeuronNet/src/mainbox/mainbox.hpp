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
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#pragma pack(push, 1)

typedef struct labels
{
    int32_t magic_number;
    int32_t numbers_of_items;
} labels_t;

typedef struct images
{
    int32_t magic_number;
    int32_t numbers_of_images;
    int32_t numbers_of_rows;
    int32_t numbers_of_columns;
} images_t;

typedef struct data_NMIST
{
    uint8_t     label;
    QByteArray  image;
} data_NMIST_t;

#pragma pack(pop)
//--------------------------------------------------------------------------------
class MySplashScreen;
class QTableWidget;
class QTextEdit;
class QToolButton;
class QToolBar;
class QComboBox;
class QLCDNumber;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

    void test_ptr(QWidget *ptr);

signals:
    void block_widget(bool);

private slots:
    void choice_test(void);
    void test_0(void);
    void test_1(void);
    void test_2(void);
    void test_3(void);
    void test_4(void);
    void test_5(void);

    void click(int row, int col);
    void show_image(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };
    struct CMD
    {
        int cmd;
        QString cmd_text;
        void (MainBox::*func)(void);
    };
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    QTableWidget *display = 0;
    QLCDNumber *display_number = 0;

    QList<data_NMIST> test_data;

    void init(void);
    void init_widgets(void);

    void createTestBar(void);

    bool get_label(QString filename,
                   int num_label,
                   int *label);
    bool get_label_param(QString filename,
                         int *numbers_of_items);

    bool get_image(QString filename,
                   int num_image,
                   QImage *image);
    bool get_image_param(QString filename,
                         int *numbers_of_images,
                         int *numbers_of_rows,
                         int *numbers_of_columns);
    bool get_image_data(QString filename,
                        int num_image,
                        QByteArray *data);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
