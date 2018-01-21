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
#   include <QApplication>
#   include <QMouseEvent>
#   include <QPainter>
#   include <QPushButton>
#   include <QToolButton>
#   include <QCheckBox>
#   include <QComboBox>
#   include <QProcess>
#   include <QToolBar>
#   include <QScreen>
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "webcamera.hpp"
#include "other.hpp"
//--------------------------------------------------------------------------------
#include "cards.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
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

    connect(ui->btn_kmines,         SIGNAL(clicked(bool)),  this,   SLOT(run_kmines()));
    connect(ui->btn_kpat,           SIGNAL(clicked(bool)),  this,   SLOT(run_kpat()));
    connect(ui->btn_kdiamond,       SIGNAL(clicked(bool)),  this,   SLOT(run_kdiamond()));
    connect(ui->btn_find_programm,  SIGNAL(clicked(bool)),  this,   SLOT(find_programm()));

    setFixedHeight(sizeHint().height());

    camera = new WebCamera(this);

    camera->show();
    ui->camera_layout->addWidget(camera);

    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
    load_config();
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
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
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
            typedef bool (MainBox::*function)(void);
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
void MainBox::mouse_click(unsigned int button, QPoint pos)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    QCursor::setPos(pos);
    XTestFakeButtonEvent( display, button, true, CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
void MainBox::mouse_release(unsigned int button)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    XTestFakeButtonEvent( display, button, false, CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
void MainBox::mouse_move_to(QPoint pos)
{
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return;
    }

    XTestFakeMotionEvent( display, 0, pos.x(), pos.y(), CurrentTime );

    XFlush( display );
    XCloseDisplay( display );
}
//--------------------------------------------------------------------------------
void MainBox::run_kmines(void)
{
    QString program = "kmines";
    QString program_name = "KMines";
    QStringList arguments;

    run_program(program, program_name, arguments);
}
//--------------------------------------------------------------------------------
void MainBox::run_kpat(void)
{
    QString program = "kpat";
    QString program_name = "KPatience";
    QStringList arguments;

    run_program(program, program_name, arguments);
}
//--------------------------------------------------------------------------------
void MainBox::run_kdiamond(void)
{
    QString program = "kdiamond";
    QString program_name = "KDiamond";
    QStringList arguments;

    run_program(program, program_name, arguments);
}
//--------------------------------------------------------------------------------
void MainBox::run_program(const QString program,
                          const QString program_name,
                          const QStringList arguments)
{
    if(program.isEmpty())
    {
        emit error("program is empty");
        return;
    }
    if(program_name.isEmpty())
    {
        emit error("program_name is empty");
        return;
    }

    QProcess *myProcess = new QProcess(this);
    connect(myProcess,  SIGNAL(started()),                      this,   SLOT(started()));
    connect(myProcess,  SIGNAL(finished(int)),                  this,   SLOT(finished(int)));
    connect(myProcess,  SIGNAL(error(QProcess::ProcessError)),  this,   SLOT(process_error(QProcess::ProcessError)));
    myProcess->start(program, arguments);

    ui->le_programm->setText(program_name);
}
//--------------------------------------------------------------------------------
bool MainBox::find_window(const QString programm_title, int *x, int *y, int *width, int *heigth)
{
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
}
//--------------------------------------------------------------------------------
void MainBox::find_programm(void)
{
    if(ui->le_programm->text().isEmpty())
    {
        messagebox_critical("Ошибка", "Название программы не выбрано");
        return;
    }

    QString title = ui->le_programm->text();
    QScreen *screen = QGuiApplication::primaryScreen();

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    bool ok = find_window(title, &x, &y, &w, &h);

    // KMimes 30 * 16
    if(ok)
    {
        QPixmap screen_shot = screen->grabWindow(0, x, y, w, h);
        screen_shot.save(QString("%1.png").arg(title));

#if 0
        int temp_x = 36;
        int temp_y = 91;

        QPainter paint;
        paint.begin(&screen_shot);

        for(int n=0; n<=16; n++)
        {
            paint.drawLine ( 0, temp_y, w, temp_y );
            temp_y+=24;
        }
        for(int n=0; n<=30; n++)
        {
            paint.drawLine ( temp_x, 0, temp_x, h );
            temp_x+=24;
        }

        paint.end();
#endif

        QLabel *lbl = new QLabel;
        lbl->setPixmap(screen_shot);
        lbl->installEventFilter(this);
        lbl->show();
    }
    else
    {
        emit error(QString("%1 not found!").arg(title));
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    test_card();

#if 0
    QRect available_geom = QDesktopWidget().availableGeometry();
    QPoint pos = QPoint(available_geom.center().x(),
                        available_geom.center().y());
    mouse_click(Qt::LeftButton, pos);
    mouse_release(Qt::LeftButton);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

#if 0
    Display* display = XOpenDisplay( nullptr );
    if( display == nullptr )
    {
        return false;
    }

    QRect available_geom = QDesktopWidget().availableGeometry();
    QPoint pos = QPoint(available_geom.center().x(),
                        available_geom.center().y());

    QPoint new_pos = QPoint(pos.x(), pos.y()+200);
    mouse_click(Qt::LeftButton, pos);
    mouse_move_to(new_pos);
    mouse_release(Qt::LeftButton);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_card(void)
{
    suits[SUIT_HEARTS]      = imread("cards/hearts.png");
    suits[SUIT_DIAMONDS]    = imread("cards/diamonds.png");
    suits[SUIT_SPADES]      = imread("cards/spades.png");
    suits[SUIT_CLUBS]       = imread("cards/clubs.png");

    ranks[RANK_ACE]         = imread("cards/ace.png");
    ranks[RANK_TWO]         = imread("cards/2.png");
    ranks[RANK_THREE]       = imread("cards/3.png");
    ranks[RANK_FOUR]        = imread("cards/4.png");
    ranks[RANK_FIVE]        = imread("cards/5.png");
    ranks[RANK_SIX]         = imread("cards/6.png");
    ranks[RANK_SEVEN]       = imread("cards/7.png");
    ranks[RANK_EIGHT]       = imread("cards/8.png");
    ranks[RANK_NINE]        = imread("cards/9.png");
    ranks[RANK_TEN]         = imread("cards/10.png");
    ranks[RANK_JACK]        = imread("cards/jack.png");
    ranks[RANK_QUEEN]       = imread("cards/queen.png");
    ranks[RANK_KING]        = imread("cards/king.png");

    ColorConversionCodes cardConvert = COLOR_BGR2GRAY;

    for (int i = SUIT_HEARTS; i <= SUIT_CLUBS; ++i)
    {
        cvtColor(suits[i], suits[i], cardConvert);
    }

    for (int i = RANK_ACE; i <= RANK_KING; ++i)
    {
        cvtColor(ranks[i], ranks[i], cardConvert);
    }

    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        emit error("Failed to open camera");
        return false;
    }

    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    emit info(QString("Size: %1 x %2").arg(width).arg(height));

    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    //namedWindow("Card", CV_WINDOW_AUTOSIZE);

    //??? setMouseCallback("Processed", mouseCallback, NULL);

    createTrackbar("Threshold", "Original", &threshholdValue, 255);


    createTrackbar("Min canny threshold", "Original", &minCannyThreshold, maxCannyThreshold);


    createTrackbar("rho", "Original", &rho, 100);
    createTrackbar("line threshold", "Original", &linesThreshold, 255);
    createTrackbar("minLineLength", "Original", &minLineLength, 1000);

    //createButton("OK", buttonCallback, "OK");

    bool quit = false;

    while (!quit)
    {
        Mat frame;

#if 1
        bool success = cap.read(frame);

        if (!success)
        {
            emit error("Failed to read frame");
            break;
        }
#else
        frame = imread("KPatience.png", CV_LOAD_IMAGE_COLOR);
#endif

        Mat drawing = frame.clone();

        //Mat dst;
        //erode(frame, dst, Mat(), Point(-1, -1), sliderIterations);

        Mat grayOut;
        cvtColor(frame, grayOut, COLOR_BGR2GRAY);

        Mat gaussianBlurOut;
        GaussianBlur(grayOut, gaussianBlurOut, Size(1, 1), 1000);
        //imshow("Gray out", resizeToWidth(grayOut, WINDOW_WIDTH));

        Mat threshOut;
        threshold(gaussianBlurOut, threshOut, threshholdValue, 255, THRESH_BINARY);
        imshow("Thresh out", resizeToWidth(threshOut, WINDOW_WIDTH));

        Mat thresh = threshOut.clone();

        vector<vector<Point>  > contours;
        vector<Vec4i> hierarchy;
        findContours(threshOut, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

        vector<vector<Point> > approxPolys(contours.size());
        vector<Rect> boundRects(contours.size());

        if (!hierarchy.empty())
        {
            for (size_t contourIndex = 0; contourIndex < contours.size(); ++contourIndex)
            {
                vector<Point> contour = contours[contourIndex];

                double peri = arcLength(contour, true);
                //cout << "Perimeter: " << peri << "\n";

                if (peri > 1000.0 && hierarchy[contourIndex][3] == -1) // has no parent and perimeter big enough
                {
                    approxPolyDP(Mat(contour), approxPolys[contourIndex], 0.001 * peri, true);
                    boundRects[contourIndex] = boundingRect(Mat(approxPolys[contourIndex]));

                    vector<Point> poly = approxPolys[contourIndex];

                    if (poly.size() >= 4)
                    {
                        vector<uint32_t> lineLengths;

                        for (size_t pointIndex = 0; pointIndex < poly.size(); ++pointIndex)
                        {
                            Point current = poly[pointIndex];
                            Point previous = (pointIndex == 0) ? poly[poly.size() - 1] : poly[pointIndex - 1];

                            uint32_t length = norm(current - previous);

                            lineLengths.push_back(length);

                            //circle(drawing, current, 1, Scalar());
                        }

                        std::sort(lineLengths.begin(), lineLengths.end(), std::greater<int>());

                        int shortestLine = lineLengths[3];

                        vector<Vec4f> lines;

                        for (size_t pointIndex = 0; pointIndex < poly.size(); ++pointIndex)
                        {
                            Point current = poly[pointIndex];
                            Point previous = (pointIndex == 0) ? poly[poly.size() - 1] : poly[pointIndex - 1];

                            uint32_t length = norm(current - previous);

                            if (length >= shortestLine)
                            {
                                //line(drawing, previous, current, contoursColor, 5);

                                Vec4f currentLine;

                                // start x
                                currentLine[0] = 0;

                                // start y
                                currentLine[1] = ((float)previous.y - current.y) / (previous.x - current.x) * -previous.x + previous.y;

                                // end x
                                currentLine[2] = frame.cols;

                                // end y
                                currentLine[3] = ((float)previous.y - current.y) / (previous.x - current.x) * (frame.cols - current.x) + current.y;

                                lines.push_back(currentLine);

                                //line(drawing, Point2f(currentLine[0], currentLine[1]), Point2f(currentLine[2], currentLine[3]), contoursColor, 5);
                            }
                        }

                        vector<Point2f> corners;

                        for (size_t i = 0; i < lines.size(); ++i)
                        {
                            for (size_t j = i + 1; j < lines.size(); ++j)
                            {
                                Point2f pt = computeIntersect(lines[i], lines[j]);
                                if (pt.x >= 0 && pt.y >= 0)
                                {
                                    corners.push_back(pt);

                                    circle(drawing, pt, 10, dotColor);
                                }
                            }
                        }

                        Point2f center = calculateCenter(corners);
                        circle(drawing, center, 20, dotColor);

                        sortCorners(corners, center);

                        if (corners.size() == 4)
                        {
                            line(drawing, corners[0], corners[1], contoursColor, 4);
                            line(drawing, corners[1], corners[2], contoursColor, 4);
                            line(drawing, corners[2], corners[3], contoursColor, 4);
                            line(drawing, corners[3], corners[0], contoursColor, 4);

                            //630mm x 870mm
                            vector<Point2f> np;
                            np.push_back(Point(0, 0));
                            np.push_back(Point(cardSize.width, 0));
                            np.push_back(Point(cardSize.width, cardSize.height));
                            np.push_back(Point(0, cardSize.height));

                            Mat transform = getPerspectiveTransform(corners, np);

                            Mat warp;
                            warpPerspective(frame, warp, transform, Point(cardSize.width, cardSize.height));

                            Mat warp2;
                            warpPerspective(thresh, warp2, transform, Point(cardSize.width, cardSize.height));

                            if (!warp.empty() && !warp2.empty())
                            {
                                Card card = processCard(warp, warp2);

                                stringstream s;
                                s << rankToString(card.rank) << " of " << suitToString(card.suit);

                                putText(drawing, s.str(), center, FONT_HERSHEY_SIMPLEX, 2.0, textColor, 3, 8, false);
                            }
                        }
                    }

                    rectangle(drawing, boundRects[contourIndex].tl(), boundRects[contourIndex].br(), rectColor, 2, 8, 0);
                }
            }
        }

        imshow("Original", resizeToWidth(drawing, WINDOW_WIDTH));
        //imshow("Processed", resizeToWidth(threshOut, WINDOW_WIDTH));

        int key = waitKey(30);

        switch (key)
        {
        case 27:
            quit = true;
            break;
        }
    }

    //destroyWindow("Test");
    cvDestroyAllWindows();

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    emit info(tr("Процесс начат!"));
}
//--------------------------------------------------------------------------------
void MainBox::finished(int result)
{
    emit info(tr("Процесс завершен!"));
    if(result)
    {
        emit error(QString(tr("code %1")).arg(result));
    }
}
//--------------------------------------------------------------------------------
void MainBox::process_error(QProcess::ProcessError p_error)
{
    switch(p_error)
    {
    case QProcess::FailedToStart:
        emit error("FailedToStart");
        break;

    case QProcess::Crashed:
        emit error("Crashed");
        break;

    case QProcess::Timedout:
        emit error("Timedout");
        break;

    case QProcess::ReadError:
        emit error("ReadError");
        break;

    case QProcess::WriteError:
        emit error("WriteError");
        break;

    case QProcess::UnknownError:
        emit error("UnknownError");
        break;
    }
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    if(mouseEvent == nullptr)
    {
        return false;
    }
    //---
    if(mouseEvent->type() == QMouseEvent::MouseButtonPress)
    {
        emit info(QString("%1 %2")
                  .arg(mouseEvent->pos().x())
                  .arg(mouseEvent->pos().y()));
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
