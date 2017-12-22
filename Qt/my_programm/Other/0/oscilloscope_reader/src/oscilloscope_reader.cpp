/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
//--------------------------------------------------------------------------------
#include "oscilloscope_reader.hpp"
#include "ui_oscilloscope_reader.h"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
OscilloscopeBox_reader::OscilloscopeBox_reader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OscilloscopeBox_reader)
{
    init();
}
//--------------------------------------------------------------------------------
OscilloscopeBox_reader::~OscilloscopeBox_reader()
{
    if(grapher)
    {
        grapher->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox_reader::init(void)
{
    ui->setupUi(this);
    if(parentWidget())
    {
        connect(this, SIGNAL(info(QString)),    parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parentWidget(), SIGNAL(message(QString)));
    }

    cnt_samples = 0;
    index_samples = 0;

    ui->btn_choose_flle->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_choose_flle->setToolTip("Выбор байта");

    ui->btn_prev->setArrowType(Qt::LeftArrow);
    ui->btn_next->setArrowType(Qt::RightArrow);

    ui->btn_prev->setToolTip("На предыдущую осциллограмму");
    ui->btn_next->setToolTip("На следующую осциллограмму");

    grapher = new GrapherBox(0, 640, -32768, 32768, "", "Время, нс", "", true, this);

    grapher->set_zoom(false, true);
    grapher->set_panning(false, true);

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Statistic(false);

    grapher->push_btn_Horizontal(true);
    grapher->push_btn_Vertical(true);

    ui->le_filename->setReadOnly(true);

    curve_xp = grapher->add_curve("X+");
    curve_xn = grapher->add_curve("X-");
    curve_yp = grapher->add_curve("Y+");
    curve_yn = grapher->add_curve("Y-");

    ui->layout_scope->addWidget(grapher);

    connect(ui->btn_choose_flle, SIGNAL(clicked(bool)), this, SLOT(choose_file()));
    connect(ui->btn_prev,   SIGNAL(clicked(bool)),  this,   SLOT(prev_osc()));
    connect(ui->btn_next,   SIGNAL(clicked(bool)),  this,   SLOT(next_osc()));
}
//--------------------------------------------------------------------------------
void OscilloscopeBox_reader::choose_file(void)
{
    QFileDialog *dlg;
    QString filename;

    dlg = new QFileDialog;
    dlg->setNameFilters(QStringList() << "*.osc");
    //dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDefaultSuffix(tr("osc"));
#ifdef HOME
    dlg->setDirectory("/dev/shm");
#endif
    int res = dlg->exec();
    if(res != QDialog::Accepted)
    {
        return;
    }
    QStringList files = dlg->selectedFiles();
    filename = files.at(0);
    delete dlg;

    if(filename.isEmpty())
    {
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", QString("file %1 not open!").arg(filename));
        return;
    }
    cnt_samples = 0;
    QByteArray ba;
    samples.clear();
    while (!file.atEnd())
    {
        ba = file.readLine();
        //emit info(QString("1 read %1 bytes").arg(ba.size()));
        QByteArray temp = QByteArray::fromHex(ba);
        //emit info(QString("2 read %1 bytes").arg(temp.size()));

        //ANSWER_CMD_WAVE_HEADER *header = (ANSWER_CMD_WAVE_HEADER *)temp.data();
        //emit info(QString("samples_cnt %1").arg(header->body.samples_cnt));

        pet_wave *packet = (pet_wave *)temp.data();
        QByteArray sample;
        sample.clear();
        for(int n=0; n<packet->count; n++)
        {
            pet_wave_point temp;
            temp.xn = packet->point[n].xn;
            temp.xp = packet->point[n].xp;
            temp.yn = packet->point[n].yn;
            temp.yp = packet->point[n].yp;
            sample.append((char*)&temp, sizeof(struct pet_wave));
        }
        samples.append(sample);
        cnt_samples++;
#if 0
        ba.clear();
        ba = file.read(sizeof(ANSWER_CMD_WAVE_HEADER));
        emit info(QString("choose_file (1): read %1 bytes").arg(ba.size()));
        if(ba.size() != sizeof(ANSWER_CMD_WAVE_HEADER))
        {
            QMessageBox::critical(this, "Error", "error read HEADER_CMD_WAVE!");
            file.close();
            return;
        }
        ANSWER_CMD_WAVE_HEADER *header = (ANSWER_CMD_WAVE_HEADER *)ba.data();
        //emit message("--------------");
        //emit message(ba.toHex());
        emit info(QString("samples_cnt %1").arg(header->body.samples_cnt));
        cnt_samples++;
        for(int n=0; n<header->body.samples_cnt; n++)
        {
            ba.clear();
            ba = file.read(sizeof(SAMPLE));
            //emit info(QString("choose_file (2): read %1 bytes").arg(ba.size()));
            if(ba.size() != sizeof(SAMPLE))
            {
                emit error(QString("ba.size %1 sizeof(SAMPLE) %2")
                           .arg(ba.size())
                           .arg(sizeof(SAMPLE)));
                QMessageBox::critical(this, "Error", "error read SAMPLE!");
                file.close();
                return;
            }
            SAMPLE *sample = (SAMPLE *)ba.data();
            SAMPLE temp;
            temp.XN = sample->XN;
            temp.XP = sample->XP;
            temp.YN = sample->YN;
            temp.YP = sample->YP;
            samples.append(temp);
        }
#endif
    }
    emit info(QString("cnt_samples %1").arg(cnt_samples));
    file.close();
    QMessageBox::information(this, "Информация", QString("Прочитано %1 записей").arg(cnt_samples));

    grapher->set_silense(true);
    ui->le_filename->setText(filename);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox_reader::prev_osc(void)
{
    if(samples.isEmpty())
    {
        emit error("no data");
        return;
    }
    if(index_samples > 0)
    {
        index_samples--;
    }
    grapher->set_silense(true);
    grapher->clear();
    grapher->set_silense(false);
    for(int n=0; n<samples[index_samples].size(); n+=sizeof(struct pet_wave))
    {
        pet_wave_point *sample = (pet_wave_point *)samples[index_samples].mid(n, sizeof(struct pet_wave)).data();
        grapher->add_curve_data(curve_xp, sample->xp);
        grapher->add_curve_data(curve_xn, sample->xn);
        grapher->add_curve_data(curve_yp, sample->yp);
        grapher->add_curve_data(curve_yn, sample->yn);
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox_reader::next_osc(void)
{
    if(samples.isEmpty())
    {
        emit error("no data");
        return;
    }
    if(index_samples < samples.count())
    {
        index_samples++;
    }
    grapher->set_silense(true);
    grapher->clear();
    grapher->set_silense(false);
    for(int n=0; n<samples[index_samples].size(); n+=sizeof(struct pet_wave))
    {
        pet_wave_point *sample = (pet_wave_point *)samples[index_samples].mid(n, sizeof(struct pet_wave)).data();
        grapher->add_curve_data(curve_xp, sample->xp);
        grapher->add_curve_data(curve_xn, sample->xn);
        grapher->add_curve_data(curve_yp, sample->yp);
        grapher->add_curve_data(curve_yn, sample->yn);
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox_reader::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QPushButton *pb=qobject_cast<QPushButton *>(btn))
    {
        pb->setDisabled(state);
    }
    if (QToolButton *tb=qobject_cast<QToolButton *>(btn))
    {
        tb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox_reader::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
