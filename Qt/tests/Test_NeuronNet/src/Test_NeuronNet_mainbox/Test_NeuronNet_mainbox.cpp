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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_Test_NeuronNet_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "Test_NeuronNet_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   define htons(n) (uint16_t)((((uint16_t) (n)) << 8) | (((uint16_t) (n)) >> 8))
#   define htonl(n) (uint32_t)((((uint32_t) (n)) << 16) | (((uint32_t) (n)) >> 16))
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();

#ifdef QT_DEBUG
    QList<QWidget *> all_obj = topLevelWidget()->findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
#endif
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

#if 0
    unsigned short x = 1;
    emit info(QString("%1").arg(((unsigned char *) &x) == 0 ? "big-endian" : "little-endian"));
#endif

    init_widgets();
#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif

    load_config();
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->sb_num_image->setRange(0, 60000-1);

    display = new QTableWidget(MAX_ROW, MAX_COL, this);
    connect(display,        SIGNAL(cellClicked(int,int)),   this,   SLOT(click(int,int)));
    connect(ui->btn_show,   SIGNAL(clicked(bool)),          this,   SLOT(show_image()));

    display->horizontalHeader()->setVisible(false);
    display->verticalHeader()->setVisible(false);

    display->horizontalHeader()->setDefaultSectionSize(SIZE_GRID);
    display->horizontalHeader()->setMinimumSectionSize(SIZE_GRID);

    display->verticalHeader()->setDefaultSectionSize(SIZE_GRID);
    display->verticalHeader()->setMinimumSectionSize(SIZE_GRID);

    display->setEditTriggers(QTableWidget::NoEditTriggers);
    display->setSelectionMode(QTableWidget::NoSelection);

    for (int i=0; i<display->rowCount(); i++)
    {
        for (int j=0; j<display->columnCount(); j++)
        {
            display->setItem(i,j, new QTableWidgetItem);
            display->item(i,j)->setBackgroundColor(Qt::white);
        }
    }
    //---
    display->item(0,0)->setBackgroundColor(Qt::red);
    display->item(0,MAX_COL-1)->setBackgroundColor(Qt::red);
    display->item(MAX_ROW-1,0)->setBackgroundColor(Qt::red);
    display->item(MAX_ROW-1,MAX_COL-1)->setBackgroundColor(Qt::red);
    //---

    display->adjustSize();
    emit info(QString("display %1 %2")
              .arg(display->width())
              .arg(display->height()));
    display->setFixedSize(MAX_COL * SIZE_GRID + 4,
                          MAX_ROW * SIZE_GRID + 4);

#if 0
    emit info(QString("column %1 %2")
              .arg(display->columnWidth(0))
              .arg(display->rowHeight(0)));
#endif

    display_number = new QLCDNumber;
    display_number->setFixedHeight(32);

    QVBoxLayout *vbox = new QVBoxLayout;
#if 0
    vbox->setSpacing(0);
    vbox->setMargin(0);
