/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QDialog>
#include <QLabel>

#include <QDateTime>
#include <QDate>
#include <QTime>

#include <QFile>
#include <QMenu>

#include <limits.h>
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include <qwt_plot_histogram.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_magnifier.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot.h>
//--------------------------------------------------------------------------------
#include "ui_histogrambox.h"

#include "histogram_options.hpp"
#include "histogram_settings.hpp"

#include "mainwindow.hpp"
#include "histogrambox.hpp"
#include "csvreader.hpp"
//--------------------------------------------------------------------------------
HistogramBox::HistogramBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::HistogramBox)
{
    axis_X_min = -100;
    axis_X_max = 100;
    axis_Y_min = -100;
    axis_Y_max = 100;

    title = "title";
    title_axis_X = "X";
    title_axis_Y = "Y";

    init();
}
//--------------------------------------------------------------------------------
void HistogramBox::set_axis_scale_x(double new_axis_X_min, double new_axis_X_max)
{
    //    emit info(QString("set_axis_scale_x: %1 %2")
    //              .arg(new_axis_X_min)
    //              .arg(new_axis_X_max));
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, new_axis_X_min, new_axis_X_max);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::set_axis_scale_y(double new_axis_Y_min, double new_axis_Y_max)
{
    //    emit info(QString("set_axis_scale_y: %1 %2")
    //              .arg(new_axis_Y_min)
    //              .arg(new_axis_Y_max));
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,   new_axis_Y_min, new_axis_Y_max);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::set_title(const QString &title)
{
    if(title.isEmpty()) return;

    this->title = title;
    ui->qwtPlot->setTitle(title);
}
//--------------------------------------------------------------------------------
void HistogramBox::set_title_axis_X(const QString &title)
{
    if(title.isEmpty()) return;

    this->title_axis_X = title;
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, title);
}
//--------------------------------------------------------------------------------
void HistogramBox::set_title_axis_Y(const QString &title)
{
    if(title.isEmpty()) return;

    this->title_axis_Y = title;
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, title);
}
//--------------------------------------------------------------------------------
void HistogramBox::set_histogram_style(int index_histogram,
                                       QwtPlotHistogram::HistogramStyle style)
{
    if(histograms.count() == 0)
    {
        emit error(tr("histograms.count() == 0"));
        return;
    }
    if(index_histogram >= histograms.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(histograms.count()));
        return;
    }
    histograms[index_histogram].plot_histogram->setStyle(style);
}
//--------------------------------------------------------------------------------
int HistogramBox::add_histogram(int index_histogram,
                                const QString &title,
                                const QColor &color)
{
    HISTOGRAM_CURVE histogram;

    histogram.is_active = true;
    histogram.title = title;
    histogram.color = color;
    histogram.pos_x = 0;
    histogram.index_histogram = index_histogram;
    histogram.plot_histogram = new QwtPlotHistogram();
    histogram.plot_histogram->setStyle(QwtPlotHistogram::Columns);
    histogram.plot_histogram->setBrush(QBrush(color, Qt::SolidPattern));
    histogram.plot_histogram->setPen(color);
    histogram.plot_histogram->attach(ui->qwtPlot);
    histogram.plot_histogram->setSamples(histogram.data_histogram);
    show_histogram(histogram.plot_histogram, true);

    histograms.append(histogram);
    updateText();

    return histograms.count() - 1;
}
//--------------------------------------------------------------------------------
int HistogramBox::get_histograms_count(void)
{
    return histograms.count();
}
//--------------------------------------------------------------------------------
int HistogramBox::get_pos_x(int index_histogram)
{
    if(histograms.isEmpty())
    {
        emit error(tr("Нет данных"));
        return 0;
    }
    if(index_histogram < 0)
    {
        emit error(tr("Индекс не должен быть отрицательным"));
        return 0;
    }
    if(index_histogram >= histograms.count())
    {
        emit error(tr("Индекс слишком большой"));
        return 0;
    }
    return histograms[index_histogram].pos_x;
}
//--------------------------------------------------------------------------------
void HistogramBox::remove_histogram(int index_histogram)
{
    if(histograms.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    if(index_histogram < 0)
    {
        emit error(tr("Индекс не должен быть отрицательным"));
        return;
    }
    if(index_histogram >= histograms.count())
    {
        emit error(tr("Индекс слишком большой"));
        return;
    }
    emit info(QString("count %1").arg(histograms.count()));
    delete histograms[index_histogram].plot_histogram;
    histograms.remove(index_histogram);
    emit info(QString("count %1").arg(histograms.count()));
}
//--------------------------------------------------------------------------------
QVariant HistogramBox::itemToInfo(QwtPlotItem *plotItem) const
{
    QVariant itemInfo;
    qVariantSetValue(itemInfo, plotItem);

    return itemInfo;
}
//--------------------------------------------------------------------------------
void HistogramBox::show_histogram(QwtPlotItem *item, bool on)
{
    item->setVisible(on);

    QList<QWidget *> legendWidgets = legend->legendWidgets(itemToInfo(item));

    if(legendWidgets.size() == 1)
    {
        QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>(legendWidgets[0]);

        if(legendLabel)
            legendLabel->setChecked(on);
    }

    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::create_widgets(void)
{
    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->attach(ui->qwtPlot);

    // panning with the left mouse button
    new QwtPlotPanner(ui->qwtPlot->canvas());

    // zoom in/out with the wheel
    new QwtPlotMagnifier(ui->qwtPlot->canvas());

    legend = new QwtLegend();
    legend->setDefaultItemMode(QwtLegendData::Checkable);
    ui->qwtPlot->insertLegend( legend, QwtPlot::RightLegend );
    connect(legend, SIGNAL(checked(QVariant, bool, int)), this, SLOT(legend_checked(QVariant, bool)));

    d_picker = new QwtPlotPicker(QwtPlot::xBottom,
                                 QwtPlot::yLeft,
                                 QwtPlotPicker::CrossRubberBand,
                                 QwtPicker::AlwaysOn,
                                 ui->qwtPlot->canvas());

    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::blue));
    d_picker->setStateMachine(new QwtPickerDragPointMachine());

    if(title.isEmpty() == false)
        ui->qwtPlot->setTitle(title);

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,   axis_Y_min, axis_Y_max);

    if(title_axis_X.isEmpty() == false)
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, title_axis_X);

    if(title_axis_Y.isEmpty() == false)
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, title_axis_Y);

    connect(ui->btn_Options, SIGNAL(clicked()), this, SLOT(options()));
    connect(ui->btn_Clear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->btn_Load, SIGNAL(clicked()), this, SLOT(load_histograms()));
    connect(ui->btn_Save, SIGNAL(clicked()), this, SLOT(save_histograms()));

    connect(ui->btn_Horizontal, SIGNAL(clicked()), this, SLOT(tune_horizontal_axis()));
    connect(ui->btn_Vertical, SIGNAL(clicked()), this, SLOT(tune_vertical_axis()));

    connect(ui->btn_Statistic, SIGNAL(clicked()), this, SLOT(statistic()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popup(QPoint)));

    ui->btn_Horizontal->setToolTip(tr("Вместить график по горизонтали"));
    ui->btn_Vertical->setToolTip(tr("Вместить график по вертикали"));
    ui->btn_Clear->setToolTip(tr("Стереть график"));
}
//--------------------------------------------------------------------------------
void HistogramBox::popup(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *setting_action = new QAction(qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                          QObject::tr("настройки"),
                                          this);

    popup_menu->addAction(setting_action);
    popup_menu->addSeparator();

    connect(setting_action,  SIGNAL(triggered()), this, SLOT(slot_setting_action()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void HistogramBox::slot_setting_action(void)
{
    HistogramSettings *dlg = new HistogramSettings(histograms, d_picker, this);
    int button = dlg->exec();
    if(button == QDialog::Accepted)
    {
        QwtPicker::RubberBand rubber = dlg->get_rubberband();
        d_picker->setRubberBandPen(dlg->get_rubberband_color());
        d_picker->setRubberBand(rubber);

        foreach (HISTOGRAM_CURVE histogram, histograms)
        {
            bool ok = false;
            int r = 0;
            int g = 0;
            int b = 0;
            dlg->get_RGB(histogram.index_histogram, &r, &g, &b, &ok);
            if(ok)
            {
                histogram.plot_histogram->setPen(QColor(r, g, b));
                histogram.plot_histogram->setBrush(QColor(r, g, b));
                bool ok = false;
                QString title = dlg->get_caption(histogram.index_histogram, &ok);
                if(ok)
                {
                    histogram.plot_histogram->setTitle(title);
                }
                updateGraphics();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void HistogramBox::legend_checked(const QVariant &itemInfo, bool on)
{
    QwtPlotItem *item = itemInfo.value<QwtPlotItem *>();
    item->setVisible(on);
    tune_horizontal_axis(ui->btn_Horizontal->isChecked());
    tune_vertical_axis(ui->btn_Vertical->isChecked());
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::clear(void)
{
    int button = QMessageBox::question(this,
                                       tr("Очищение гистограм"),
                                       tr("Вы уверены, что вы хотите очистить ВСЕ гистограммы?"));
    if(button != QMessageBox::Yes) return;

    for(int n=0; n<histograms.count(); n++)
    {
        histograms[n].data_histogram.clear();
        histograms[n].pos_x = 0;
        histograms[n].plot_histogram->setSamples(histograms[n].data_histogram);
    }
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::reset(void)
{
    int button = QMessageBox::question(this,
                                       tr("Удаление гистограм"),
                                       tr("Вы уверены, что вы хотите удалить ВСЕ гистограммы?"));
    if(button != QMessageBox::Yes) return;

    int c = histograms.count();
    for(int n=0; n<c; n++)
    {
        remove_histogram(0);
    }
    updateGraphics();
}
//--------------------------------------------------------------------------------
HistogramBox::~HistogramBox()
{
    histograms.clear();
    delete ui;
}
//--------------------------------------------------------------------------------
void HistogramBox::init()
{
    ui->setupUi(this);

    create_widgets();
    updateText();
    load_setting();
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::add_histogram_data(int index_histogram,
                                      unsigned int pos_x,
                                      unsigned int width,
                                      int height)
{
    if(histograms.count() == 0)
    {
        emit error(tr("histograms.count() == 0"));
        return;
    }
    if(index_histogram >= histograms.count())
    {
        emit error(QString(tr("index_histogram > %1"))
                   .arg(histograms.count()));
        return;
    }
    histograms[index_histogram].data_histogram.append(QwtIntervalSample((double)height,
                                                                        (double)pos_x,
                                                                        (double)pos_x+width));
    if((pos_x+width) > histograms[index_histogram].pos_x)
        histograms[index_histogram].pos_x = (pos_x + width);

    tune_horizontal_axis(ui->btn_Horizontal->isChecked());
    tune_vertical_axis(ui->btn_Vertical->isChecked());

    histograms[index_histogram].plot_histogram->setSamples(histograms[index_histogram].data_histogram);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::add_histogram_data(int index_histogram,
                                      unsigned int width,
                                      int height)
{
    if(histograms.count() == 0)
    {
        emit error(tr("histograms.count() == 0"));
        return;
    }
    if(index_histogram >= histograms.count())
    {
        emit error(QString(tr("channel > %1")).arg(histograms.count()));
        return;
    }

    histograms[index_histogram].data_histogram.append(QwtIntervalSample(height,
                                                                        histograms[index_histogram].pos_x,
                                                                        histograms[index_histogram].pos_x+width));
    histograms[index_histogram].pos_x += width;

    tune_horizontal_axis(ui->btn_Horizontal->isChecked());
    tune_vertical_axis(ui->btn_Vertical->isChecked());

    histograms[index_histogram].plot_histogram->setSamples(histograms[index_histogram].data_histogram);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::updateGraphics(void)
{
    ui->qwtPlot->replot();
}
//--------------------------------------------------------------------------------
void HistogramBox::updateText()
{
    ui->retranslateUi(this);

    for(int n=0; n<histograms.count(); n++)
    {
        histograms[n].plot_histogram->setTitle(histograms[n].title);
    }
}
//--------------------------------------------------------------------------------
void HistogramBox::options(void)
{
    //qDebug() << axis_X_min << axis_X_max << axis_Y_min << axis_Y_max;
    Histogram_Options *dlg = new Histogram_Options(axis_X_min,
                                                   axis_X_max,
                                                   axis_Y_min,
                                                   axis_Y_max);
    dlg->setWindowIcon(QIcon(ICON_PROGRAMM));
    int button = dlg->exec();
    if(button == QDialog::Accepted)
    {
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom, dlg->get_min_axis_x(), dlg->get_max_axis_x());
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft,   dlg->get_min_axis_y(), dlg->get_max_axis_y());
        updateGraphics();
    }
}
//--------------------------------------------------------------------------------
void HistogramBox::load_histograms(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setWindowIcon(QIcon(ICON_PROGRAMM));
    dlg->setNameFilter(tr("CSV files (*.csv)"));
    dlg->setDefaultSuffix(tr("csv"));
#ifdef Q_OS_LINUX
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
#endif
    dlg->setFileMode(QFileDialog::ExistingFile);
    dlg->setDirectory(".");

    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QFile file(files.at(0));
        CsvReader *csv = new CsvReader(0,files.at(0));
        csv->set_new_separator(';');
        if(csv->Open())
        {
            for(int n=0; n<histograms.count(); n++)
            {
                histograms[n].data_histogram.clear();
            }
            QList<QStringList> str = csv->CSVRead();
            foreach (QStringList sl, str)
            {
                if(sl.count() == 3)
                {
                    bool ok = false;
                    int i = sl.at(0).toInt(&ok);
                    if(!ok) i=0;
                    float x = sl.at(1).toFloat(&ok);
                    if(!ok) x=histograms.at(i).pos_x;
                    histograms[i].data_histogram.append(QwtIntervalSample(i, x, x+1));
                }
                else
                {
                    emit error("error data");
                    break;
                }

            }
        }
        file.close();
        updateGraphics();
    }
}
//--------------------------------------------------------------------------------
void HistogramBox::save_histograms(void)
{
    if(histograms.isEmpty())
    {
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              tr("Нет данных (histograms)"));
        return;
    }

    for(int n=0; n<histograms.count(); n++)
    {
        if(histograms[n].data_histogram.size() == 0)
        {
            QMessageBox::critical(this,
                                  tr("Ошибка"),
                                  tr("Нет данных (sample)"));
            return;
        }
    }

    //записываем файл
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setWindowIcon(QIcon(ICON_PROGRAMM));
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("CSV files (*.csv)"));
    dlg->setDefaultSuffix(tr("csv"));
#ifdef Q_OS_LINUX
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
#endif
    dlg->setDirectory(".");
    dlg->selectFile(QString("%1")
                    .arg(histograms.at(0).title));
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QFile file(files.at(0));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            emit error(QString(tr("file %1 not writed!"))
                       .arg(files.at(0)));
            return;
        }

        for(int n=0; n<histograms.count(); n++)
        {
            for(int x=0; x<histograms[n].data_histogram.size(); x++)
            {
                QString temp = QString("%1;%2\n")
                        .arg(n)
                        .arg(histograms[n].data_histogram.data()->value);
                file.write(temp.toLocal8Bit());
            }
        }
        file.close();
        emit info(QString(tr("файл %1 записан успешно"))
                  .arg(files.at(0)));
        dlg->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void HistogramBox::tune_vertical_axis(bool state)
{
    if(!state) return;

    if(histograms.isEmpty())
    {
        emit error(tr("Нет данных!"));
        return;
    }

    double max_y = INT_MIN;
    double min_y = INT_MAX;

    min_y = 0;
    for(int n=0; n<histograms.count(); n++)
    {
        for(int x=0; x<histograms[n].data_histogram.count(); x++)
        {
            double temp_y = histograms[n].data_histogram[x].value;
            if(temp_y > max_y) max_y = temp_y;
            if(temp_y < min_y) min_y = temp_y;
        }
    }
    if(min_y != max_y)
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft, min_y, max_y);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::tune_horizontal_axis(bool state)
{
    if(!state) return;

    if(histograms.isEmpty())
    {
        emit error(tr("Нет данных!"));
        return;
    }

    double max_x = INT_MIN;
    double min_x = 0;

    min_x = 0;
    for(int n=0; n<histograms.count(); n++)
    {
        double max_value = 0;
        for(int x=0; x<histograms[n].data_histogram.count(); x++)
        {
            max_value += histograms[n].pos_x;
            if(max_value > max_x) max_x = max_value;
        }
    }
    if(min_x != max_x)
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom, min_x, max_x);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void HistogramBox::statistic(void)
{
    emit info(tr("Статистика:"));
    for(int n=0; n<histograms.count(); n++)
    {
        float max_value = INT_MIN;
        float min_value = INT_MAX;
        double value = 0;
        double average = 0;
        int cnt = histograms[n].data_histogram.size();
        for(int x=0; x<cnt; x++)
        {
            value = histograms[n].data_histogram.data()->value;
            average += value;
            if(value > max_value) max_value = value;
            if(value < min_value) min_value = value;
        }
        emit info(QString("   График %1, кол-во точек %2").arg(n).arg(cnt));
        if(cnt > 0)
        {
            average /= (double)cnt;
            emit info(QString("     min %1").arg(min_value));
            emit info(QString("     max %1").arg(max_value));
            emit info(QString("     average %1").arg(average));
        }
    }
}
//--------------------------------------------------------------------------------
void HistogramBox::setVisible_btn_Options(bool state)
{
    ui->btn_Options->setVisible(state);
}
//--------------------------------------------------------------------------------
void HistogramBox::setVisible_btn_Load(bool state)
{
    ui->btn_Load->setVisible(state);
}
//--------------------------------------------------------------------------------
void HistogramBox::setVisible_btn_Save(bool state)
{
    ui->btn_Save->setVisible(state);
}
//--------------------------------------------------------------------------------
void HistogramBox::setVisible_btn_Statistic(bool state)
{
    ui->btn_Statistic->setVisible(state);
}
//--------------------------------------------------------------------------------
void HistogramBox::push_btn_Horizontal(bool state)
{
    ui->btn_Horizontal->setChecked(state);
    tune_horizontal_axis(state);
}
//--------------------------------------------------------------------------------
void HistogramBox::push_btn_Vertical(bool state)
{
    ui->btn_Vertical->setChecked(state);
    tune_vertical_axis(state);
}
//--------------------------------------------------------------------------------
void HistogramBox::test(void)
{
    //test_draw_circle();
    test_get_info();
}
//--------------------------------------------------------------------------------
void HistogramBox::test_get_info(void)
{
    //QwtPlotCanvas *canvas = new QwtPlotCanvas();
    //canvas->setFrameStyle(QFrame::Box | QFrame::Plain);
    //canvas->setLineWidth(1);
    //canvas->setPalette(Qt::white);
    //ui->qwtPlot->setCanvas(canvas);

    ui->qwtPlot->setCanvasBackground(QBrush(Qt::white));
    updateGraphics();

    QwtPicker::RubberBand temp = d_picker->rubberBand();
    switch(temp)
    {
    case QwtPicker::NoRubberBand:       emit info("NoRubberBand"); break;
    case QwtPicker::HLineRubberBand:    emit info("HLineRubberBand"); break;
    case QwtPicker::VLineRubberBand:    emit info("VLineRubberBand"); break;
    case QwtPicker::CrossRubberBand:    emit info("CrossRubberBand"); break;
    case QwtPicker::RectRubberBand:     emit info("RectRubberBand"); break;
    case QwtPicker::EllipseRubberBand:  emit info("EllipseRubberBand"); break;
    case QwtPicker::PolygonRubberBand:  emit info("PolygonRubberBand"); break;
    case QwtPicker::UserRubberBand:     emit info("UserRubberBand"); break;
    }

    emit info(QString("canvas %1:%2:%3")
              .arg(ui->qwtPlot->canvasBackground().color().red())
              .arg(ui->qwtPlot->canvasBackground().color().green())
              .arg(ui->qwtPlot->canvasBackground().color().blue()));
    foreach (HISTOGRAM_CURVE histogram, histograms)
    {
        emit info(QString("histogram %1 %2:%3:%4")
                  .arg(histogram.title)
                  .arg(histogram.plot_histogram->pen().color().red())
                  .arg(histogram.plot_histogram->pen().color().green())
                  .arg(histogram.plot_histogram->pen().color().blue()));
    }
}
//--------------------------------------------------------------------------------
void HistogramBox::load_setting(void)
{
}
//--------------------------------------------------------------------------------
void HistogramBox::save_setting(void)
{
}
//--------------------------------------------------------------------------------
