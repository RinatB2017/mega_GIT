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
#include <QMouseEvent>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QtMath>
//--------------------------------------------------------------------------------
#include <QFile>
//--------------------------------------------------------------------------------
#include <QPainter>
#include <QPixmap>
//--------------------------------------------------------------------------------
#include "ui_imitator_pet.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "imitator_pet.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "floodmap.hpp"
#include "viewer.hpp"
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Imitator_PET::Imitator_PET(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Imitator_PET)
{
    init();

    load_QDoubleSpinBox("Imitator_PET");
    load_QSpinBox("Imitator_PET");
    load_QComboBox("Imitator_PET");
    //load_widgets("Imitator_PET");    //TODO
}
//--------------------------------------------------------------------------------
Imitator_PET::~Imitator_PET()
{
    save_QDoubleSpinBox("Imitator_PET");
    save_QSpinBox("Imitator_PET");
    save_QComboBox("Imitator_PET");
    //save_widgets("Imitator_PET");    //TODO

    delete ui;
}
//--------------------------------------------------------------------------------
void Imitator_PET::init(void)
{
    ui->setupUi(this);

    create_widgets();

    main_source_x = DEFAULT_SIZE / 2;
    main_source_y = DEFAULT_SIZE / 2;

    control_source_x = DEFAULT_SIZE / 2;
    control_source_y = DEFAULT_SIZE / 2;

    redraw_all();

    ui->main_label->installEventFilter(this);
}
//--------------------------------------------------------------------------------
QString Imitator_PET::convert_int_to_string(uint32_t value)
{
    if(value >= 1e9)
    {
        return QString("%1 млрд.").arg((double)value / (double)1e9, 0, 'f', 3);
    }
    if(value >= 1e6)
    {
        return QString("%1 млн.").arg((double)value / (double)1e6, 0, 'f', 3);
    }
    if(value >= 1e3)
    {
        return QString("%1 тыс.").arg((double)value / (double)1e3, 0, 'f', 3);
    }
    return QString("%1").arg(value);
}
//--------------------------------------------------------------------------------
void Imitator_PET::calc_line(qreal center_x,
                        qreal center_y,
                        qreal angle,
                        qreal radius,
                        qreal *end_x,
                        qreal *end_y)
{
    //qDebug() << "calc_line" << center_x << center_y << angle << radius;

    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    //qDebug() << center_x + B << center_y + C;

    *end_x = center_x + B;
    *end_y = center_y + C;
}
//--------------------------------------------------------------------------------
qreal Imitator_PET::get_angle(qreal x1, qreal y1, qreal x2, qreal y2)
{
    double radians  = qAtan2(y2 - y1, x2 - x1); // Получаем угол.
    double grad     = radians * 180.0 / M_PI;
    if(grad < 0) grad += 360.0;

    return grad;
}
//--------------------------------------------------------------------------------
bool Imitator_PET::check_brg(int index_brg,
                        qreal center_x,
                        qreal center_y,
                        qreal angle)
{
    if(points.isEmpty())
    {
        messagebox_critical("Ошибка", "Сначала выберите файл данных!");
        return false;
    }

    if(index_brg < 0)
    {
        emit error(QString("index_brg %1 < 0").arg(index_brg));
        return false;
    }
    if(index_brg > 47)
    {
        emit error(QString("index_brg %1 > 47").arg(index_brg));
        return false;
    }

    point_data point = points.at(index_brg);

    qreal angle_1 = get_angle(center_x, center_y, point.x1, point.y1);
    qreal angle_2 = get_angle(center_x, center_y, point.x2, point.y2);

    if(angle_1 == angle_2)
    {
        return false;
    }

    if(angle_1 < angle_2)
    {
        if(angle < angle_1) return false;
        if(angle >= angle_2) return false;
    }
    else
    {
        if(angle >= angle_1) return true;
        if(angle < angle_2) return true;
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
void Imitator_PET::generate_control_events(void)
{
    block_this_button(true);

    pet_event event;
    Viewer *v = new Viewer();
    double xx = 0;
    double yy = 0;

    int brg_1 = 0;
    int brg_2 = 0;
    quint64 ts = 0;
    uint8_t  flags = 0;
    int max_point = MAX_POINT;
    int temp = 0xFFFF / max_point;
    control_events.clear();
    for(int index=0; index < (ui->sb_num_events->value()/2); index++)
    {
        if(ui->rb_circle->isChecked())
        {
            //---
            brg_data d = brg_data_lists.at(index % PET_ADDR_BRG_MAX);
            brg_1 = d.brg_1;
            brg_2 = d.brg_2;
            //---
            xx = 0x8000 + double((double)0x6500)*qCos(double(index)*double(M_PI)/double(180.0));
            yy = 0x8000 + double((double)0x6500)*qSin(double(index)*double(M_PI)/double(180.0));
            xx += ((rand() % 5000) - 2500);
            yy += ((rand() % 1000) - 500);

            event = v->get_point(brg_1, flags, xx, yy, ts);
            control_events.append(event);
            //
            xx = 0x8000 + double((double)0x6500)*qCos(double(index)*double(M_PI)/double(180.0));
            yy = 0x8000 + double((double)0x6500)*qSin(double(index)*double(M_PI)/double(180.0));
            xx += ((rand() % 5000) - 2500);
            yy += ((rand() % 1000) - 500);

            event = v->get_point(brg_2, flags, xx, yy, ts);
            control_events.append(event);
            //---
        }
        //---
        if(ui->rb_grid->isChecked())
        {
            //---
            brg_data d = brg_data_lists.at(index % PET_ADDR_BRG_MAX);
            brg_1 = d.brg_1;
            brg_2 = d.brg_2;
            //---
            int x = rand() % max_point;
            int y = rand() % max_point;

            xx = x * temp + temp / 2;
            yy = y * temp + temp / 2;

            xx += ((rand() % 200) - 100);
            yy += ((rand() % 200) - 100);

            event = v->get_point(brg_1, flags, xx, yy, ts);
            control_events.append(event);
            //---
            x = rand() % max_point;
            y = rand() % max_point;

            xx = x * temp + temp / 2;
            yy = y * temp + temp / 2;

            xx += ((rand() % 200) - 100);
            yy += ((rand() % 200) - 100);

            event = v->get_point(brg_2, flags, xx, yy, ts);
            control_events.append(event);
            //---
        }
        ts += (rand() % ui->sb_delta_ts->value());
    }
    messagebox_info("Информация", "generate_control_events");
    block_this_button(false);
}
//--------------------------------------------------------------------------------
bool Imitator_PET::comp(const pet_event &e1, const pet_event &e2)
{
#ifdef NEW_TIME
    U_TIME_STEP t1;
    t1.value = 0;
    t1.bytes.a = e1.ts.a;
    t1.bytes.b = e1.ts.b;
    t1.bytes.c = e1.ts.c;
    t1.bytes.d = e1.ts.d;
    t1.bytes.e = e1.ts.e;
    t1.bytes.f = e1.ts.f;

    U_TIME_STEP t2;
    t2.value = 0;
    t2.bytes.a = e2.ts.a;
    t2.bytes.b = e2.ts.b;
    t2.bytes.c = e2.ts.c;
    t2.bytes.d = e2.ts.d;
    t2.bytes.e = e2.ts.e;
    t2.bytes.f = e2.ts.f;

    return (t1.value < t2.value);
#endif
    return e1.ts < e2.ts;
}
//--------------------------------------------------------------------------------
void Imitator_PET::show_control_events(void)
{
    if(control_events.length()<1)
    {
        emit error("control_events: bad size");
        return;
    }
    block_this_button(true);

    emit info(QString("Анализируются %1 событий").arg(control_events.length()));

#ifdef NEW_TINE
    U_TIME_STEP t1;
    U_TIME_STEP t2;
#endif
    qSort(control_events.begin(), control_events.end(), comp);
    int cnt_double_events = 0;
    for(int n=0; n<control_events.length(); n+=2)
    {
#ifdef NEW_TIME
        t1.value = 0;
        t1.bytes.a = control_events.at(n).ts.a;
        t1.bytes.b = control_events.at(n).ts.b;
        t1.bytes.c = control_events.at(n).ts.c;
        t1.bytes.d = control_events.at(n).ts.d;
        t1.bytes.e = control_events.at(n).ts.e;
        t1.bytes.f = control_events.at(n).ts.f;

        t2.value = 0;
        t2.bytes.a = control_events.at(n+1).ts.a;
        t2.bytes.b = control_events.at(n+1).ts.b;
        t2.bytes.c = control_events.at(n+1).ts.c;
        t2.bytes.d = control_events.at(n+1).ts.d;
        t2.bytes.e = control_events.at(n+1).ts.e;
        t2.bytes.f = control_events.at(n+1).ts.f;

        if(t1.value > t2.value)
        {
            messagebox_critical("Ошибка", QString("Found %1").arg(n));
            block_this_button(false);
            return;
        }
        if(t1.value == t2.value)
        {
            cnt_double_events++;
        }
#endif
    }
    messagebox_info("Информация", QString("Found %1 double events").arg(cnt_double_events));
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Imitator_PET::draw_main_picture(void)
{
    QPixmap pixmap;
    pixmap = ui->main_label->pixmap()->copy();
    pixmap.fill(Qt::black);

    QPainter painter;
    painter.begin(&pixmap);
    painter.setPen(Qt::white);

    const qreal R = DEFAULT_R;
    const qreal center_x = CENTER_X;
    const qreal center_y = CENTER_Y;

    qreal end_x = 0;
    qreal end_y = 0;

    //---
    painter.setPen(Qt::gray);
    for(int n=0; n<360; n+=45)
    {
        calc_line(main_source_x, main_source_y, n, 1000, &end_x, &end_y);
        painter.drawLine(main_source_x, main_source_y, end_x, end_y);
    }

    QImage image;
    QString filename = NAME_RADIATION_ICON;
    bool ok = image.load(filename);
    if(ok)
    {
        painter.drawImage(main_source_x - image.width()/2, main_source_y - image.height()/2, image);
    }
    else
    {
        emit error(QString("%1 not loaded").arg(filename));
    }
    //---

    qreal inc = 360.0 / 48.0;
    qreal old_x = center_x + R;
    qreal old_y = center_y;
    bool flag = false;
    points.clear();
    qreal angle=inc;

    while(angle<=360)
    {
        calc_line(center_x,
                  center_y,
                  angle,
                  R,
                  &end_x,
                  &end_y);

        // https://www.opennet.ru/docs/RUS/qt3_prog/c4100.html
        painter.save();
        painter.setPen(Qt::white);

        painter.translate(old_x, old_y);
        //painter.rotate((180.0 - inc) / 2.0);
        painter.rotate(90.0 + angle - 4.0);

        double len = qSqrt((end_x - old_x)*(end_x - old_x) + (end_y - old_y)*(end_y - old_y));

        painter.drawRect(0, -20, len, 20);
        painter.drawText(len/2, -5, QString("%1").arg(angle / inc - 1));
        painter.restore();
        //---

        point_data point;
        point.brg = (int)(angle / inc - 1);
        point.x1 = old_x;
        point.y1 = old_y;
        point.x2 = end_x;
        point.y2 = end_y;
        points.append(point);

        flag = !flag;
        if(flag) painter.setPen(Qt::blue);
        else painter.setPen(Qt::green);

        old_x = end_x;
        old_y = end_y;
        angle+=inc;
    }

    painter.end();

    ui->main_label->setPixmap(pixmap);
}
//--------------------------------------------------------------------------------
void Imitator_PET::draw_control_picture(void)
{
    QPixmap pixmap;
    pixmap = ui->main_label->pixmap()->copy();
    pixmap.fill(Qt::black);

    QPainter painter;
    painter.begin(&pixmap);
    painter.setPen(Qt::white);

    const qreal R = DEFAULT_R;
    const qreal center_x = CENTER_X;
    const qreal center_y = CENTER_Y;

    qreal end_x = 0;
    qreal end_y = 0;

    //---
    painter.setPen(Qt::gray);
    for(int n=0; n<360; n+=45)
    {
        calc_line(control_source_x, control_source_y, n, 1000, &end_x, &end_y);
        painter.drawLine(control_source_x, control_source_y, end_x, end_y);
    }

    QImage image;
    QString filename = NAME_RADIATION_ICON;
    bool ok = image.load(filename);
    if(ok)
    {
        painter.drawImage(control_source_x - image.width()/2, control_source_y - image.height()/2, image);
    }
    else
    {
        emit error(QString("%1 not loaded").arg(filename));
    }
    //---

    qreal inc = 360.0 / 48.0;
    qreal old_x = center_x + R;
    qreal old_y = center_y;
    bool flag = false;
    points.clear();
    qreal angle=inc;

    while(angle<=360)
    {
        calc_line(center_x,
                  center_y,
                  angle,
                  R,
                  &end_x,
                  &end_y);

        // https://www.opennet.ru/docs/RUS/qt3_prog/c4100.html
        painter.save();
        painter.setPen(Qt::white);

        painter.translate(old_x, old_y);
        //painter.rotate((180.0 - inc) / 2.0);
        painter.rotate(90.0 + angle - 4.0);

        double len = qSqrt((end_x - old_x)*(end_x - old_x) + (end_y - old_y)*(end_y - old_y));

        painter.drawRect(0, -20, len, 20);
        painter.drawText(len/2, -5, QString("%1").arg(angle / inc - 1));
        painter.restore();
        //---

        point_data point;
        point.brg = (int)(angle / inc - 1);
        point.x1 = old_x;
        point.y1 = old_y;
        point.x2 = end_x;
        point.y2 = end_y;
        points.append(point);

        flag = !flag;
        if(flag) painter.setPen(Qt::blue);
        else painter.setPen(Qt::green);

        old_x = end_x;
        old_y = end_y;
        angle+=inc;
    }

    painter.end();

    ui->control_label->setPixmap(pixmap);
}
//--------------------------------------------------------------------------------
void Imitator_PET::redraw_all(void)
{
    draw_main_picture();
    draw_control_picture();
    update_brg_doubles();
}
//--------------------------------------------------------------------------------
void Imitator_PET::update_brg_doubles(void)
{
    emit info("update_brg_doubles");

    int index_brg_1 = 0;
    int index_brg_2 = 0;
    bool ok = false;

    brg_data_lists.clear();
    for(int angle=0; angle < 180; angle++)
    {
        index_brg_1 = -1;
        index_brg_2 = -1;
        for(int index_brg = 0; index_brg<48; index_brg++)
        {
            ok = check_brg(index_brg,
                           main_source_x,
                           main_source_y,
                           angle);
            if(ok)
            {
                index_brg_1 = index_brg;
            }
        }
        for(int index_brg = 0; index_brg<48; index_brg++)
        {
            ok = check_brg(index_brg,
                           main_source_x,
                           main_source_y,
                           angle + 180);
            if(ok)
            {
                index_brg_2 = index_brg;
            }
        }
        ok = false;
        foreach (brg_data data, brg_data_lists)
        {
            if((data.brg_1 == index_brg_1) && (data.brg_2 == index_brg_2))
            {
                ok = true;
                break;
            }
        }
        if(ok == false)
        {
            brg_data temp_data;
            temp_data.brg_1 = index_brg_1;
            temp_data.brg_2 = index_brg_2;
            brg_data_lists.append(temp_data);
        }
    }
}
//--------------------------------------------------------------------------------
void Imitator_PET::create_widgets(void)
{
    //---
    QPixmap *main_pixmap = new QPixmap(DEFAULT_SIZE, DEFAULT_SIZE);
    main_pixmap->fill(Qt::black);

    ui->main_label->setFixedSize(DEFAULT_SIZE, DEFAULT_SIZE);
    ui->main_label->setPixmap(*main_pixmap);
    //---

    //---
    QPixmap *control_pixmap = new QPixmap(DEFAULT_SIZE, DEFAULT_SIZE);
    control_pixmap->fill(Qt::black);

    ui->control_label->setFixedSize(DEFAULT_SIZE, DEFAULT_SIZE);
    ui->control_label->setPixmap(*control_pixmap);
    //---

    ui->sb_center_x->setRange(CENTER_X - DEFAULT_R, CENTER_X + DEFAULT_R);
    ui->sb_center_x->setValue(CENTER_X);

    ui->sb_center_y->setRange(CENTER_Y - DEFAULT_R, CENTER_Y + DEFAULT_R);
    ui->sb_center_y->setValue(CENTER_Y);

    ui->le_filename->setReadOnly(true);

    ui->sb_delta_ts->setRange(1, INT_MAX);

    ui->sb_num_events->setRange(1, INT_MAX);

    connect(ui->btn_choice_filename,    SIGNAL(clicked(bool)), this, SLOT(choice_filename()));
    connect(ui->btn_create_filename,    SIGNAL(clicked(bool)), this, SLOT(create_filename()));
    connect(ui->btn_set_source,         SIGNAL(clicked(bool)), this, SLOT(set_source()));

    connect(ui->btn_generate_control_events,    SIGNAL(clicked(bool)), this, SLOT(generate_control_events()));
    connect(ui->btn_show_control_events,        SIGNAL(clicked(bool)), this, SLOT(show_control_events()));
}
//--------------------------------------------------------------------------------
void Imitator_PET::choice_filename(void)
{
    QString filename;

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "ib_dlg");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    QStringList filters;
    filters << "Файлы данных (*.dat3)"
            << "Все файлы (*.*)";
    dlg->setNameFilters(filters);
    dlg->setDefaultSuffix(tr("dat3"));
    dlg->selectFile("без имени");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    delete dlg;

    if(filename.isEmpty())
    {
        return;
    }
    ui->le_filename->setText(filename);
}
//--------------------------------------------------------------------------------
void Imitator_PET::create_filename(void)
{
    block_this_button(true);

    QString filename = ui->le_filename->text();
    if(filename.isEmpty())
    {
        messagebox_critical("Ошибка", "Сначала выберите файл данных!");
        block_this_button(false);
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        emit error(QString(QObject::tr("Файл %1 не открыт!")).arg(filename));
        block_this_button(false);
        return;
    }

    pet_event event;
    Viewer *v = new Viewer();
    double xx = 0;
    double yy = 0;

    int brg_1 = 0;
    int brg_2 = 0;
    quint64 ts = 0;
    uint8_t  flags = 0;
    int max_point = MAX_POINT;
    int temp = 0xFFFF / max_point;
    qint64 bytes = 0;
    for(int index=0; index < ui->sb_num_events->value(); index++)
    {
        if(ui->rb_circle->isChecked())
        {
            //---
            brg_data data = brg_data_lists.at(rand() % brg_data_lists.length());
            brg_1 = data.brg_1;
            brg_2 = data.brg_2;
            //---
            xx = 0x8000 + double((double)0x6500)*qCos(double(index)*double(M_PI)/double(180.0));
            yy = 0x8000 + double((double)0x6500)*qSin(double(index)*double(M_PI)/double(180.0));
            xx += ((rand() % 5000) - 2500);
            yy += ((rand() % 1000) - 500);

            event = v->get_point(brg_1, flags, xx, yy, ts);
            bytes = file.write((char *)&event, sizeof(struct pet_event));
            if(bytes != sizeof(struct pet_event))
            {
                emit error("error write");
                break;
            }
            //
            xx = 0x8000 + double((double)0x6500)*qCos(double(index)*double(M_PI)/double(180.0));
            yy = 0x8000 + double((double)0x6500)*qSin(double(index)*double(M_PI)/double(180.0));
            xx += ((rand() % 5000) - 2500);
            yy += ((rand() % 1000) - 500);

            event = v->get_point(brg_2, flags, xx, yy, ts);
            bytes = file.write((char *)&event, sizeof(struct pet_event));
            if(bytes != sizeof(struct pet_event))
            {
                emit error("error write");
                break;
            }
            //
        }
        //---
        if(ui->rb_grid->isChecked())
        {
            //---
            brg_data data = brg_data_lists.at(rand() % brg_data_lists.length());
            brg_1 = data.brg_1;
            brg_2 = data.brg_2;
            //---
            int x = rand() % max_point;
            int y = rand() % max_point;

            xx = x * temp + temp / 2;
            yy = y * temp + temp / 2;

            xx += ((rand() % 200) - 100);
            yy += ((rand() % 200) - 100);

            event = v->get_point(brg_1, flags, xx, yy, ts);
            bytes = file.write((char *)&event, sizeof(struct pet_event));
            if(bytes != sizeof(struct pet_event))
            {
                emit error("error write");
                break;
            }
            //---
            x = rand() % max_point;
            y = rand() % max_point;

            xx = x * temp + temp / 2;
            yy = y * temp + temp / 2;

            xx += ((rand() % 200) - 100);
            yy += ((rand() % 200) - 100);

            event = v->get_point(brg_2, flags, xx, yy, ts);
            bytes = file.write((char *)&event, sizeof(struct pet_event));
            if(bytes != sizeof(struct pet_event))
            {
                emit error("error write");
                break;
            }
            //---
            ts += (rand() % ui->sb_delta_ts->value());
        }
    }
    file.close();
    messagebox_info("Создание файла", QString("Файл %1 успешно создан").arg((filename)));
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Imitator_PET::set_source(void)
{
    qreal x = ui->sb_center_x->value();
    qreal y = ui->sb_center_y->value();
    qreal len = qSqrt((x - CENTER_X)*(x - CENTER_X) + (y - CENTER_Y)*(y - CENTER_Y));

    if(len < DEFAULT_R)
    {
        main_source_x = x;
        main_source_y = y;
        draw_main_picture();
    }
}
//--------------------------------------------------------------------------------
bool Imitator_PET::eventFilter(QObject *obj, QEvent *event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
#if 0
    if(event->type() == QEvent::MouseMove)
    {
        qreal x = mouseEvent->pos().x();
        qreal y = mouseEvent->pos().y();
        emit info(QString("%1:%2").arg(x).arg(y));

        return true;
    }
#endif
    if(event->type() == QEvent::MouseButtonPress)
    {
        //emit info(QString("press %1:%2").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));

        qreal x = mouseEvent->pos().x();
        qreal y = mouseEvent->pos().y();
        qreal len = qSqrt((x - CENTER_X)*(x - CENTER_X) + (y - CENTER_Y)*(y - CENTER_Y));

        if(len < DEFAULT_R)
        {
            main_source_x = x;
            main_source_y = y;
            draw_main_picture();
        }

        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
void Imitator_PET::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
