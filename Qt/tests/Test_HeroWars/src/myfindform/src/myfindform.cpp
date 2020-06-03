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
#include "myfiledialog.hpp"
#include "mainwindow.hpp"
#include "showpicture.hpp"
#include "myfindform.hpp"
#include "ui_myfindform.h"
//--------------------------------------------------------------------------------
MyFindForm::MyFindForm(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MyFindForm)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
MyFindForm::~MyFindForm()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MyFindForm::init(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("Show picture", "show_picture", Qt::RightDockWidgetArea, ui->showpicture_widget);
    }

    connect(ui->btn_set,            &QToolButton::clicked,  this,   &MyFindForm::set_src_file);
    connect(ui->btn_show,           &QToolButton::clicked, [this]() {
        ui->showpicture_widget->show_picture(ui->le_screenshot->text());
    });

    connect(ui->btn_find_ok,        &QPushButton::clicked,  this,   &MyFindForm::find_ok);
    connect(ui->btn_find_auto,      &QPushButton::clicked,  this,   &MyFindForm::find_auto);
    connect(ui->btn_find_programm,  &QPushButton::clicked,  this,   &MyFindForm::find_programm);
    connect(ui->btn_find_to_battle, &QPushButton::clicked,  this,   &MyFindForm::find_to_battle);

    prepare_l(file_ok,          &l_file_ok);
    prepare_l(file_auto,        &l_file_auto);
    prepare_l(file_in_battle,   &l_file_in_battle);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MyFindForm::fail(void)
{
    emit error("Пока не сделано!");
}
//--------------------------------------------------------------------------------
bool MyFindForm::prepare_l(QString filename, QList<QRgb> *list)
{
    QImage *image = new QImage();
    bool ok = image->load(filename);
    if(!ok)
    {
        emit error(QString("Error load %1").arg(filename));
        return false;
    }

    (*list).clear();
    for(int y=0; y<image->height(); y++)
    {
        for(int x=0; x<image->width(); x++)
        {
            (*list).append(image->pixel(x, y));
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MyFindForm::prepare_temp(QString filename, QRect rect, QList<QRgb> *list)
{
    QImage *image = new QImage();
    bool ok = image->load(filename);
    if(!ok)
    {
        emit error(QString("Error load %1").arg(filename));
        return false;
    }

    (*list).clear();
    for(int y=rect.y(); y<(rect.y() + rect.height()); y++)
    {
        for(int x=rect.x(); x<(rect.x() + rect.width()); x++)
        {
            (*list).append(image->pixel(x, y));
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void MyFindForm::show_res(QString filename, QRect rect)
{
    QImage *src_image = new QImage();
    bool ok = src_image->load(filename);
    if(!ok)
    {
        emit error(QString("Error load %1").arg(src_file));
        return;
    }

    QLabel *label = new QLabel();
    QPainter painter(src_image);
    painter.setPen(Qt::yellow);
    painter.setBrush(QBrush(Qt::yellow));
    painter.drawRect(rect.x(),
                     rect.y(),
                     rect.width(),
                     rect.height());

    label->setPixmap(QPixmap::fromImage(*src_image));
    label->setMinimumSize(src_image->width(), src_image->height());
    label->show();
}
//--------------------------------------------------------------------------------
void MyFindForm::set_src_file(void)
{
    MyFileDialog *dlg = new MyFileDialog("find_form", "find_form", this);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->setDefaultSuffix("png");
    //dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        if(filename.isEmpty() == false)
        {
            ui->le_screenshot->setText(filename);
            ui->showpicture_widget->show_picture(filename);
        }
    }
}
//--------------------------------------------------------------------------------
void MyFindForm::find_ok(void)
{
    emit trace(Q_FUNC_INFO);

    QRect rect;
    QElapsedTimer timer;
    timer.start();
    src_file = ui->le_screenshot->text();
    bool ok = searchObjectByTemplate(src_file,
                                     file_ok,
                                     &rect);
    if(ok)
    {
        emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
        emit info(QString("%1 %2 %3 %4")
                  .arg(rect.x())
                  .arg(rect.y())
                  .arg(rect.width())
                  .arg(rect.height()));
        ok = prepare_temp(src_file, rect, &l_temp);
        if(!ok)
        {
            emit error("prepare_temp return false");
            return;
        }
        if(l_temp == l_file_ok)
        {
            emit info("OK");
        }
        else
        {
            emit error("FAIL");
        }
    }
    else
    {
        emit error("Not found");
    }
}
//--------------------------------------------------------------------------------
void MyFindForm::find_auto(void)
{
    emit trace(Q_FUNC_INFO);

    QRect rect;
    QElapsedTimer timer;
    timer.start();
    src_file = ui->le_screenshot->text();
    bool ok = searchObjectByTemplate(src_file,
                                     file_auto,
                                     &rect);
    if(ok)
    {
        emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
        emit info(QString("%1 %2 %3 %4")
                  .arg(rect.x())
                  .arg(rect.y())
                  .arg(rect.width())
                  .arg(rect.height()));
        ok = prepare_temp(src_file, rect, &l_temp);
        if(!ok)
        {
            emit error("prepare_temp return false");
            return;
        }
        if(l_temp == l_file_auto)
        {
            emit info("OK");
        }
        else
        {
            emit error("FAIL");
        }
    }
    else
    {
        emit error("Not found");
    }
}
//--------------------------------------------------------------------------------
void MyFindForm::find_programm(void)
{
    emit trace(Q_FUNC_INFO);
    fail();
}
//--------------------------------------------------------------------------------
void MyFindForm::find_to_battle(void)
{
    emit trace(Q_FUNC_INFO);

    QRect rect;
    QElapsedTimer timer;
    timer.start();
    src_file = ui->le_screenshot->text();
    bool ok = searchObjectByTemplate(src_file,
                                     file_in_battle,
                                     &rect);
    if(ok)
    {
        emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
        emit info(QString("%1 %2 %3 %4")
                  .arg(rect.x())
                  .arg(rect.y())
                  .arg(rect.width())
                  .arg(rect.height()));
        ok = prepare_temp(src_file, rect, &l_temp);
        if(!ok)
        {
            emit error("prepare_temp return false");
            return;
        }
        if(l_temp == l_file_in_battle)
        {
            emit info("OK");
        }
        else
        {
            emit error("FAIL");
        }
    }
    else
    {
        emit error("Not found");
    }
}
//--------------------------------------------------------------------------------
void MyFindForm::test(void)
{
    QRect rect;
    QElapsedTimer timer;
    timer.start();
    src_file = ui->le_screenshot->text();
    bool ok = searchObjectByTemplate(src_file,
                                     file_in_battle,
                                     &rect);
    if(ok)
    {
        emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
        emit info(QString("%1 %2 %3 %4")
                  .arg(rect.x())
                  .arg(rect.y())
                  .arg(rect.width())
                  .arg(rect.height()));
        show_res(file_in_battle, rect);
    }
    else
    {
        emit error("Not found");
    }
}
//--------------------------------------------------------------------------------
// https://www.cyberforum.ru/qt/thread1096383.html

bool MyFindForm::searchObjectByTemplate(QString srcImgName,
                                        QString templImgName,
                                        QRect *rect)
{
    emit trace(Q_FUNC_INFO);

    if(srcImgName.isEmpty())
    {
        emit error("srcImgName is empty!");
        return false;
    }
    if(templImgName.isEmpty())
    {
        emit error("templImgName is empty!");
        return false;
    }

    emit debug(srcImgName);
    emit debug(templImgName);

    QRect CCORR_result;
    QRect SQDIFF_result;
    QPair <QRect, QRect> resultPair;

    resultPair.first = QRect(0, 0, 0, 0);
    resultPair.second = QRect(1, 1, 0, 0);

    double min, max;
    CvPoint minpos, maxpos;

    char* imgName   = srcImgName.toLocal8Bit().data();
    char* templName = templImgName.toLocal8Bit().data();

    IplImage *src    = nullptr;
    IplImage *templ  = nullptr;
    IplImage *result = nullptr;

    src = cvLoadImage(imgName, CV_LOAD_IMAGE_GRAYSCALE);
    //    src = cvLoadImage(imgName, CV_LOAD_IMAGE_COLOR);
    if(!src)
    {
        emit error(QString("Error load %1").arg(srcImgName));
        return false;
    }

    templ = cvLoadImage(templName, CV_LOAD_IMAGE_GRAYSCALE);
    //    templ = cvLoadImage(templName, CV_LOAD_IMAGE_COLOR);
    if(!templ)
    {
        emit error(QString("Error load %1").arg(templImgName));
        return false;
    }

    result = cvCreateImage(cvSize(src->width - templ->width + 1, src->height - templ->height + 1), 32, 1);

    cvMatchTemplate(src, templ, result, CV_TM_CCORR_NORMED);
    cvNormalize(result, result, 1, 0, CV_MINMAX );
    cvMinMaxLoc(result, &min, &max, &minpos, &maxpos);

    CCORR_result = QRect(maxpos.x, maxpos.y, templ->width, templ->height);

    cvMatchTemplate(src, templ, result, CV_TM_SQDIFF_NORMED);
    cvNormalize(result, result, 1, 0, CV_MINMAX );
    cvMinMaxLoc(result, &min, &max, &minpos, &maxpos);

    SQDIFF_result = QRect(minpos.x, minpos.y, templ->width, templ->height);

    resultPair.first = CCORR_result;
    resultPair.second = SQDIFF_result;

    if(resultPair.first.x() == resultPair.second.x() &&
            resultPair.first.y() == resultPair.second.y())
    {
        *rect = resultPair.first;
        return true;
    }

    cvReleaseImage(&src);
    cvReleaseImage(&templ);
    cvReleaseImage(&result);
    return false;
}
//--------------------------------------------------------------------------------
void MyFindForm::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MyFindForm::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MyFindForm::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MyFindForm::save_setting(void)
{

}
//--------------------------------------------------------------------------------
