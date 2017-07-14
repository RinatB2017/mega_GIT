/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QStyleOptionProgressBarV2>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QApplication>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QRect>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
    #include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
//--------------------------------------------------------------------------------
MyWaitSplashScreen::MyWaitSplashScreen(const QPixmap &pixmap,
                                       int max_step,
                                       QWidget *parent) :
    QSplashScreen(parent),
    m_progress(0),
    current_progress(0),
    max_step(max_step)
{
    progress = new QProgressBar(this);
    progress->setValue(0);
    progress->setMaximum(max_step);
    progress->setEnabled(true);

    QVBoxLayout *vbox = new QVBoxLayout();

    QLabel *label = new QLabel(this);
    label->setPixmap(pixmap);

    vbox->addWidget(label);
    vbox->addWidget(progress);

    setLayout(vbox);
    setCursor(Qt::BusyCursor);

    adjustSize();
    QRect available_geom = QDesktopWidget().availableGeometry();

    int app_left = available_geom.width() / 2 - width() / 2;
    int app_top  = available_geom.height() / 2 - height() / 2;

    setGeometry(app_left, app_top, width(), height());
    move(app_left, app_top);
}
//--------------------------------------------------------------------------------
void MyWaitSplashScreen::showMessage(const QString &message,
                                     int alignment,
                                     const QColor &color)
{
    QSplashScreen::showMessage(message, alignment, color);
    progress->setValue(current_progress);
    current_progress++;
    if(current_progress > max_step)
    {
        current_progress=0;
    }
    repaint();
    qApp->processEvents();
}
//--------------------------------------------------------------------------------
