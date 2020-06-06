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
#include "defines.hpp"
#include "ui_myfindform.h"
//--------------------------------------------------------------------------------
// Хроники Хаоса | Ролевая онлайн игра
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include "wingdi.h"
#   include "tchar.h"
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include "other.hpp"
#endif
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

    ui->sb_pos_x->setRange(0,   0xFFFF);
    ui->sb_pos_y->setRange(0,   0xFFFF);

    connect(ui->btn_click,          &QToolButton::clicked,      this,   &MyFindForm::click);
    connect(ui->sb_pos_x,           &QSpinBox::editingFinished, this,   &MyFindForm::click);
    connect(ui->sb_pos_y,           &QSpinBox::editingFinished, this,   &MyFindForm::click);

    connect(ui->btn_set,            &QToolButton::clicked,      this,   &MyFindForm::set_src_file);
    connect(ui->btn_show,           &QToolButton::clicked, [this]() {
        ui->showpicture_widget->show_picture(ui->le_screenshot->text());
    });

    add_buttons();
    connect(ui->btn_test,           &QPushButton::clicked,      this,   &MyFindForm::test);
    connect(ui->btn_find_programm,  &QPushButton::clicked,      this,   &MyFindForm::find_programm);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MyFindForm::click(void)
{
    QPoint pos;
    pos.setX(ui->sb_pos_x->value());
    pos.setY(ui->sb_pos_y->value());
    mouse_click(Qt::LeftButton, pos);
}
//--------------------------------------------------------------------------------
void MyFindForm::fail(void)
{
    emit error("Пока не сделано!");
}
//--------------------------------------------------------------------------------
bool MyFindForm::prepare_l(QString filename, QList<QRgb> *list, QRect *rect)
{
    QImage *image = new QImage();
    bool ok = image->load(filename);
    if(!ok)
    {
        emit error(QString("Error load %1").arg(filename));
        return false;
    }

    (*rect).setX(0);
    (*rect).setY(0);
    (*rect).setWidth(image->width());
    (*rect).setHeight(image->height());

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
bool MyFindForm::prepare_temp(QString filename,
                              QRect rect,
                              QList<QRgb> *list,
                              QRect *out_rect)
{
    QImage *image = new QImage();
    bool ok = image->load(filename);
    if(!ok)
    {
        emit error(QString("Error load %1").arg(filename));
        return false;
    }

    *out_rect = rect;

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
        ok = prepare_temp(src_file, rect, &l_temp, &rect_temp);
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
            show_rect_picture("src_file",   l_temp,     rect);
            show_rect_picture("l_file_ok",  l_file_ok,  rect_file_ok);
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
        ok = prepare_temp(src_file, rect, &l_temp, &rect_temp);
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
            show_rect_picture("src_file",       l_temp,         rect);
            show_rect_picture("l_file_auto",    l_file_auto,    rect_file_auto);
            emit error("FAIL");
        }
    }
    else
    {
        emit error("Not found");
    }
}
//--------------------------------------------------------------------------------
void MyFindForm::find_auto_active(void)
{
    emit trace(Q_FUNC_INFO);

    QRect rect;
    QElapsedTimer timer;
    timer.start();
    src_file = ui->le_screenshot->text();
    bool ok = searchObjectByTemplate(src_file,
                                     file_auto_active,
                                     &rect);
    if(ok)
    {
        emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
        emit info(QString("%1 %2 %3 %4")
                  .arg(rect.x())
                  .arg(rect.y())
                  .arg(rect.width())
                  .arg(rect.height()));
        ok = prepare_temp(src_file, rect, &l_temp, &rect_temp);
        if(!ok)
        {
            emit error("prepare_temp return false");
            return;
        }
        if(l_temp == l_file_auto_active)
        {
            emit info("OK");
        }
        else
        {
            show_rect_picture("src_file",           l_temp,                 rect);
            show_rect_picture("l_file_auto_active", l_file_auto_active,     rect_file_auto_active);
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
    bool ok = find_programm_with_title(ui->le_title->text());
    if(ok)
    {
        emit info("Find!");
    }
    else
    {
        emit error("FAIL");
    }
}
//--------------------------------------------------------------------------------
bool MyFindForm::find_programm_with_title(const QString &title)
{
    if(title.isEmpty())
    {
        emit error("title is empty");
        return false;
    }

    int x = -1;
    int y = -1;
    int w = -1;
    int h = -1;
    bool ok = find_window(title, &x, &y, &w, &h);
    if(ok)
    {
        emit info(QString("%1 %2 %3 %4")
                  .arg(x)
                  .arg(y)
                  .arg(w)
                  .arg(h));

        QScreen *screen = QGuiApplication::primaryScreen();
        Q_CHECK_PTR(screen);
        QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);

#if 1
        QDateTime now = QDateTime::currentDateTime();
        QString temp_file       = QString("/dev/shm/temp_screenshot_%1.png")
                .arg(now.toString("dd_MM_yyyy_hh_mm_ss"));

        screen_shot.save(temp_file);

        ui->le_screenshot->setText(temp_file);
        ui->showpicture_widget->show_picture(temp_file);
#else
        QLabel *lbl = new QLabel();
        lbl->setPixmap(screen_shot);
        lbl->show();
#endif
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MyFindForm::find_window(const QString programm_title,
                             int *x,
                             int *y,
                             int *width,
                             int *heigth)
{
#ifdef Q_OS_LINUX
    Display* display = XOpenDisplay( nullptr );
    Q_CHECK_PTR(display);

    bool is_found = false;
    ulong count = 0;
    Window* wins = findWindows( display, &count );
    char* name;
    QString temp;

    for( ulong i = 0; i < count; ++i )
    {
        Window w = wins[ i ];
        name = getWindowName( display, w );
        if( name )
        {
            temp.clear();
            temp.append(name);
            XFree( name );
        }

        XWindowAttributes attrs;
        if( XGetWindowAttributes( display, w, &attrs ) )
        {
            Window child;
            if( XTranslateCoordinates( display,
                                       w, attrs.root,
                                       0, 0,
                                       &attrs.x, &attrs.y,
                                       &child
                                       ) )
            {
                if(temp.contains(programm_title))
                {
                    *x = attrs.x;
                    *y = attrs.y;
                    *width = attrs.width;
                    *heigth = attrs.height;
                    is_found = true;
                }
            }
        }
    }

    if( wins )
    {
        XFree( wins );
    }

    XCloseDisplay( display );

    return is_found;
#else
    return false;
#endif

#ifdef Q_OS_WIN
    RECT rc;
//    HWND hwnd = ::FindWindow(0, _T("Calculator"));
    HWND hwnd = ::FindWindow(0, _T("Проводник"));
    if (hwnd == NULL)
    {
        emit error("hwnd == NULL");
        return false;
    }
    GetClientRect(hwnd, &rc);

    //create
    HDC hdcScreen = GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,
                                          rc.right - rc.left,
                                          rc.bottom - rc.top);
    SelectObject(hdc, hbmp);

    //Print to memory hdc
#if 1
    WINDOWINFO wi;
    GetWindowInfo(hwnd, &wi);

    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcScreen, wi.rcClient.left, wi.rcClient.top, SRCCOPY);
#else
    // так не получится сфотографировать калькулятор
    bool ok = PrintWindow(hwnd, hdc, PW_CLIENTONLY);
    if(!ok)
    {
        emit error("PrintWindow return false");
    }
#endif

    //copy to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hbmp);
    CloseClipboard();

    //release
    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);

    emit info(QString("%1 %2 %3 %4")
              .arg(rc.left)
              .arg(rc.top)
              .arg(rc.right)
              .arg(rc.bottom));
    emit info("OK");
    return true;
#endif
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
        ok = prepare_temp(src_file, rect, &l_temp, &rect_temp);
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
            show_rect_picture("src_file",           l_temp,             rect);
            show_rect_picture("l_file_in_battle",   l_file_in_battle,   rect_file_in_battle);

            for(int n=0; n<10; n++)
            {
                emit info(QString("%1 %2")
                          .arg(l_temp.at(n),            0,  16)
                          .arg(l_file_in_battle.at(n),  0,  16));
            }
            emit error("FAIL");
        }
    }
    else
    {
        emit error("Not found");
    }
}
//--------------------------------------------------------------------------------
void MyFindForm::show_rect_picture(QString caption, QList<QRgb> array, QRect rect)
{
    emit info(QString("%1: %2 %3 %4 %5")
              .arg(caption)
              .arg(rect.x())
              .arg(rect.y())
              .arg(rect.width())
              .arg(rect.height()));

    QImage *image = new QImage(rect.width(), rect.height(), QImage::Format_RGBX8888);

    int index = 0;
    for(int y=0; y<rect.height(); y++)
    {
        for(int x=0; x<rect.width(); x++)
        {
            image->setPixel(x, y, array.at(index++));
        }
    }

    QLabel *label = new QLabel();
    label->setToolTip(caption);
    label->setWindowTitle(caption);
    label->setPixmap(QPixmap::fromImage(*image));
    label->show();
}
//--------------------------------------------------------------------------------
// https://www.cyberforum.ru/qt/thread1096383.html

