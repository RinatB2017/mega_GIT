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
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    connect(ui->browser,            &TestBrowser::add_pixmap,   ui->worker_widget,  &Worker::add_widget);

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("Main", "main_frame", Qt::LeftDockWidgetArea, reinterpret_cast<QWidget *>(ui->main_frame));
        setVisible(false);
    }

    QString filename = ":/dev/shm/calc.html";
    if(QFile::exists(filename) == false)
    {
        QFile::copy(":/html/calc.html", filename);
    }

//    ui->browser->setUrl(QUrl("https://hero-wars.com"));
    ui->browser->setUrl(QUrl("file:///dev/shm/calc.html"));

    load_widgets();
}
//--------------------------------------------------------------------------------
void MyFindForm::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw != nullptr);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",             &MyFindForm::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test,    &QToolButton::clicked,  this,   &MyFindForm::choice_test);

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MyFindForm::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    // QList<MainBox::CMD>::iterator cmd_it = std::find_if(
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef void (MyFindForm::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MyFindForm::fail(void)
{
    emit error("Пока не сделано!");
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
// https://www.cyberforum.ru/qt/thread1096383.html

bool MyFindForm::searchObjectByTemplate(const char *imgName,
                                        const char *templName,
                                        QRect *rect)
{
    emit trace(Q_FUNC_INFO);

#ifdef Q_OS_LINUX
    QRect CCORR_result;
    QRect SQDIFF_result;
    QPair <QRect, QRect> resultPair;

    resultPair.first = QRect(0, 0, 0, 0);
    resultPair.second = QRect(1, 1, 0, 0);

    double min, max;
    CvPoint minpos, maxpos;

    IplImage src; //    = nullptr;
    IplImage templ; //  = nullptr;
    IplImage *result = nullptr;

    Mat cv_img = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
    src = cvIplImage(cv_img);

    Mat cv_templ = imread(templName, CV_LOAD_IMAGE_GRAYSCALE);
    templ = cvIplImage(cv_templ);

    result = cvCreateImage(cvSize(src.width - templ.width + 1, src.height - templ.height + 1), 32, 1);
    Q_CHECK_PTR(result);

    Q_ASSERT(src.width != 0);
    Q_ASSERT(src.height != 0);
    Q_ASSERT(templ.width != 0);
    Q_ASSERT(templ.height != 0);

    cvMatchTemplate(&src, &templ, result, TM_CCORR_NORMED);
    cvNormalize(result, result, 1, 0, NORM_MINMAX );
    cvMinMaxLoc(result, &min, &max, &minpos, &maxpos);

    CCORR_result = QRect(maxpos.x, maxpos.y, templ.width, templ.height);

    cvMatchTemplate(&src, &templ, result, TM_SQDIFF_NORMED);
    cvNormalize(result, result, 1, 0, NORM_MINMAX );
    cvMinMaxLoc(result, &min, &max, &minpos, &maxpos);

    SQDIFF_result = QRect(minpos.x, minpos.y, templ.width, templ.height);

    resultPair.first = CCORR_result;
    resultPair.second = SQDIFF_result;

    if(resultPair.first.x() == resultPair.second.x() &&
            resultPair.first.y() == resultPair.second.y())
    {
        *rect = resultPair.first;
        return true;
    }

    //cvReleaseImage(&src);
    //cvReleaseImage(&templ);
    cvReleaseImage(&result);
#else
    Q_UNUSED(imgName);
    Q_UNUSED(templName);
    Q_UNUSED(rect);
#endif
    return false;
}
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include <Windows.h>
#   include <WinUser.h>
#endif

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
    // не забудь добавить в .pro LIBS += -luser32

    SetCursorPos(pos.x(), pos.y());
    mouse_event(MOUSEEVENTF_LEFTDOWN,   pos.x(),    pos.y(),    0,  0);
    mouse_event(MOUSEEVENTF_LEFTUP,     pos.x(),    pos.y(),    0,  0);

    Q_UNUSED(button);
#endif
}
//--------------------------------------------------------------------------------
void MyFindForm::test(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void MyFindForm::check_pict(const QString &orig_file, const QString &copy_file)
{
    if(QFile::exists(copy_file) == false)
    {
        QFileInfo fi(copy_file);
        emit debug(fi.absolutePath());
        QDir dir;
        if(dir.exists(fi.absolutePath()) ==  false)
        {
            dir.mkdir(fi.absolutePath());
        }

        //emit error(QString("%1 не найден").arg(copy_file));
        bool ok = QFile::copy(orig_file, copy_file);
        if(ok)
        {
            emit info(QString("%1 создан").arg(copy_file));
        }
        else
        {
            emit error(QString("%1 НЕ создан").arg(copy_file));
        }
    }
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
