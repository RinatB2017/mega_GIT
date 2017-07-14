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
#include "mysplashscreen.hpp"
//--------------------------------------------------------------------------------
MySplashScreen::MySplashScreen(const QPixmap &pixmap,
                               int max_progress,
                               QWidget *parent) :
    QSplashScreen(parent),
    current_progress(0),
    max_progress(max_progress)
{
    progress = new QProgressBar(this);
    progress->setValue(0);
    progress->setMaximum(max_progress);
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

    //label->installEventFilter(this);
}
//--------------------------------------------------------------------------------
void MySplashScreen::showMessage(const QString &message,
                                 int alignment,
                                 const QColor &color)
{
    QSplashScreen::showMessage(message, alignment, color);
    progress->setValue(current_progress);
    current_progress++;
    if(current_progress > max_progress)
    {
        current_progress=0;
    }
    repaint();
}
//--------------------------------------------------------------------------------
void MySplashScreen::set_progress(int value)
{
    current_progress = value;
    progress->setValue(current_progress);
    //repaint();
}
//--------------------------------------------------------------------------------
bool MySplashScreen::eventFilter(QObject *target, QEvent *event)
{
#ifdef QT_DEBUG
    qDebug() << "event!!!!";
#endif
    Q_UNUSED(target)
    Q_UNUSED(event)
    return true;
}
//--------------------------------------------------------------------------------