bool MyFindForm::searchObjectByTemplate(QString srcImgName,
                                        QString templImgName,
                                        QRect *rect)
{
    emit trace(Q_FUNC_INFO);

#ifdef Q_OS_LINUX
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

#endif
    return false;
}
//--------------------------------------------------------------------------------
void MyFindForm::mouse_click(unsigned int button, QPoint pos)
{
#ifdef Q_OS_LINUX
    Display* display = XOpenDisplay( nullptr );
    Q_CHECK_PTR(display);
    if( display == nullptr )
    {
        emit error("display == nullptr");
        return;
    }

    QCursor::setPos(pos);
    XTestFakeButtonEvent( display, button,  true,   CurrentTime );
    XTestFakeButtonEvent( display, button,  false,  CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
#endif

#ifdef Q_OS_WIN
    // https://stackoverflow.com/questions/8272681/how-can-i-simulate-a-mouse-click-at-a-certain-position-on-the-screen
    //int MOUSEEVENTF_LEFTDOWN  = 0x02;
    //int MOUSEEVENTF_LEFTUP    = 0x04;

    SetCursorPos(pos.x(), pos.y());
    mouse_event(MOUSEEVENTF_LEFTDOWN,   pos.x(),    pos.y(),    0,  0);
    mouse_event(MOUSEEVENTF_LEFTUP,     pos.x(),    pos.y(),    0,  0);
#endif
}
//--------------------------------------------------------------------------------
void MyFindForm::test(void)
{

}
//--------------------------------------------------------------------------------
void MyFindForm::add_buttons(void)
{
    typedef struct
    {
        QString btn_text;
        QString file_str;
        void (MyFindForm::*func)(void);
    } BTN;
    QList<BTN> buttons;

    buttons.clear();
    buttons.append( { "OK",             file_ok,            &MyFindForm::find_ok} );
    buttons.append( { "Auto",           file_auto,          &MyFindForm::find_auto} );
    buttons.append( { "Auto(active)",   file_auto_active,   &MyFindForm::find_auto_active} );
    buttons.append( { "Battle",         file_in_battle,     &MyFindForm::find_to_battle} );

    int row = 0;
    foreach (BTN btn, buttons)
    {
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap(btn.file_str));

        QPushButton *button = new QPushButton();
        button->setText(btn.btn_text);
        connect(button, &QPushButton::clicked,  this,   btn.func);

        ui->grid->addWidget(label,  row,    0);
        ui->grid->addWidget(button, row,    1);
        row++;
    }

    prepare_l(file_ok,          &l_file_ok,             &rect_file_ok);
    prepare_l(file_auto,        &l_file_auto,           &rect_file_auto);
    prepare_l(file_auto_active, &l_file_auto_active,    &rect_file_auto_active);
    prepare_l(file_in_battle,   &l_file_in_battle,      &rect_file_in_battle);
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