#endif

    QFrame *frame = new QFrame;

    vbox->addWidget(display);
    vbox->addWidget(display_number);

    frame->setLayout(vbox);

    emit info(QString("frame %1 %2")
              .arg(frame->sizeHint().width())
              .arg(frame->sizeHint().height()));

    ui->display_layout->addWidget(frame);

    setFixedSize(ui->frame->sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QCheckBox *cb_block = new QCheckBox("block");
    cb_block->setObjectName("cb_block");
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *frm_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    frm_choice_test->setObjectName("frm_choice_test");

    connect(frm_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), frm_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef void (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::get_label(QString filename, int num_label, int *label)
{
    if(filename.isEmpty())
    {
        emit error("filename is empty!");
        return false;
    }

    bool ok = false;
    QFile file(filename);
    ok = file.open(QIODevice::ReadOnly);
    if(!ok)
    {
        emit error(QString("file %1 not open").arg(filename));
        return false;
    }

    labels ls;
    memset(&ls, 0, sizeof(labels));
    qint64 res = file.read((char *)&ls, sizeof(ls));
    if(res != sizeof(ls))
    {
        emit error("error read magic number");
        return false;
    }

    emit debug(QString("magic_number 0x%1").arg(htons(htonl(ls.magic_number)), 0, 16));
    emit debug(QString("numbers_of_items %1").arg(htons(htonl(ls.numbers_of_items))));

    ok = file.seek(num_label + sizeof(ls));
    if(!ok)
    {
        emit error("error seek");
        return false;
    }

    QByteArray ba = file.read(1);
    emit debug(QString("data %1").arg((int)ba.at(0)));

    *label = ba.at(0);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::get_label_param(QString filename, int *numbers_of_items)
{
    if(filename.isEmpty())
    {
        emit error("filename is empty!");
        return false;
    }

    bool ok = false;
    QFile file(filename);
    ok = file.open(QIODevice::ReadOnly);
    if(!ok)
    {
        emit error(QString("file %1 not open").arg(filename));
        return false;
    }

    labels ls;
    memset(&ls, 0, sizeof(labels));
    qint64 res = file.read((char *)&ls, sizeof(ls));
    if(res != sizeof(ls))
    {
        emit error("error read magic number");
        return false;
    }

    *numbers_of_items = ls.numbers_of_items;
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::get_image(QString filename, int num_image, QImage *image)
{
    if(filename.isEmpty())
    {
        emit error("filename is empty!");
        return false;
    }
    if(image == 0)
    {
        emit error("image mot created");
        return false;
    }

    bool ok = false;
    QFile file(filename);
    ok = file.open(QIODevice::ReadOnly);
    if(!ok)
    {
        emit error(QString("file %1 not open").arg(filename));
        return false;
    }

    images is;
    memset(&is, 0, sizeof(is));
    qint64 res = file.read((char *)&is, sizeof(is));
    if(res != sizeof(is))
    {
        emit error("error read magic number");
        file.close();
        return false;
    }
    int numbers_of_items = htons(htonl(is.numbers_of_images));
    if((num_image > numbers_of_items) || (num_image < 0))
    {
        emit error("bad num_image");
        file.close();
        return false;
    }

    int row = htons(htonl(is.numbers_of_rows));
    int col = htons(htonl(is.numbers_of_columns));

    emit debug(QString("magic_number 0x%1").arg(htons(htonl(is.magic_number)), 0, 16));
    emit debug(QString("magic_number %1").arg(htons(htonl(is.magic_number))));
    emit debug(QString("numbers_of_images %1").arg(htons(htonl(is.numbers_of_images))));
    emit debug(QString("numbers_of_rows %1").arg(row));
    emit debug(QString("numbers_of_columns %1").arg(col));

    ok = file.seek(num_image * row * col + sizeof(is));
    if(!ok)
    {
        emit error("error seek");
        file.close();
        return false;
    }

    QByteArray ba = file.read(col * row);
    if(ba.length() != (col * row))
    {
        emit error("error read data");
        file.close();
        return false;
    }

    (*image).fill(Qt::black);
    for(int y=0; y<row; y++)
    {
        for(int x=0; x<col; x++)
        {
            (*image).setPixel(x, y, ba.at(y*row + x));
        }
    }
    file.close();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::get_image_data(QString filename,
                             int num_image,
                             QByteArray *data)
{
    if(filename.isEmpty())
    {
        emit error("filename is empty!");
        return false;
    }
    if(data == 0)
    {
        emit error("data array mot created");
        return false;
    }

    bool ok = false;
    QFile file(filename);
    ok = file.open(QIODevice::ReadOnly);
    if(!ok)
    {
        emit error(QString("file %1 not open").arg(filename));
        return false;
    }

    images is;
    memset(&is, 0, sizeof(is));
    qint64 res = file.read((char *)&is, sizeof(is));
    if(res != sizeof(is))
    {
        emit error("error read magic number");
        file.close();
        return false;
    }
    int numbers_of_items = htons(htonl(is.numbers_of_images));
    if((num_image > numbers_of_items) || (num_image < 0))
    {
        emit error("bad num_image");
        file.close();
        return false;
    }

    int row = htons(htonl(is.numbers_of_rows));
    int col = htons(htonl(is.numbers_of_columns));

    emit debug(QString("magic_number 0x%1").arg(htons(htonl(is.magic_number)), 0, 16));
    emit debug(QString("magic_number %1").arg(htons(htonl(is.magic_number))));
    emit debug(QString("numbers_of_images %1").arg(htons(htonl(is.numbers_of_images))));
    emit debug(QString("numbers_of_rows %1").arg(row));
    emit debug(QString("numbers_of_columns %1").arg(col));

    ok = file.seek(num_image * row * col + sizeof(is));
    if(!ok)
    {
        emit error("error seek");
        file.close();
        return false;
    }

    QByteArray ba = file.read(col * row);
    if(ba.length() != (col * row))
    {
        emit error("error read data");
        file.close();
        return false;
    }

    (*data).append(ba);
    file.close();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::get_image_param(QString filename,
                              int *numbers_of_images,
                              int *numbers_of_rows,
                              int *numbers_of_columns)
{
    if(filename.isEmpty())
    {
        emit error("filename is empty!");
        return false;
    }

    bool ok = false;
    QFile file(filename);
    ok = file.open(QIODevice::ReadOnly);
    if(!ok)
    {
        emit error(QString("file %1 not open").arg(filename));
        return false;
    }

    images is;
    memset(&is, 0, sizeof(is));
    qint64 res = file.read((char *)&is, sizeof(is));
    if(res != sizeof(is))
    {
        emit error("error read magic number");
        file.close();
        return false;
    }
    *numbers_of_images  = htons(htonl(is.numbers_of_images));
    *numbers_of_rows    = htons(htonl(is.numbers_of_rows));
    *numbers_of_columns = htons(htonl(is.numbers_of_columns));
    file.close();
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::click(int row, int col)
{
    if(!display) return;

    if (display->item(row, col)->backgroundColor() == Qt::black)
    {
        display->item(row, col)->setBackgroundColor(Qt::white);
    }
    else
    {
        display->item(row, col)->setBackgroundColor(Qt::black);
    }
}
//--------------------------------------------------------------------------------
void MainBox::show_image(void)
{
    QString filename_image = ":/data_NMIST/train-images-idx3-ubyte";
    QString filename_label = ":/data_NMIST/train-labels-idx1-ubyte";

    bool ok = false;

    int cnt = 0;
    int col = 0;
    int row = 0;

    ok = get_image_param(filename_image, &cnt, &row, &col);
    if(!ok)
    {
        emit error("error in param");
        return;
    }

    QByteArray ba;
    ok = get_image_data(filename_image, ui->sb_num_image->value(), &ba);
    if(ok)
    {
        int row = display->rowCount();
        int col = display->columnCount();
        if(ba.length() == (row * col))
        {
            for(int y=0; y<display->rowCount(); y++)
            {
                for(int x=0; x<display->columnCount(); x++)
                {
                    char value = ba.at(y*row + x);
                    //display->item(y, x)->setBackgroundColor(value);
                    display->item(y, x)->setBackgroundColor(qRgb(value, value, value));
                }
            }
        }
        else
        {
            emit error(QString("error ba.length == %1").arg(ba.length()));
            emit error(QString("row %1").arg(row));
            emit error(QString("col %1").arg(col));
            return;
        }
    }

    //---
    int num = 0;
    ok = get_label(filename_label, ui->sb_num_image->value(), &num);
    if(ok)
    {
        display_number->display(num);
    }
    //---

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    block_this_button(true);
    emit info("Test_0()");

    test_data.clear();

    QString filename_image = ":/data_NMIST/train-images-idx3-ubyte";
    QString filename_label = ":/data_NMIST/train-labels-idx1-ubyte";

    bool ok = false;
    QFile file_image(filename_image);
    QFile file_label(filename_label);

    ok = file_image.open(QIODevice::ReadOnly);
    if(!ok)
    {
        emit error(QString("file %1 not open").arg(filename_image));
        block_this_button(false);
        return;
    }
    ok = file_label.open(QIODevice::ReadOnly);
    if(!ok)
    {
        emit error(QString("file %1 not open").arg(filename_label));
        block_this_button(false);
        return;
    }

    images is;
    qint64 res = 0;
    memset(&is, 0, sizeof(is));
    res = file_image.read((char *)&is, sizeof(is));
    if(res != sizeof(is))
    {
        emit error("error read magic number");
        file_image.close();
        block_this_button(false);
        return;
    }

    labels ls;
    memset(&ls, 0, sizeof(labels));
    res = file_label.read((char *)&ls, sizeof(ls));
    if(res != sizeof(ls))
    {
        emit error("error read magic number");
        file_label.close();
        block_this_button(false);
        return;
    }

    int cnt_test_data = 0;
    while(cnt_test_data<60000)
    {
        QByteArray ba_image = file_image.read(MAX_COL*MAX_ROW);
        if(ba_image.length() != (MAX_COL*MAX_ROW))
        {
            emit error("error read image_data");
            block_this_button(false);
            return;
        }

        QByteArray ba_label = file_label.read(1);
        if(ba_label.length() != 1)
        {
            emit error("error read label_data");
            block_this_button(false);
            return;
        }

        data_NMIST data;
        data.label = ba_label.at(0);
        data.image.clear();
        data.image.append(ba_image);
        test_data.append(data);

        cnt_test_data++;
    }

    file_image.close();
    file_label.close();

    emit info(QString("read %1 images").arg(test_data.length()));
    emit info("OK");

    cnt_test_data=0;
    foreach (data_NMIST data, test_data)
    {
        emit info(QString("num %1").arg(data.label));
        //emit info(QString("image %1").arg(data.image.toHex().data()));

        cnt_test_data++;
        if(cnt_test_data>10)
        {
            block_this_button(false);
            return;
        }
    }
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("Test_5()");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
