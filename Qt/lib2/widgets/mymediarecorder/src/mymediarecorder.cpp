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
#include "mymediarecorder.hpp"
#include "ui_mymediarecorder.h"
//--------------------------------------------------------------------------------
MyMediaRecorder::MyMediaRecorder(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MyMediaRecorder)
{
    init();
}
//--------------------------------------------------------------------------------
MyMediaRecorder::~MyMediaRecorder()
{
    out.release();
    delete ui;
}
//--------------------------------------------------------------------------------
void MyMediaRecorder::init(void)
{
    ui->setupUi(this);

#if 0
    ui->btn_filename->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
#else
    ui->btn_filename->setIcon(QIcon(":/nuvola/actions/actions/document-open.png"));
    ui->btn_start->setIcon(QIcon(":/nuvola/actions/actions/media-record.png"));
    ui->btn_stop->setIcon(QIcon(":/nuvola/actions/actions/media-playback-stop.png"));
#endif

    connect(ui->btn_filename,   &QToolButton::clicked,  this,   &MyMediaRecorder::set_filename);
    connect(ui->btn_start,      &QToolButton::clicked,  this,   &MyMediaRecorder::start);
    connect(ui->btn_stop,       &QToolButton::clicked,  this,   &MyMediaRecorder::stop);

    ui->lbl_filename->setText(QFileInfo(filename).fileName());

    ui->btn_start->setEnabled(true);
    ui->btn_stop->setDisabled(true);
}
//--------------------------------------------------------------------------------
void MyMediaRecorder::get_frame(QVideoFrame frame)
{
    if(out.isOpened())
    {
        // https://forum.qt.io/topic/88694/converting-between-qvideoframe-and-opencv-mat/7
        QImage img = frame.image();

        //Mat mat(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
        Mat mat(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());

        cvtColor( mat, mat, COLOR_BGR2RGB );

        out.write(mat);
    }
}
//--------------------------------------------------------------------------------
void MyMediaRecorder::set_filename(void)
{
    MyFileDialog *dlg = new MyFileDialog("mediarecorder_box", "mediarecorder_box");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("AVI files (*.avi)");
    dlg->setDefaultSuffix("avi");
    dlg->selectFile(filename);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString t_filename = files.at(0);
        if(t_filename.isEmpty() == false)
        {
            filename = QFileInfo(t_filename).absoluteFilePath();
            ui->lbl_filename->setText(QFileInfo(filename).fileName());
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void MyMediaRecorder::start(void)
{
    int frame_width = 1280;
    int frame_height = 720;
    out.open(filename.toStdString().c_str(),
             // VideoWriter::fourcc('X','V','I','D'),
             // VideoWriter::fourcc('m','p','4','v'),
             VideoWriter::fourcc('h','2','6','4'),
             30.0,
             Size(frame_width, frame_height),
             true);
    if(out.isOpened())
    {
        emit info("open");
        ui->btn_start->setDisabled(true);
        ui->btn_stop->setEnabled(true);
    }
    else
    {
        emit error("NOT open");
        ui->btn_start->setEnabled(true);
        ui->btn_stop->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyMediaRecorder::stop(void)
{
    out.release();
    ui->btn_start->setEnabled(true);
    ui->btn_stop->setDisabled(true);
}
//--------------------------------------------------------------------------------
void MyMediaRecorder::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MyMediaRecorder::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MyMediaRecorder::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MyMediaRecorder::save_setting(void)
{

}
//--------------------------------------------------------------------------------
