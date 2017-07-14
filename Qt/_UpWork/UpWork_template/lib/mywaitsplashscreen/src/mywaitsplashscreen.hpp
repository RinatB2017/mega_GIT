/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef MYWAITSPLASHSCREEN_HPP
#define MYWAITSPLASHSCREEN_HPP
//--------------------------------------------------------------------------------
#include <QSplashScreen>
#include <QPixmap>
//--------------------------------------------------------------------------------
class QProgressBar;
//--------------------------------------------------------------------------------
class MyWaitSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit MyWaitSplashScreen(const QPixmap &pixmap,
                                int max_step = 100,
                                QWidget *parent = 0);

public slots:
    void showMessage(const QString &message,
                     int alignment = Qt::AlignLeft,
                     const QColor &color = Qt::black);

private:
    QProgressBar *progress = 0;
    int m_progress = 0;
    int current_progress = 0;
    int max_step = 0;
};
//--------------------------------------------------------------------------------
#endif // MYWAITSPLASHSCREEN_HPP
