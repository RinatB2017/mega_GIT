/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef MYSPLASHSCREEN_HPP
#define MYSPLASHSCREEN_HPP
//--------------------------------------------------------------------------------
#include <QSplashScreen>
#include <QPixmap>
//--------------------------------------------------------------------------------
class QProgressBar;
//--------------------------------------------------------------------------------
class MySplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit MySplashScreen(const QPixmap &pixmap,
                            int max_progress = 100,
                            QWidget *parent = 0);

    void set_progress(int value);

public slots:
    void showMessage(const QString &message,
                     int alignment = Qt::AlignLeft,
                     const QColor &color = Qt::black);

private:
    QProgressBar *progress = 0;
    int current_progress = 0;
    int max_progress = 0;
    
protected:
    bool eventFilter(QObject *target, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MYSPLASHSCREEN_HPP
